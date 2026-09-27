#pragma once
#include "instance_handle.h"
#include "camera_instance.h"
#include "light_instance.h"
#include "solid_instance.h"
#include "constant_manager.h"

#include <vector>
#include <type_traits>

namespace renderer
{
	class InstanceManager
	{
	public:

		InstanceManager(ConstantManager& constantManager);

		template<typename THandle>
		auto& GetInstance(THandle handle);

		template<typename TResourceHandle>
		inline auto CreateInstance(TResourceHandle resource);

		template<typename THandle>
		void SetWorldMatrix(THandle handle, const DirectX::XMMATRIX& matrix);

	private:

		ConstantManager& constantManager_;

		std::vector<CameraInstance> cameras_;
		std::vector<LightInstance> lights_;
		std::vector<SolidInstance> solids_;
	};

	inline InstanceManager::InstanceManager(ConstantManager& constantManager) :
		constantManager_(constantManager)
	{
	}

	template<typename THandle>
	inline auto& InstanceManager::GetInstance(THandle handle)
	{
		if constexpr (std::is_same_v<THandle, CameraInstanceHandle>)
		{
			return cameras_[handle.GetValue()];
		}
		else if constexpr (std::is_same_v<THandle, LightInstanceHandle>)
		{
			return lights_[handle.GetValue()];
		}
		else if constexpr (std::is_same_v<THandle, SolidInstanceHandle>)
		{
			return solids_[handle.GetValue()];
		}
		else
		{
			static_assert(sizeof(THandle) == 0, "Unsupported instance handle type for GetInstance");
		}
	}

	template<typename TResourceHandle>
	inline auto InstanceManager::CreateInstance(TResourceHandle resource)
	{
		if constexpr (std::is_same_v<TResourceHandle, CameraResourceHandle>)
		{
			cameras_.emplace_back(resource);
			return CameraInstanceHandle(cameras_.size() - 1);
		}
		else if constexpr (std::is_same_v<TResourceHandle, LightResourceHandle>)
		{
			Constant constant = constantManager_.CreateLightInstanceConstant();
			lights_.emplace_back(resource, constant);
			return LightInstanceHandle(lights_.size() - 1);
		}
		else if constexpr (std::is_same_v<TResourceHandle, SolidResourceHandle>)
		{
			Constant constant = constantManager_.CreateSolidInstanceConstant();
			solids_.emplace_back(resource, constant);
			return SolidInstanceHandle(solids_.size() - 1);
		}
		else
		{
			static_assert(sizeof(TResourceHandle) == 0, "Unsupported resource handle type for CreateInstance");
		}
	}

	template<typename THandle>
	inline void InstanceManager::SetWorldMatrix(THandle handle, const DirectX::XMMATRIX& matrix)
	{
		if constexpr (std::is_same_v<THandle, CameraInstanceHandle>)
		{
			cameras_[handle.GetValue()].SetWorldMatrix(matrix);
		}
		else if constexpr (std::is_same_v<THandle, LightInstanceHandle>)
		{
			lights_[handle.GetValue()].SetWorldMatrix(matrix);
		}
		else if constexpr (std::is_same_v<THandle, SolidInstanceHandle>)
		{
			solids_[handle.GetValue()].SetWorldMatrix(matrix);
		}
		else
		{
			static_assert(sizeof(THandle) == 0, "Unsupported instance handle type for SetWorldMatrix");
		}
	}

}