#pragma once
#include "pch.h"
#include "renderer.h"

using namespace DirectX;
using namespace motheye::dx12;
using Microsoft::WRL::ComPtr;

namespace motheye::renderer
{
    void Renderer::Destroy()
    {
        // Ensure that the GPU is no longer referencing resources that are about to be
        // cleaned up by the destructor.
        commandQueue_.Flush();

        resourceManager_.reset();

        // Fullscreen state should always be false before exiting the app.
        swapChain_.SetFullscreenState(false);
    }

    void Renderer::SetFullscreen(bool fullscreen)
    {
        swapChain_.SetFullscreenState(fullscreen);
    }

    bool Renderer::IsFullscreen()
    {
        return swapChain_.GetFullscreenState();
    }

    void Renderer::Resize(UINT width, UINT height)
    {
        // Flush before changing any resources.
        commandQueue_.Flush();

        // Resize width/height dependent resources
        swapChain_.Resize(device_, renderTargetDescriptorHeap_, width, height);
        depthStencilBuffer_.Resize(device_, depthStencilDescriptorHeap_, width, height);

        viewport_.Resize(width, height);

        // Transition the resource from its initial state to be used as a depth buffer.
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            depthStencilBuffer_.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);

        CommandList commandList(device_);
        commandList.Get()->ResourceBarrier(1, &barrier);
        commandList.Close();

        std::array<ID3D12CommandList*, 1> commandLists{ { commandList.Get() } };
        commandQueue_.Get()->ExecuteCommandLists(1, commandLists.data());
        commandQueue_.Flush();
    }

    void Renderer::BeginRender()
    {
        commandList_.Reset();
        auto* list = commandList_.Get();

        // Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
        list->RSSetViewports(1, &viewport_.GetViewport());
        list->RSSetScissorRects(1, &viewport_.GetScissorRect());

        // Indicate that the back buffer will be used as a render target.
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            swapChain_.GetCurrentBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
        list->ResourceBarrier(1, &barrier);

        // Clear the back buffer and depth buffer.
        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = renderTargetDescriptorHeap_.GetCPUHandle(swapChain_.GetCurrentBufferIndex());
        list->ClearRenderTargetView(rtvHandle, renderTargetClearColor_, 0, nullptr);

        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = depthStencilDescriptorHeap_.GetCPUHandle();
        list->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

        // Specify the buffers we are going to render to.
        list->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);

        // After a descriptor heap is set on a command list, subsequent calls that define descriptor
        // tables refer to the current descriptor heap.  Only one descriptor heap of each type can be
        // set at one time, which means a maximum of 2 heaps (one sampler, one CBV / SRV / UAV) can be
        // set at one time.  A descriptor table isn't an allocation of memory; it's simply an offset
        // and length into a descriptor heap.
        ID3D12DescriptorHeap* ppHeaps[] = { comboDescriptorManager_->Get() };
        list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

        // Set root signature.
        list->SetGraphicsRootSignature(rootSignature_.Get());

        // Set the root parameter to the frame constant buffer's one and only heap descriptor.
        D3D12_GPU_DESCRIPTOR_HANDLE frameDescriptor = comboDescriptorManager_->GetGPUHandle(frame_->GetFrameConstant().descriptorIndex);
        list->SetGraphicsRootDescriptorTable(RootParameters::FrameConstantIndex, frameDescriptor);
    }

    void Renderer::RenderScene()
    {
        const auto& camera = instanceManager_->GetInstance(frame_->GetCamera());
        const auto viewMatrix = this->GetViewMatrix(camera);
        const auto projMatrix = this->GetProjectionMatrix(camera);        
        const auto viewProjMatrix = viewMatrix * projMatrix;

        // Set per frame constants.
        FrameConstant frameConstant;
        XMStoreFloat4x4(&frameConstant.View, XMMatrixTranspose(viewMatrix));
        XMStoreFloat4x4(&frameConstant.ViewProj, XMMatrixTranspose(viewProjMatrix));
        frameConstant.LightAmbient = frame_->GetAmbientLightColor();
        constantManager_->Load(frame_->GetFrameConstant(), frameConstant);

        // Set material constants.
        auto& materialResources = resourceManager_->GetMaterials();
        for (const auto& materialResource : materialResources)
        {
            MaterialConstant materialConstant;
            materialConstant.baseColor = materialResource.GetBaseColor();
            constantManager_->Load(materialResource.GetConstant(), materialConstant);
        }

        auto* list = commandList_.Get();

        RenderLightedSolids(list);

        RenderUnlightedSolids(list);
    }

    void Renderer::RenderLightedSolids(ID3D12GraphicsCommandList* list)
    {
        // Render ambient lighting.
        list->SetPipelineState(pipelineStates_.GetAmbientState());

        RenderSolids(list);

        // Render point lighting.
        list->SetPipelineState(pipelineStates_.GetLightedState());

        for (const auto& frameLight : frame_->GetLights())
        {
            const auto& lightInstance = instanceManager_->GetInstance(frameLight);
            const auto& lightResource = resourceManager_->GetResource(lightInstance.GetResource());

            // Set per light pass constant.
            LightConstant lightConstant;
            XMVECTOR lightPos{ 0.0f, 0.0f, 0.0f };
            XMStoreFloat3(&lightConstant.LightPosW, XMVector3Transform(lightPos, lightInstance.GetWorldMatrix()));
            lightConstant.LightDiffuse = lightResource.GetDiffuse();
            constantManager_->Load(lightInstance.GetConstant(), lightConstant);

            // Set root parameter 1 to point to the per pass constant buffer descriptor.
            D3D12_GPU_DESCRIPTOR_HANDLE passDescriptor = comboDescriptorManager_->GetGPUHandle(lightInstance.GetConstant().descriptorIndex);
            list->SetGraphicsRootDescriptorTable(RootParameters::PassConstantIndex, passDescriptor);

            RenderSolids(list);
        }
    }

    void Renderer::RenderSolids(ID3D12GraphicsCommandList* list)
    {
        const auto& frameSolids = frame_->GetSolids(MeshKind::Lighted);
        for (const auto& frameSolid : frameSolids)
        {
            const auto& solidInstance = instanceManager_->GetInstance(frameSolid);
            const auto& solidResource = resourceManager_->GetResource(solidInstance.GetResource());
            const auto& meshResource = resourceManager_->GetResource(solidResource.GetMesh());

            // TODO:  Any performance hit to setting the constant buffer data during rendering?
            // The constants themselves won't get read until the the command queue is executed.
            SolidConstant solidConstant;
            XMStoreFloat4x4(&solidConstant.World, XMMatrixTranspose(solidInstance.GetWorldMatrix()));
            constantManager_->Load(solidInstance.GetConstant(), solidConstant);

            const auto& mesh = meshResource.GetMesh();

            list->IASetVertexBuffers(0, 1, &mesh.GetVertexBufferView());
            list->IASetIndexBuffer(&mesh.GetIndexBufferView());
            list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

            // Set root parameter to point to the solid's constant buffer descriptor.
            D3D12_GPU_DESCRIPTOR_HANDLE solidDescriptor = comboDescriptorManager_->GetGPUHandle(solidInstance.GetConstant().descriptorIndex);
            list->SetGraphicsRootDescriptorTable(RootParameters::SolidConstantIndex, solidDescriptor);

            const auto spansSize = meshResource.GetSpans().size();
            for (size_t index = 0; index < spansSize; ++index)
            {
                auto materialHandle = solidResource.GetMaterials()[index];
                const auto& materialResource = resourceManager_->GetResource(materialHandle);
                const auto& textureResource = resourceManager_->GetResource(materialResource.GetTexture());

                // Set root parameter to point to the materials_'s constant buffer descriptor.
                D3D12_GPU_DESCRIPTOR_HANDLE materialDescriptor = comboDescriptorManager_->GetGPUHandle(materialResource.GetConstant().descriptorIndex);
                list->SetGraphicsRootDescriptorTable(RootParameters::MaterialConstantIndex, materialDescriptor);

                // Set root parameter to point to the materials's texture descriptor.
                D3D12_GPU_DESCRIPTOR_HANDLE textureDescriptor = comboDescriptorManager_->GetGPUHandle(textureResource.GetDescriptorIndex());
                list->SetGraphicsRootDescriptorTable(RootParameters::TextureIndex, textureDescriptor);

                list->DrawIndexedInstanced(meshResource.GetSpans()[index].second, 1, meshResource.GetSpans()[index].first, 0, 0);
            }
        }
    }

    void Renderer::RenderUnlightedSolids(ID3D12GraphicsCommandList* list)
    {
        // Render ambient lighting.
        list->SetPipelineState(pipelineStates_.GetUnlightedState());

        const auto& frameSolids = frame_->GetSolids(MeshKind::Unlighted);
        for (const auto& frameSolid : frameSolids)
        {
            const auto& solidInstance = instanceManager_->GetInstance(frameSolid);
            const auto& solidResource = resourceManager_->GetResource(solidInstance.GetResource());
            const auto& meshResource = resourceManager_->GetResource(solidResource.GetMesh());

            // TODO:  Any performance hit to setting the constant buffer data during rendering?
            // The constants themselves won't get read until the the command queue is executed.
            SolidConstant solidConstant;
            XMStoreFloat4x4(&solidConstant.World, XMMatrixTranspose(solidInstance.GetWorldMatrix()));
            constantManager_->Load(solidInstance.GetConstant(), solidConstant);

            const auto& mesh = meshResource.GetMesh();

            list->IASetVertexBuffers(0, 1, &mesh.GetVertexBufferView());
            list->IASetIndexBuffer(&mesh.GetIndexBufferView());
            list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

            // Set root parameter to point to the solid's constant buffer descriptor.
            D3D12_GPU_DESCRIPTOR_HANDLE solidDescriptor = comboDescriptorManager_->GetGPUHandle(solidInstance.GetConstant().descriptorIndex);
            list->SetGraphicsRootDescriptorTable(RootParameters::SolidConstantIndex, solidDescriptor);
         
            list->DrawIndexedInstanced(mesh.GetIndexCount(), 1U, 0U, 0U, 0U);
        }
    }

    void Renderer::EndRender()
    {
        auto* list = commandList_.Get();

        // Indicate a state transition on the resource usage.
        CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
            swapChain_.GetCurrentBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
        list->ResourceBarrier(1, &barrier);

        // Done recording commands.
        ThrowIfFailed(list->Close());

        // Add the command list to the queue for execution.
        ID3D12CommandList* commandLists[] = { list };
        commandQueue_.Get()->ExecuteCommandLists(_countof(commandLists), commandLists);

        swapChain_.Present();

        commandQueue_.Flush();
    }

    void Renderer::InitScene(const ResourceLimits& limits)
    {        
        ConstantLimits constantLimits;
        constantLimits.frames = 1;
        constantLimits.materials = limits.MaxMaterials;
        constantLimits.solids = limits.MaxSolids;
        constantLimits.lights = limits.MaxLights;

        // There are descriptors for five types of objects.
        const UINT descriptorCapacity = static_cast<UINT>(constantLimits.GetTotal() + limits.MaxTextures);
        comboDescriptorManager_ = std::make_unique<ComboDescriptorManager>(device_, descriptorCapacity);
        
        constantManager_ = std::make_unique<ConstantManager>(device_, *comboDescriptorManager_, constantLimits);
        resourceManager_ = std::make_unique<ResourceManager>(*comboDescriptorManager_, *constantManager_);
        instanceManager_ = std::make_unique<InstanceManager>(*constantManager_);
        
        frame_ = std::make_unique<Frame>(*resourceManager_, *instanceManager_);
        frame_->SetFrameConstant(this->constantManager_->CreateFrameConstant());
    }

}
	
