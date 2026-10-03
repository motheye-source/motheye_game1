#include "fps_camera.h"
#include <motheye/math/math.h>
#include <motheye/engine/world/transform.h>

using motheye::math::DegreesToRadians;

namespace game
{
	void FPSCamera::Initialize()
	{
	}

	void FPSCamera::Tick(const TickContext& context)
	{
		const auto& actions = static_cast<const ActionFrame&>(*context.gameContext);
		
		Rotate(actions.lookYaw, actions.lookPitch);
		Move(actions.moveY, actions.moveX, actions.moveZ, context.deltaSeconds);
	}

	void FPSCamera::Rotate(float lookYaw, float lookPitch)
	{
		auto& transform = this->GetTransform();
		transform.rz -= lookYaw * (180.0f / DirectX::XM_PI);
		transform.rx -= lookPitch * (180.0f / DirectX::XM_PI);
		transform.rx = std::clamp(transform.rx, -89.99f, 89.99f);
		basisDirty_ = true;
	}

	void FPSCamera::UpdateBasisVectors()
	{
		const auto& transform = this->GetTransform();

		const float yawRad = DegreesToRadians(transform.rz);
		const float pitchRad = DegreesToRadians(transform.rx);
		const float cosPitch = std::cos(pitchRad);

		forward_ = DirectX::XMVectorSet(
			std::sin(yawRad) * cosPitch,
			std::cos(yawRad) * cosPitch,
			std::sin(pitchRad),
			0.0f);

		const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		right_ = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(forward_, up));

		basisDirty_ = false;
	}

	void FPSCamera::Move(float forward, float strafe, float vertical, float deltaSeconds)
	{
		if (deltaSeconds <= 0.0f || (forward == 0.0f && strafe == 0.0f && vertical == 0.0f))
		{
			return;
		}

		if (basisDirty_)
		{
			UpdateBasisVectors();
		}

		const DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		const DirectX::XMVECTOR direction = DirectX::XMVectorAdd(
			DirectX::XMVectorAdd(
				DirectX::XMVectorScale(forward_, forward),
				DirectX::XMVectorScale(right_, strafe)),
			DirectX::XMVectorScale(up, vertical));

		const DirectX::XMVECTOR movement = DirectX::XMVectorScale(
			DirectX::XMVector3Normalize(direction), moveSpeed_ * deltaSeconds);

		auto& transform = this->GetTransform();

		const DirectX::XMVECTOR position = DirectX::XMVectorSet(transform.x, transform.y, transform.z, 0.0f);
		DirectX::XMFLOAT3 newPosition;
		DirectX::XMStoreFloat3(&newPosition, DirectX::XMVectorAdd(position, movement));

		transform.x = newPosition.x;
		transform.y = newPosition.y;
		transform.z = newPosition.z;
	}
	

}