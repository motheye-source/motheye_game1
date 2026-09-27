#pragma once
#include "instance_base.h"
#include "resource_handle.h"

namespace motheye::renderer
{
	class CameraInstance : public InstanceBase
	{
	public:

		CameraInstance(CameraResourceHandle resource);

		CameraResourceHandle GetResource() const;

	private:

		CameraResourceHandle resource_;
	};

	inline CameraInstance::CameraInstance(CameraResourceHandle resource) :
		InstanceBase(Constant{}),
		resource_(resource)
	{
	}

	inline CameraResourceHandle CameraInstance::GetResource() const
	{
		return resource_;
	}
}