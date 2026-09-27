#pragma once
#include "framework.h"

namespace motheye::renderer
{
	class LightResource
	{
	public:

		LightResource() = default;
		LightResource(DirectX::XMFLOAT4 diffuse);

		const DirectX::XMFLOAT4& GetDiffuse() const;

	private:

		DirectX::XMFLOAT4 diffuse_{ 1.0, 1.0, 1.0, 1.0 };
	};

	inline LightResource::LightResource(DirectX::XMFLOAT4 diffuse) :
		diffuse_(diffuse)
	{
	}

	inline const DirectX::XMFLOAT4& LightResource::GetDiffuse() const
	{
		return this->diffuse_;
	}
}