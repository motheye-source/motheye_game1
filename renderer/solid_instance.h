#pragma once
#include "instance_base.h"
#include "resource_handle.h"

namespace renderer
{
	class SolidInstance : public InstanceBase
	{
	public:

		SolidInstance(SolidResourceHandle resource, Constant constant);

		SolidResourceHandle GetResource() const;

	private:

		SolidResourceHandle resource_;
	};

	inline SolidInstance::SolidInstance(SolidResourceHandle resource, Constant constant) :
		InstanceBase(constant),
		resource_(resource)
	{
	}

	inline SolidResourceHandle SolidInstance::GetResource() const
	{
		return resource_;
	}
}