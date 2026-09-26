#pragma once

#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/controller_event.h>

#include "clock_source.h"

namespace app::engine
{
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::ControllerEvent;

	class Engine
	{
	public:

		void OnBeginFrame();
		void OnUpdate();
		void OnMouseKeyboardEvent(const MouseKeyboardEvent& event);
		void OnControllerEvent(const ControllerEvent& event);
		
	private:
		ClockSource clock_;
	};
	
}
