#include "pch.h"
#include "pipeline_states.h"

#include <motheye/dx12/shader_factory.h>

using namespace motheye::dx12;

namespace renderer
{
    void PipelineStates::Initialize(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat)
    {
        CreateAmbientState(device, rootSignature, dsvFormat);

        CreateLightedState(device, rootSignature, dsvFormat);

        CreateUnlightedState(device, rootSignature, dsvFormat);
    }

    void PipelineStates::CreateAmbientState(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat)
    {
        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        Shader vertexShader = ShaderFactory::CompileVertexShader("shaders/ambient.hlsl", "VS_Ambient");
        Shader pixelShader = ShaderFactory::CompilePixelShader("shaders/ambient.hlsl", "PS_Ambient");

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = rootSignature;
        psoDesc.VS = vertexShader.GetByteCode();
        psoDesc.PS = pixelShader.GetByteCode();
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.DSVFormat = dsvFormat;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

        ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&ambientState_)));
    }


    void PipelineStates::CreateLightedState(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat)
    {
        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        Shader vertexShader = ShaderFactory::CompileVertexShader("shaders/lighted.hlsl", "VS_Lighted");
        Shader pixelShader = ShaderFactory::CompilePixelShader("shaders/lighted.hlsl", "PS_Lighted");

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = rootSignature;
        psoDesc.VS = vertexShader.GetByteCode();
        psoDesc.PS = pixelShader.GetByteCode();
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
        psoDesc.DSVFormat = dsvFormat;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;

        // Disable depth buffer writes.
        psoDesc.DepthStencilState.DepthEnable = TRUE;
        psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
        psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        psoDesc.DepthStencilState.StencilEnable = FALSE;

        // Blend by adding new source pixels with what's currently in the render target.
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        psoDesc.BlendState.AlphaToCoverageEnable = FALSE;
        psoDesc.BlendState.IndependentBlendEnable = FALSE;
        psoDesc.BlendState.RenderTarget->LogicOpEnable = FALSE;
        psoDesc.BlendState.RenderTarget->BlendEnable = TRUE;
        psoDesc.BlendState.RenderTarget->SrcBlend = D3D12_BLEND_ONE;
        psoDesc.BlendState.RenderTarget->DestBlend = D3D12_BLEND_ONE;
        psoDesc.BlendState.RenderTarget->BlendOp = D3D12_BLEND_OP_ADD;
        psoDesc.BlendState.RenderTarget->SrcBlendAlpha = D3D12_BLEND_ONE;
        psoDesc.BlendState.RenderTarget->DestBlendAlpha = D3D12_BLEND_ONE;
        psoDesc.BlendState.RenderTarget->BlendOpAlpha = D3D12_BLEND_OP_ADD;
        psoDesc.BlendState.RenderTarget->RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&lightedState_)));
    }

    void PipelineStates::CreateUnlightedState(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat)
    {
        // Define the vertex input layout.
        D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        Shader vertexShader = ShaderFactory::CompileVertexShader("shaders/unlighted.hlsl", "VS_Unlighted");
        Shader pixelShader = ShaderFactory::CompilePixelShader("shaders/unlighted.hlsl", "PS_Unlighted");

        // Describe and create the graphics pipeline state object (PSO).
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
        psoDesc.pRootSignature = rootSignature;
        psoDesc.VS = vertexShader.GetByteCode();
        psoDesc.PS = pixelShader.GetByteCode();
        psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        psoDesc.DSVFormat = dsvFormat;
        psoDesc.SampleMask = UINT_MAX;
        psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        psoDesc.NumRenderTargets = 1;
        psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        psoDesc.SampleDesc.Count = 1;
        psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

        ThrowIfFailed(device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&unlightedState_)));
    }
}