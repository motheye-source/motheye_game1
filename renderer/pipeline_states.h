#pragma once
#include "framework.h"

namespace motheye::renderer
{
	class PipelineStates
	{
	public:

		void Initialize(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat);

		ID3D12PipelineState* GetAmbientState();
		ID3D12PipelineState* GetLightedState();
		ID3D12PipelineState* GetUnlightedState();

	private:

		Microsoft::WRL::ComPtr<ID3D12PipelineState> ambientState_;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> lightedState_;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> unlightedState_;

		void CreateAmbientState(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat);
		void CreateLightedState(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat);
		void CreateUnlightedState(ID3D12Device8* device, ID3D12RootSignature* rootSignature, DXGI_FORMAT dsvFormat);
	};

	inline ID3D12PipelineState* PipelineStates::GetAmbientState()
	{
		return ambientState_.Get();
	}

	inline ID3D12PipelineState* PipelineStates::GetLightedState()
	{
		return lightedState_.Get();
	}

	inline ID3D12PipelineState* PipelineStates::GetUnlightedState()
	{
		return unlightedState_.Get();
	}
}

