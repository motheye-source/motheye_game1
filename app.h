#pragma once
#include "pch.h"

#include "engine/engine.h"
#include <motheye/platform/window/win32/window.h>
#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/controller_event.h>

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

		void OnSize(unsigned int, unsigned int);
		void OnFocusLost();
		void OnLegacyCaptureLost();


	private:
		std::unique_ptr<Window<App, app::engine::Engine>> CreateMainWindow();

	private:
		HINSTANCE hInstance_;

		std::unique_ptr<Window<App, app::engine::Engine>> window_;
		app::engine::Engine engine_;
	};

	inline App::App(HINSTANCE hInstance) :
		hInstance_(hInstance)
	{
	}

}