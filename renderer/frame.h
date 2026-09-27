#pragma once
#include "framework.h"
#include "resource_handle.h"
#include "instance_handle.h"
#include "constant_buffer.h"
#include "mesh_resource.h"
#include "resource_manager.h"
#include "instance_manager.h"

#include <array>
#include <vector>

namespace motheye::renderer
{
	class Frame
	{
	public:

		Frame(ResourceManager& resourceManager, InstanceManager& instanceManager);

		const Constant& GetFrameConstant() const;		
		void SetFrameConstant(const Constant& constant);

		const DirectX::XMFLOAT4& GetAmbientLightColor() const;

		void SetAmbientLightColor(const DirectX::XMFLOAT4& ambient);
		
		const std::vector<LightInstanceHandle>& GetLights() const;
		const std::vector<SolidInstanceHandle>& GetSolids(MeshKind kind) const;

		CameraInstanceHandle GetCamera() const;
		void SetCamera(CameraInstanceHandle camera);

		void Push(LightInstanceHandle light);
		void Push(SolidInstanceHandle solid);

	private:

		ResourceManager& resourceManager_;
		InstanceManager& instanceManager_;

		DirectX::XMMATRIX viewMatrix_ = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX projMatrix_ = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT4 ambientLightColor_{ 0.1f, 0.1f, 0.1f, 1.0f };

		std::vector<LightInstanceHandle> lights_;
		std::array<std::vector<SolidInstanceHandle>, renderer::kMeshKindSize> solids_;
		
		CameraInstanceHandle camera_{ InstanceHandle::InvalidValue };

		Constant constant_{};
	};

	inline Frame::Frame(ResourceManager& resourceManager, InstanceManager& instanceManager) :
		resourceManager_(resourceManager), instanceManager_(instanceManager)
	{
	}

	inline const Constant& Frame::GetFrameConstant() const
	{
		return constant_;
	}

	inline void Frame::SetFrameConstant(const Constant& constant)
	{
		constant_ = constant;
	}

	inline const DirectX::XMFLOAT4& Frame::GetAmbientLightColor() const
	{
		return ambientLightColor_;
	}

	inline void Frame::SetAmbientLightColor(const DirectX::XMFLOAT4& ambient)
	{
		ambientLightColor_ = ambient;
	}

	inline const std::vector<LightInstanceHandle>& Frame::GetLights() const
	{
		return lights_;
	}

	inline const std::vector<SolidInstanceHandle>& Frame::GetSolids(MeshKind kind) const
	{
		return solids_[static_cast<size_t>(kind)];
	}

	inline void Frame::Push(LightInstanceHandle light)
	{
		lights_.push_back(light);
	}
	
	inline void Frame::Push(SolidInstanceHandle solid)
	{
		auto instance = instanceManager_.GetInstance(solid);
		auto resource = resourceManager_.GetResource(instance.GetResource());
		auto kind = resourceManager_.GetResource(resource.GetMesh()).GetKind();
		solids_[static_cast<size_t>(kind)].push_back(solid);
	}

	inline CameraInstanceHandle Frame::GetCamera() const
	{
		return this->camera_;
	}

	inline void Frame::SetCamera(CameraInstanceHandle camera)
	{
		this->camera_ = camera;
	}		
}