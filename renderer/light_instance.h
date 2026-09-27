#pragma once
#include "instance_base.h"
#include "resource_handle.h"

namespace renderer
{
	class LightInstance : public InstanceBase
	{
	public:
		
		LightInstance(LightResourceHandle resource, Constant constant);

		LightResourceHandle GetResource() const;

	private:

		LightResourceHandle resource_;
	};

	inline LightInstance::LightInstance(LightResourceHandle resource, Constant constant) :
		InstanceBase(constant),
		resource_(resource)
	{
	}

	inline LightResourceHandle LightInstance::GetResource() const
	{
		return resource_;
	}
}