#pragma once
#include "resource_handle.h"
#include <motheye/dx12/dx12.h>
#include <memory>

namespace motheye::renderer
{
	class TextureResource
	{
	public:

		TextureResource() = default;
		TextureResource(std::unique_ptr<motheye::dx12::Texture>&& texture, int descriptorIndex);

		int GetDescriptorIndex() const;

	private:

		std::unique_ptr<motheye::dx12::Texture> texture_;
		int descriptorIndex_{ 0 };
	};

	inline TextureResource::TextureResource(std::unique_ptr<motheye::dx12::Texture>&& texture, int descriptorIndex) :
		texture_(std::move(texture)),
		descriptorIndex_(descriptorIndex)
	{
	}

	inline int TextureResource::GetDescriptorIndex() const
	{
		return this->descriptorIndex_;
	}

}