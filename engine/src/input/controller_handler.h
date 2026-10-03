#pragma once
#include <motheye/engine/input/controller_state.h>
#include <motheye/platform/input/controller_analog_state.h>
#include <motheye/platform/input/controller_event.h>

#include <cstdint>

namespace motheye::engine::input
{
	using motheye::platform::input::ControllerAnalogState;
	using motheye::platform::input::ControllerEvent;

	class ControllerHandler
	{
	public:
		const ControllerState& GetState() const;

		void BeginFrame();
		void Apply(const ControllerEvent& event);
		void ReleaseAllHeld();

		static bool ShouldApply(const ControllerEvent& event);		

	private:
		ControllerState state_;
	};

	inline const ControllerState& ControllerHandler::GetState() const
	{
		return state_;
	}

	inline void ControllerHandler::Apply(const ControllerEvent& event)
	{
		const uint32_t previousHeld = state_.heldButtons;
		state_.heldButtons = event.buttonState;
		state_.pressedButtons |= (state_.heldButtons & ~previousHeld);
		state_.releasedButtons |= (previousHeld & ~state_.heldButtons);

		state_.analog = event.analog;
	}

	inline bool ControllerHandler::ShouldApply(const ControllerEvent& event)
	{
		return event.buttonState != ControllerEvent::ButtonMask::None ||
			event.analog.leftStickX != 0.0f || event.analog.leftStickY != 0.0f ||
			event.analog.rightStickX != 0.0f || event.analog.rightStickY != 0.0f ||
			event.analog.leftTrigger != 0.0f || event.analog.rightTrigger != 0.0f;
	}

	inline void ControllerHandler::BeginFrame()
	{
		state_.pressedButtons = ControllerEvent::ButtonMask::None;
		state_.releasedButtons = ControllerEvent::ButtonMask::None;
	}

	inline void ControllerHandler::ReleaseAllHeld()
	{
		state_.releasedButtons |= state_.heldButtons;
		state_.heldButtons = ControllerEvent::ButtonMask::None;

		state_.analog = ControllerAnalogState{};
	}	
}
