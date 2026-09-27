#pragma once
#include "framework.h"
#include "resource_handle.h"
#include "constant_buffer.h"

namespace motheye::renderer
{
	class MaterialResource
	{
	public:


		MaterialResource() = default;
		MaterialResource(const DirectX::XMFLOAT4& baseColor, TextureResourceHandle texture, Constant constant);

		const DirectX::XMFLOAT4& GetBaseColor() const;
		TextureResourceHandle GetTexture() const;
		Constant GetConstant() const;

	private:

		DirectX::XMFLOAT4 baseColor_{ DirectX::Colors::White };
		TextureResourceHandle texture_;
		Constant constant_{};
	};

	inline MaterialResource::MaterialResource(const DirectX::XMFLOAT4& baseColor, TextureResourceHandle texture, Constant constant) :
		baseColor_(baseColor),
		texture_(texture),
		constant_(constant)
	{
	}

	inline const DirectX::XMFLOAT4& MaterialResource::GetBaseColor() const
	{
		return this->baseColor_;
	}

	inline TextureResourceHandle MaterialResource::GetTexture() const
	{
		return this->texture_;
	}

	inline Constant MaterialResource::GetConstant() const
	{
		return this->constant_;
	}
}