#pragma once
#include "pch.h"

#include "game/game.h"
#include "game/input_context.h"
#include <motheye/platform/window/win32/window.h>

#include <memory>

namespace app
{
	using namespace motheye::platform::window::win32;
	using namespace motheye::platform::input;

	class App
	{
	public:

		App(HINSTANCE hInstance);

		int Run(int nCmdShow);

	private:
		std::unique_ptr<Window> CreateMainWindow();
		void OnInputContextChanged(game::InputContext context);

	private:
		HINSTANCE hInstance_;

		std::unique_ptr<Window> window_;
		game::Game game_;
	};

	inline App::App(HINSTANCE hInstance) :
		hInstance_(hInstance)
	{
	}

}