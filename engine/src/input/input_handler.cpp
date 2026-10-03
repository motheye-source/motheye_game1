#include "input_handler.h"

namespace motheye::engine::input
{
	void InputHandler::ApplyMouseKeyboardEvent(const MouseKeyboardEvent& event)
	{
		if (activeDevice_ != InputDevice::MouseKeyboard)
		{
			ReleaseHeldInputFor(activeDevice_);
			activeDevice_ = InputDevice::MouseKeyboard;
		}

		mouseKeyboardHandler_.Apply(event);
	}

	void InputHandler::ApplyControllerEvent(const ControllerEvent& event)
	{
		if (event.kind == ControllerEvent::Kind::Disconnected)
		{
			if (activeDevice_ == InputDevice::Controller)
			{
				activeDevice_ = InputDevice::MouseKeyboard;
			}
			controllerHandler_.Apply(event); // clears held state to match reality
			return;
		}

		if (event.kind == ControllerEvent::Kind::StateChanged)
		{
			if (activeDevice_ != InputDevice::Controller)
			{
				if (!ControllerHandler::ShouldApply(event))
				{
					return;
				}

				// Switch devices if the controller had active input.
				ReleaseHeldInputFor(activeDevice_);
				activeDevice_ = InputDevice::Controller;
			}

			controllerHandler_.Apply(event);
			return;
		}

		// Kind::Connected is informational only (e.g. for HUD/telemetry); it does
		// not update the state or change the active device, since a controller
		// merely being connected is not itself an indication of player intent.
	}
}