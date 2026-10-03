#pragma once
#include <motheye/engine/input/input_device.h>
#include <motheye/engine/input/frame_input.h>

#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/controller_event.h>

#include "engine/src/input/controller_handler.h"
#include "engine/src/input/mouse_keyboard_handler.h"

namespace motheye::engine::input
{
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::ControllerEvent;

	class InputHandler
	{
	public:
		InputHandler();

		const FrameInput& GetFrameInput() const;

		void BeginFrame();
		void ApplyMouseKeyboardEvent(const MouseKeyboardEvent& event);
		void ApplyControllerEvent(const ControllerEvent& event);

		// Releases held input for the currently active device (e.g. on focus loss).
		void ReleaseActiveHeldInput();

		InputDevice ActiveDevice() const;
		const MouseKeyboardHandler& MouseKeyboard() const;
		const ControllerHandler& Controller() const;

	private:
		void ReleaseHeldInputFor(InputDevice device);

		InputDevice activeDevice_{ InputDevice::MouseKeyboard };
		MouseKeyboardHandler mouseKeyboardHandler_;
		ControllerHandler controllerHandler_;

		FrameInput frameInput_;
	};

	inline InputHandler::InputHandler() :
		frameInput_(activeDevice_, mouseKeyboardHandler_.GetState(), controllerHandler_.GetState())
	{
	}

	inline const FrameInput& InputHandler::GetFrameInput() const
	{
		return frameInput_;
	}

	inline void InputHandler::BeginFrame()
	{
		mouseKeyboardHandler_.BeginFrame();
		controllerHandler_.BeginFrame();
	}	

	inline void InputHandler::ReleaseActiveHeldInput()
	{
		ReleaseHeldInputFor(activeDevice_);
	}

	inline const MouseKeyboardHandler& InputHandler::MouseKeyboard() const
	{
		return mouseKeyboardHandler_;
	}

	inline const ControllerHandler& InputHandler::Controller() const
	{
		return controllerHandler_;
	}

	inline InputDevice InputHandler::ActiveDevice() const
	{
		return activeDevice_;
	}

	inline void InputHandler::ReleaseHeldInputFor(InputDevice device)
	{
		if (device == InputDevice::MouseKeyboard)
		{
			mouseKeyboardHandler_.ReleaseAllHeld();
		}
		else if (device == InputDevice::Controller)
		{
			controllerHandler_.ReleaseAllHeld();
		}
	}
}
