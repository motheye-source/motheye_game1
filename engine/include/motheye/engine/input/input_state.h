#pragma once
#include "mouse_keyboard_state.h"
#include "controller_state.h"
#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/controller_event.h>
#include "input_device.h"

namespace motheye::engine::input
{
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::ControllerEvent;

	// Owns the mouse/keyboard and controller state, and tracks which device is
	// currently active (mutually exclusive). Pure state: has no knowledge of
	// how the active device's state is resolved into gameplay actions; that is
	// the responsibility of game::ActionResolver, which reads this class each
	// frame via MouseKeyboard()/Controller()/ActiveDevice().
	class InputState
	{
	public:
		// Clears this-frame transitions on both device states; preserves held-state.
		// Call once per Run() iteration before pumping messages for that iteration.
		void BeginFrame();

		// Any mouse/keyboard event activates the mouse/keyboard device, releasing
		// held input for the device switched away from (if any).
		void ApplyMouseKeyboardEvent(const MouseKeyboardEvent& event);

		// Applies a controller event to the controller state and updates active
		// device selection. See ApplyControllerEvent() definition for full rules.
		void ApplyControllerEvent(const ControllerEvent& event);

		// Releases held input for the currently active device (e.g. on focus loss).
		void ReleaseActiveHeldInput();

		const MouseKeyboardState& MouseKeyboard() const;
		const ControllerState& Controller() const;

		InputDevice ActiveDevice() const;

	private:
		void ReleaseHeldInputFor(InputDevice device);

		MouseKeyboardState mouseKeyboardState_;
		ControllerState controllerState_;
		InputDevice activeDevice_{ InputDevice::MouseKeyboard };
	};

	inline void InputState::BeginFrame()
	{
		mouseKeyboardState_.BeginFrame();
		controllerState_.BeginFrame();
	}

	inline void InputState::ApplyMouseKeyboardEvent(const MouseKeyboardEvent& event)
	{
		if (activeDevice_ != InputDevice::MouseKeyboard)
		{
			ReleaseHeldInputFor(activeDevice_);
			activeDevice_ = InputDevice::MouseKeyboard;
		}

		mouseKeyboardState_.Apply(event);
	}

	inline void InputState::ApplyControllerEvent(const ControllerEvent& event)
	{
		if (event.kind == ControllerEvent::Kind::Disconnected)
		{
			if (activeDevice_ == InputDevice::Controller)
			{
				activeDevice_ = InputDevice::MouseKeyboard;
			}
			controllerState_.Apply(event); // clears held state to match reality
			return;
		}

		if (event.kind == ControllerEvent::Kind::StateChanged)
		{
			if (activeDevice_ != InputDevice::Controller)
			{
				if (!ControllerState::ShouldApply(event))
				{
					return;
				}

				// Switch devices if the controller had active input.
				ReleaseHeldInputFor(activeDevice_);
				activeDevice_ = InputDevice::Controller;
			}

			controllerState_.Apply(event);
			return;
		}

		// Kind::Connected is informational only (e.g. for HUD/telemetry); it does
		// not update the state or change the active device, since a controller
		// merely being connected is not itself an indication of player intent.
	}

	inline void InputState::ReleaseActiveHeldInput()
	{
		ReleaseHeldInputFor(activeDevice_);
	}

	inline const MouseKeyboardState& InputState::MouseKeyboard() const
	{
		return mouseKeyboardState_;
	}

	inline const ControllerState& InputState::Controller() const
	{
		return controllerState_;
	}

	inline InputDevice InputState::ActiveDevice() const
	{
		return activeDevice_;
	}

	inline void InputState::ReleaseHeldInputFor(InputDevice device)
	{
		if (device == InputDevice::MouseKeyboard)
		{
			mouseKeyboardState_.ReleaseAllHeld();
		}
		else if (device == InputDevice::Controller)
		{
			controllerState_.ReleaseAllHeld();
		}
	}
}
