#pragma once
#include "framework.h"
#include <motheye/dx12/dx12.h>
#include <cassert>

namespace motheye::renderer
{
	using motheye::dx12::RootSignatureDescription;

	class RootParameters
	{
	public:
		
		static constexpr UINT TextureIndex = 0;
		static constexpr UINT SolidConstantIndex = 1;
		static constexpr UINT PassConstantIndex = 2;
		static constexpr UINT MaterialConstantIndex = 3;
		static constexpr UINT FrameConstantIndex = 4;

		static RootSignatureDescription MakeSignatureDescription();
	};
	
	inline RootSignatureDescription RootParameters::MakeSignatureDescription()
	{
		RootSignatureDescription description;

		description.PushSRV(0, D3D12_SHADER_VISIBILITY_PIXEL);
		description.PushCBV(0, D3D12_SHADER_VISIBILITY_ALL);
		description.PushCBV(1, D3D12_SHADER_VISIBILITY_ALL);
		description.PushCBV(2, D3D12_SHADER_VISIBILITY_ALL);
		description.PushCBV(3, D3D12_SHADER_VISIBILITY_ALL);

		description.PushSampler(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR, D3D12_TEXTURE_ADDRESS_MODE_WRAP);
		description.SetFlags(
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

		return description;
	}
}
