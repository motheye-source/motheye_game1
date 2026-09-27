#pragma once
#include "framework.h"

namespace renderer
{
	class CameraResource
	{
	public:

		CameraResource() = default;
		CameraResource(float fov, float nearClip, float farClip);

		float GetFov() const;
		float GetNearClip() const;
		float GetFarClip() const;

	private:

		float fov_;
		float nearClip_;
		float farClip_;
	};

	inline CameraResource::CameraResource(float fov, float nearClip, float farClip) :
		fov_(fov),
		nearClip_(nearClip),
		farClip_(farClip)
	{
	}
	
	inline float CameraResource::GetFov() const
	{
		return this->fov_;
	}

	inline float CameraResource::GetNearClip() const
	{
		return this->nearClip_;
	}

	inline float CameraResource::GetFarClip() const
	{
		return this->farClip_;
	}
}