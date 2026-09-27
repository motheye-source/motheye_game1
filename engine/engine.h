#pragma once

#include <motheye/platform/window/i_frame_handler.h>
#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/controller_event.h>

#include "input/input_state.h"

#include "clock_source.h"

namespace motheye::engine
{
	using motheye::platform::window::IFrameHandler;
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::ControllerEvent;

	class Engine : public IFrameHandler
	{
	public:

		void Start(HWND hWnd);
		void Stop();

		void OnInitFrame() override;
		void OnRunFrame() override;

		void OnMouseKeyboardEvent(const MouseKeyboardEvent& event) override;
		void OnControllerEvent(const ControllerEvent& event) override;

		void OnResize(unsigned int, unsigned int) override;
		void OnReleaseInput() override;
				
	private:
		ClockSource clock_;
		input::InputState inputState_;
	};
	
	inline void Engine::OnInitFrame()
	{
		inputState_.BeginFrame();
	}

	inline void Engine::OnMouseKeyboardEvent(const input::MouseKeyboardEvent& event)
	{
		inputState_.ApplyMouseKeyboardEvent(event);
	}

	inline void Engine::OnControllerEvent(const input::ControllerEvent& event)
	{
		inputState_.ApplyControllerEvent(event);
	}

	inline void Engine::OnResize(unsigned int, unsigned int)
	{
	}

	inline void Engine::OnReleaseInput()
	{
		inputState_.ReleaseActiveHeldInput();

		// TODO: How to notify App so that input mode can be switched to legacy if needed?
	}

}
