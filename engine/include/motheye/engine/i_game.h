#pragma once
#include <motheye/engine/input/frame_input.h>
#include <motheye/engine/world/i_game_context.h>

namespace motheye::engine
{
	using motheye::engine::input::FrameInput;
	using motheye::engine::world::IGameContext;

	struct IGame
	{
		virtual const IGameContext* ResolveFrameInput(const FrameInput& input, double deltaSeconds) = 0;
	};
}