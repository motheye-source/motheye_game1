#pragma once

#include <motheye/engine/world/entity.h>
#include <motheye/engine/world/tick_context.h>

namespace game
{
	using motheye::engine::world::Entity;
	using motheye::engine::world::TickContext;

	class FPSCamera : public Entity
	{
		void Initialize() override;
		void Tick(const TickContext& context) override;
	};
}