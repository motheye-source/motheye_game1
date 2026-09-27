#pragma once
#include "resource_kind.h"
#include <cstdint>

namespace renderer
{
	class ResourceHandle
	{
	public:

		using ValueType = uint16_t;
		
		static constexpr ValueType InvalidValue{ std::numeric_limits<ValueType>::max() };

	public:
		
		ResourceHandle() = default;
		ResourceHandle(ResourceKind kind, ValueType value);

		ResourceKind GetKind() const;		
		ValueType GetValue() const;
		bool IsValid() const;
		
		template<typename T>
		T& As();

		template<typename T>
		const T& As() const;

	private:

		ResourceKind kind_{ ResourceKind::Invalid };
		ValueType value_{ InvalidValue };
	};

	inline ResourceHandle::ResourceHandle(ResourceKind kind, ValueType value) :
		kind_(kind), value_(value)
	{
	}

	inline ResourceKind  ResourceHandle::GetKind() const
	{
		return this->kind_;
	}

	inline ResourceHandle::ValueType ResourceHandle::GetValue() const
	{
		return this->value_;
	}
	
	inline bool ResourceHandle::IsValid() const
	{
		return !((this->kind_ == ResourceKind::Invalid) || (this->value_ == InvalidValue));
	}

	template<typename T>
	inline T& ResourceHandle::As()
	{
		return static_cast<T&>(*this);
	}

	template<typename T>
	inline const T& ResourceHandle::As() const
	{
		return static_cast<const T&>(*this);
	}

	class CameraResourceHandle : public ResourceHandle
	{
	public:
		CameraResourceHandle() = default;
		explicit CameraResourceHandle(ValueType value);
	};

	inline CameraResourceHandle::CameraResourceHandle(ValueType value) :
		ResourceHandle(ResourceKind::Camera, value)
	{
	}

	class LightResourceHandle : public ResourceHandle
	{
	public:
		LightResourceHandle() = default;
		explicit LightResourceHandle(ValueType value);
	};

	inline LightResourceHandle::LightResourceHandle(ValueType value) :
		ResourceHandle(ResourceKind::Light, value)
	{
	}

	class MaterialResourceHandle : public ResourceHandle
	{
	public:
		MaterialResourceHandle() = default;
		explicit MaterialResourceHandle(ValueType value);
	};

	inline MaterialResourceHandle::MaterialResourceHandle(ValueType value) :
		ResourceHandle(ResourceKind::Material, value)
	{
	}

	class MeshResourceHandle : public ResourceHandle
	{
	public:
		MeshResourceHandle() = default;
		explicit MeshResourceHandle(ValueType value);
	};

	inline MeshResourceHandle::MeshResourceHandle(ValueType value) :
		ResourceHandle(ResourceKind::Mesh, value)
	{
	}

	class SolidResourceHandle : public ResourceHandle
	{
	public:
		SolidResourceHandle() = default;
		explicit SolidResourceHandle(ValueType value);
	};

	inline SolidResourceHandle::SolidResourceHandle(ValueType value) :
		ResourceHandle(ResourceKind::Solid, value)
	{
	}

	class TextureResourceHandle : public ResourceHandle
	{
	public:
		TextureResourceHandle() = default;
		explicit TextureResourceHandle(ValueType value);
	};

	inline TextureResourceHandle::TextureResourceHandle(ValueType value) :
		ResourceHandle(ResourceKind::Texture, value)
	{
	}	
}
