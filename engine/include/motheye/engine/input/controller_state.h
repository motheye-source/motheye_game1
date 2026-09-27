#pragma once
#include <motheye/platform/input/controller_analog_state.h>
#include <motheye/platform/input/controller_event.h>

#include <cstdint>

namespace motheye::engine::input
{
	using motheye::platform::input::ControllerAnalogState;
	using motheye::platform::input::ControllerEvent;

	// Owns the authoritative held-state for the controller, plus this-frame
	// press/release transitions, updated by applying a ControllerEvent (which
	// carries full absolute state per poll). Platform independent; adapters
	// (motheye_platform) only ever produce events, never hold this state.
	struct ControllerState
	{
		// Diffs event.buttonState against the currently held buttons to derive
		// this-frame transitions, then overwrites held state and analog values.
		void Apply(const ControllerEvent& event);

		// Clears this-frame transitions; preserves held-state and analog values.
		// Call once per Run() iteration before polling for that iteration.
		void BeginFrame();

		// Emits release transitions for any currently held buttons, then clears
		// held-state and analog values. Used when the controller stops being the
		// active device (e.g. disconnect or switch-away) so stale held state
		// doesn't leak into the next active period.
		void ReleaseAllHeld();

		// Determines whether a StateChanged event carries meaningful (post-deadzone)
		// input, and therefore should be applied and treated as controller activity.
		static bool ShouldApply(const ControllerEvent& event);

		bool IsButtonHeld(uint32_t buttonMask) const;
		bool WasButtonPressed(uint32_t buttonMask) const;
		bool WasButtonReleased(uint32_t buttonMask) const;

		uint32_t HeldButtonState() const;
		uint32_t ButtonsPressedThisFrame() const;
		uint32_t ButtonsReleasedThisFrame() const;

		const ControllerAnalogState& Analog() const;

	private:
		uint32_t heldButtons_{ ControllerEvent::ButtonMask::None };
		uint32_t pressedButtons_{ ControllerEvent::ButtonMask::None };
		uint32_t releasedButtons_{ ControllerEvent::ButtonMask::None };

		ControllerAnalogState analog_{};
	};

	inline void ControllerState::Apply(const ControllerEvent& event)
	{
		const uint32_t previousHeld = heldButtons_;
		heldButtons_ = event.buttonState;
		pressedButtons_ |= (heldButtons_ & ~previousHeld);
		releasedButtons_ |= (previousHeld & ~heldButtons_);

		analog_ = event.analog;
	}

	inline bool ControllerState::ShouldApply(const ControllerEvent& event)
	{
		return event.buttonState != ControllerEvent::ButtonMask::None ||
			event.analog.leftStickX != 0.0f || event.analog.leftStickY != 0.0f ||
			event.analog.rightStickX != 0.0f || event.analog.rightStickY != 0.0f ||
			event.analog.leftTrigger != 0.0f || event.analog.rightTrigger != 0.0f;
	}

	inline void ControllerState::BeginFrame()
	{
		pressedButtons_ = ControllerEvent::ButtonMask::None;
		releasedButtons_ = ControllerEvent::ButtonMask::None;
	}

	inline void ControllerState::ReleaseAllHeld()
	{
		releasedButtons_ |= heldButtons_;
		heldButtons_ = ControllerEvent::ButtonMask::None;

		analog_ = ControllerAnalogState{};
	}

	inline bool ControllerState::IsButtonHeld(uint32_t buttonMask) const
	{
		return (heldButtons_ & buttonMask) != 0;
	}

	inline bool ControllerState::WasButtonPressed(uint32_t buttonMask) const
	{
		return (pressedButtons_ & buttonMask) != 0;
	}

	inline bool ControllerState::WasButtonReleased(uint32_t buttonMask) const
	{
		return (releasedButtons_ & buttonMask) != 0;
	}

	inline uint32_t ControllerState::HeldButtonState() const
	{
		return heldButtons_;
	}

	inline uint32_t ControllerState::ButtonsPressedThisFrame() const
	{
		return pressedButtons_;
	}

	inline uint32_t ControllerState::ButtonsReleasedThisFrame() const
	{
		return releasedButtons_;
	}

	inline const ControllerAnalogState& ControllerState::Analog() const
	{
		return analog_;
	}
}
