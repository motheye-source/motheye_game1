#pragma once
#include <motheye/engine/input/frame_input.h>
#include <motheye/engine/i_engine.h>
#include <motheye/model/model.h>

#include "action_resolver.h"

namespace game
{
	using namespace motheye::engine::input;
	using motheye::model::Model;

	class Game
	{
	public:

		Game(motheye::engine::IEngine& engine);

		void Start();
		void Stop();

	private:
		void LoadWorld();
		void LoadDefaultCamera(Model& data);

	private:
		motheye::engine::IEngine& engine_;
		ActionResolver actionResolver_;
	};

	inline Game::Game(motheye::engine::IEngine& engine) :
		engine_(engine)		 
	{
	}
}
