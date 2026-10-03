#pragma once
#include "action_frame.h"
#include <motheye/engine/world/entity.h>
#include <motheye/engine/world/tick_context.h>
#include <motheye/engine/world/transform.h>

namespace game
{
	using motheye::engine::world::Entity;
	using motheye::engine::world::TickContext;
	using motheye::engine::world::Transform;

	class FPSCamera : public Entity
	{
	public:
		FPSCamera(const std::string& name, motheye::model::EntityKind kind, const std::string& classname);

		void Initialize() override;
		void Tick(const TickContext& context) override;

	private:
		void Rotate(float lookYaw, float lookPitch);
		void Move(float forward, float strafe, float vertical, float deltaSeconds);
		void UpdateBasisVectors();

	private:
		static constexpr float moveSpeed_{ 200.0f };

		DirectX::XMVECTOR forward_{ DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) };
		DirectX::XMVECTOR right_{ DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) };
		bool basisDirty_{ true };
	};

	inline FPSCamera::FPSCamera(const std::string& name, motheye::model::EntityKind kind, const std::string& classname) :
		Entity(name, kind, classname)
	{
	}
}