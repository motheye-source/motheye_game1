#pragma once
#include <motheye/platform/window/i_frame_handler.h>
#include <motheye/engine/i_engine.h>
#include <motheye/engine/i_game.h>
#include <motheye/engine/input/frame_input.h>
#include <motheye/engine/world/i_game_context.h>
#include <motheye/model/model.h>

#include "engine/src/engine.h"
#include "action_frame.h"
#include "action_resolver.h"
#include "entity_factory.h"

namespace game
{
	using motheye::platform::window::IFrameHandler;
	using motheye::engine::world::IGameContext;
	using motheye::model::Model;

	class Game : public motheye::engine::IGame
	{
	public:

		Game();

		IFrameHandler& GetFrameHandler();

		void Start(HWND hWnd);
		void Stop();

		const IGameContext* ResolveFrameInput(const FrameInput& input, double deltaSeconds) override;

	private:
		void LoadWorld();
		void LoadDefaultCamera(Model& data);

	private:
		motheye::engine::Engine engine_;
		ActionResolver actionResolver_;
	};

	inline Game::Game() :
		engine_(*this)		 
	{
	}

	inline IFrameHandler& Game::GetFrameHandler()
	{
		return engine_;
	}

	inline const IGameContext* Game::ResolveFrameInput(const FrameInput& input, double deltaSeconds)
	{		
		return &actionResolver_.Resolve(input, deltaSeconds);
	}
}
