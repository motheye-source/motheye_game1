#pragma once
#include "i_game_context.h"

namespace motheye::engine::world
{
	struct TickContext
	{
		float deltaSeconds;
		const IGameContext* gameContext;
	};
}