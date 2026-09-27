#pragma once
#include "constant_buffer.h"
#include "combo_descriptor_manager.h"

#include <motheye/dx12/dx12.h>

#include <memory>

namespace renderer
{
	struct FrameConstant
	{
		DirectX::XMFLOAT4X4 View;
		DirectX::XMFLOAT4X4 ViewProj;
		DirectX::XMFLOAT4 LightAmbient;
		float padding[28]; // Padding so the constant buffer is 256-byte aligned.
	};
	static_assert((sizeof(FrameConstant) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

	struct LightConstant
	{
		DirectX::XMFLOAT4 LightDiffuse;
		DirectX::XMFLOAT3 LightPosW;
		float padding[57]; // Padding so the constant buffer is 256-byte aligned.
	};
	static_assert((sizeof(LightConstant) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

	struct MaterialConstant
	{
		DirectX::XMFLOAT4 baseColor;
		float padding[60]; // Padding so the constant buffer is 256-byte aligned.
	};
	static_assert((sizeof(MaterialConstant) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

	struct SolidConstant
	{
		DirectX::XMFLOAT4X4 World;
		float padding[48]; // Padding so the constant buffer is 256-byte aligned.
	};
	static_assert((sizeof(SolidConstant) % 256) == 0, "Constant Buffer size must be 256-byte aligned");

	using FrameConstantBuffer = ConstantBuffer<FrameConstant>;
	using LightConstantBuffer = ConstantBuffer<LightConstant>;
	using SolidConstantBuffer = ConstantBuffer<SolidConstant>;
	using MaterialConstantBuffer = ConstantBuffer<MaterialConstant>;

	struct ConstantLimits
	{
		size_t frames{ 1 };
		size_t lights{ 1 };
		size_t solids{ 1 };
		size_t materials{ 1 };

		size_t GetTotal() const
		{
			return frames + lights + solids + materials;
		}
	};

	class ConstantManager
	{
	public:

		ConstantManager(motheye::dx12::Device& device, ComboDescriptorManager& descriptorHeap, const ConstantLimits& limits);

		Constant CreateFrameConstant();
		Constant CreateMaterialResourceConstant();
		Constant CreateSolidInstanceConstant();
		Constant CreateLightInstanceConstant();

		void Load(const Constant constant, const FrameConstant& value);
		void Load(const Constant constant, const MaterialConstant& value);
		void Load(const Constant constant, const SolidConstant& value);
		void Load(const Constant constant, const LightConstant& value);

	private:

		motheye::dx12::Device& device_;
		ComboDescriptorManager& descriptorHeap_;

		std::unique_ptr<FrameConstantBuffer> frameConstantBuffer_;
		std::unique_ptr<LightConstantBuffer> lightConstantBuffer_;
		std::unique_ptr<SolidConstantBuffer> solidConstantBuffer_;
		std::unique_ptr<MaterialConstantBuffer> materialConstantBuffer_;
	};

	inline ConstantManager::ConstantManager(motheye::dx12::Device& device, ComboDescriptorManager& descriptorHeap, const ConstantLimits& limits) :
		device_(device),
		descriptorHeap_(descriptorHeap)
	{
		frameConstantBuffer_ = std::make_unique<ConstantBuffer<FrameConstant>>(device, static_cast<UINT>(limits.frames));
		lightConstantBuffer_ = std::make_unique<ConstantBuffer<LightConstant>>(device, static_cast<UINT>(limits.lights));
		materialConstantBuffer_ = std::make_unique<ConstantBuffer<MaterialConstant>>(device, static_cast<UINT>(limits.materials));
		solidConstantBuffer_ = std::make_unique<ConstantBuffer<SolidConstant>>(device, static_cast<UINT>(limits.solids));
	}

	inline Constant ConstantManager::CreateFrameConstant()
	{
		// Assume that the frame constant will be at descriptorIndex zero.
		// TODO:  Why?
		return frameConstantBuffer_->CreateConstant(device_, descriptorHeap_);
	}

	inline Constant ConstantManager::CreateMaterialResourceConstant()
	{
		return materialConstantBuffer_->CreateConstant(device_, descriptorHeap_);
	}

	inline Constant ConstantManager::CreateSolidInstanceConstant()
	{
		return solidConstantBuffer_->CreateConstant(device_, descriptorHeap_);
	}

	inline Constant ConstantManager::CreateLightInstanceConstant()
	{
		return lightConstantBuffer_->CreateConstant(device_, descriptorHeap_);
	}

	inline void ConstantManager::Load(const Constant constant, const FrameConstant& value)
	{
		frameConstantBuffer_->Load(constant.bufferIndex, value);
	}

	inline void ConstantManager::Load(const Constant constant, const MaterialConstant& value)
	{
		materialConstantBuffer_->Load(constant.bufferIndex, value);
	}

	inline void ConstantManager::Load(const Constant constant, const SolidConstant& value)
	{
		solidConstantBuffer_->Load(constant.bufferIndex, value);
	}

	inline void ConstantManager::Load(const Constant constant, const LightConstant& value)
	{
		lightConstantBuffer_->Load(constant.bufferIndex, value);
	}
}