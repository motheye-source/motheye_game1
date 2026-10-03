#pragma once
#include <motheye/engine/world/i_game_context.h>
#include "input_context.h"

namespace game
{
	// Resolved, per-frame snapshot of game actions for a given InputContext.
	// Produced by the ActionResolver after binding device state against the
	// current context's binding table.
	struct ActionFrame : public motheye::engine::world::IGameContext
	{
		InputContext context = InputContext::Gameplay;

		// Gameplay context
		float moveX = 0.0f;
		float moveY = 0.0f;
		float moveZ = 0.0f;
		float lookYaw = 0.0f;
		float lookPitch = 0.0f;
		bool openMenuPressed = false;

		// Menu context
		bool backPressed = false;

		// Global (any context)
		bool toggleFullscreenPressed = false;
	};
}