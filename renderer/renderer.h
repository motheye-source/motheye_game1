#pragma once
#include "framework.h"
#include "root_parameters.h"
#include "pipeline_states.h"

#include "resource_limits.h"
#include "resource_manager.h"
#include "constant_manager.h"
#include "combo_descriptor_manager.h"
#include "instance_manager.h"
#include "frame.h"

#include <motheye/dx12/dx12.h>

#include <memory>

namespace renderer
{
	class Renderer
	{
	public:

		Renderer(motheye::dx12::DXGIAdapter& adapter, HWND hWndTarget, D3D_FEATURE_LEVEL minFeatureLevel = D3D_FEATURE_LEVEL_11_0);
		
		void Destroy();

		void Resize(UINT width, UINT height);

		void BeginRender();
		void RenderScene();
		void EndRender();

		void SetRenderTargetClearColor(DirectX::XMVECTORF32 color);
		void SetFullscreen(bool fullscreen);
		bool IsFullscreen();

// ===================================
// TODO:  Move this set of methods and related members below into a separate Model class
// and pass instance to RenderScene()
		void InitScene(const ResourceLimits& limits = ResourceLimits());
		
		template<MeshKind Kind>
		MeshResourceHandle CreateMeshResource(
			const std::vector<typename MeshTraits<Kind>::VertexType>& vertices, const std::vector<MeshIndexType>& indices, std::vector<MeshIndexSpan>&& spans = {});
		TextureResourceHandle CreateTextureResource(const std::string& textureFilePath);
		MaterialResourceHandle CreateMaterialResource(const DirectX::XMFLOAT4& baseColor, TextureResourceHandle texture);

		CameraResourceHandle CreateCameraResource(float fov, float nearClip, float farClip);
		LightResourceHandle CreateLightResource(const DirectX::XMFLOAT4& diffuseColor);
		SolidResourceHandle CreateSolidResource(MeshResourceHandle mesh, std::vector<MaterialResourceHandle>&& materials = {});

		template<typename TResourceHandle>
		auto CreateInstance(TResourceHandle resource);

		template<typename THandle>
		void SetInstanceMatrix(THandle instance, const DirectX::XMMATRIX& matrix);
// ====================================

		Frame& GetFrame();
			
	private:

		void RenderLightedSolids(ID3D12GraphicsCommandList* list);
		void RenderSolids(ID3D12GraphicsCommandList* list);
		void RenderUnlightedSolids(ID3D12GraphicsCommandList* list);

		DirectX::XMMATRIX GetViewMatrix(const CameraInstance& camera) const;
		DirectX::XMMATRIX GetProjectionMatrix(const CameraInstance& camera) const;
		
	private:

		friend class ResourceManager;

		HWND hWndTarget_{ NULL };
				
		motheye::dx12::Device device_;
		motheye::dx12::CommandQueue commandQueue_;
		motheye::dx12::CommandList commandList_;
		motheye::dx12::SwapChain swapChain_;
		motheye::dx12::RenderTargetDescriptorHeap renderTargetDescriptorHeap_;
		motheye::dx12::DepthStencilDescriptorHeap depthStencilDescriptorHeap_;
		motheye::dx12::DepthStencilBuffer depthStencilBuffer_;
		motheye::dx12::RootSignature rootSignature_;
		
		motheye::dx12::Viewport viewport_;
		PipelineStates pipelineStates_;
		
		std::unique_ptr<ComboDescriptorManager> comboDescriptorManager_;
		std::unique_ptr<ConstantManager> constantManager_;
		std::unique_ptr<ResourceManager> resourceManager_;
		std::unique_ptr<InstanceManager> instanceManager_;
		std::unique_ptr<Frame> frame_;

		DirectX::XMVECTORF32 renderTargetClearColor_{ DirectX::Colors::Black };	
	};

	inline Renderer::Renderer(motheye::dx12::DXGIAdapter& adapter, HWND hWndTarget, D3D_FEATURE_LEVEL minFeatureLevel) :
		hWndTarget_(hWndTarget),
		device_(adapter, minFeatureLevel),
		commandQueue_(device_),
		commandList_(device_, true),
		swapChain_(commandQueue_, hWndTarget),
		renderTargetDescriptorHeap_(device_, swapChain_.GetBufferCount()),
		depthStencilDescriptorHeap_(device_, 1),
		rootSignature_(device_, RootParameters::MakeSignatureDescription())
	{	
		// Create/init pipeline state(s).
		pipelineStates_.Initialize(device_.Get(), rootSignature_.Get(), depthStencilBuffer_.GetFormat());

		commandQueue_.Flush();
	}

	template<MeshKind Kind>
	inline MeshResourceHandle Renderer::CreateMeshResource(
		const std::vector<typename MeshTraits<Kind>::VertexType>& vertices, const std::vector<MeshIndexType>& indices, std::vector<MeshIndexSpan>&& spans)
	{
		return resourceManager_->CreateMesh<Kind>(commandQueue_, vertices, indices, std::move(spans));
	}

	inline TextureResourceHandle Renderer::CreateTextureResource(const std::string& textureFilePath)
	{
		return resourceManager_->CreateTexture(commandQueue_, textureFilePath);
	}

	inline MaterialResourceHandle Renderer::CreateMaterialResource(const DirectX::XMFLOAT4& baseColor, TextureResourceHandle texture)
	{
		return resourceManager_->CreateMaterial(baseColor, texture);
	}

	inline SolidResourceHandle Renderer::CreateSolidResource(MeshResourceHandle mesh, std::vector<MaterialResourceHandle>&& materials)
	{
		return resourceManager_->CreateSolid(mesh, std::move(materials));	
	}

	inline CameraResourceHandle Renderer::CreateCameraResource(float fov, float nearClip, float farClip)
	{
		return resourceManager_->CreateCamera(fov, nearClip, farClip);
	}

	inline LightResourceHandle Renderer::CreateLightResource(const DirectX::XMFLOAT4& diffuseColor)
	{
		return resourceManager_->CreateLight(diffuseColor);
	}

	inline void Renderer::SetRenderTargetClearColor(DirectX::XMVECTORF32 color)
	{
		renderTargetClearColor_ = color;
	}

	inline Frame& Renderer::GetFrame()
	{
		return *frame_;
	}

	template<typename TResourceHandle>
	inline auto Renderer::CreateInstance(TResourceHandle resource)
	{
		return instanceManager_->CreateInstance(resource);
	}

	template<typename THandle>
	inline void Renderer::SetInstanceMatrix(THandle instance, const DirectX::XMMATRIX& matrix)
	{
		instanceManager_->SetWorldMatrix(instance, matrix);
	}

	inline DirectX::XMMATRIX Renderer::GetViewMatrix(const CameraInstance& camera) const
	{
		const auto& worldMatrix = camera.GetWorldMatrix();
		DirectX::XMVECTOR forward = DirectX::XMVector3Normalize(worldMatrix.r[1]);
		DirectX::XMVECTOR up = DirectX::XMVector3Normalize(worldMatrix.r[2]);
		return DirectX::XMMatrixLookToRH(worldMatrix.r[3], forward, up);
	}

	inline DirectX::XMMATRIX Renderer::GetProjectionMatrix(const CameraInstance& camera) const
	{
		const auto resource = resourceManager_->GetResource(camera.GetResource());
		return DirectX::XMMatrixPerspectiveFovRH(
			resource.GetFov(), viewport_.GetAspectRatio(), resource.GetNearClip(), resource.GetFarClip());
	}	
}