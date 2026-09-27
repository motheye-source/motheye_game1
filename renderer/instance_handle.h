#pragma once
#include "instance_kind.h"
#include <cstdint>

namespace motheye::renderer
{
	class InstanceHandle
	{
	public:

		using ValueType = uint16_t;

		static constexpr ValueType InvalidValue{ std::numeric_limits<ValueType>::max() };

	public:

		InstanceHandle() = default;
		InstanceHandle(InstanceKind kind, ValueType value);

		InstanceKind GetKind() const;
		ValueType GetValue() const;

		template<typename T>
		T& As();

		template<typename T>
		const T& As() const;

	private:

		InstanceKind kind_{ InstanceKind::Invalid };
		ValueType value_{ InvalidValue };
	};

	inline InstanceHandle::InstanceHandle(InstanceKind kind, ValueType value) :
		kind_(kind), value_(value)
	{
	}

	inline InstanceKind InstanceHandle::GetKind() const
	{
		return this->kind_;
	}

	inline InstanceHandle::ValueType InstanceHandle::GetValue() const
	{
		return this->value_;
	}

	template<typename T>
	inline T& InstanceHandle::As()
	{
		return static_cast<T&>(*this);
	}

	template<typename T>
	inline const T& InstanceHandle::As() const
	{
		return static_cast<const T&>(*this);
	}

	class CameraInstanceHandle : public InstanceHandle
	{
	public:
		explicit CameraInstanceHandle(ValueType value);
	};

	inline CameraInstanceHandle::CameraInstanceHandle(ValueType value) :
		InstanceHandle(InstanceKind::Camera, value)
	{
	}

	class LightInstanceHandle : public InstanceHandle
	{
	public:
		explicit LightInstanceHandle(ValueType value);
	};

	inline LightInstanceHandle::LightInstanceHandle(ValueType value) :
		InstanceHandle(InstanceKind::Light, value)
	{
	}

	class SolidInstanceHandle : public InstanceHandle
	{
	public:
		explicit SolidInstanceHandle(ValueType value);
	};

	inline SolidInstanceHandle::SolidInstanceHandle(ValueType value) :
		InstanceHandle(InstanceKind::Solid, value)
	{
	}
}