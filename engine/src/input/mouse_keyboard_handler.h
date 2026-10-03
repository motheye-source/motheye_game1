#pragma once
#include <motheye/engine/input/mouse_keyboard_state.h>
#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/key_code.h>

#include <bitset>
#include <cstdint>

namespace motheye::engine::input
{
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::KeyCode;
	using motheye::platform::input::KeyCodeLimits;

	class MouseKeyboardHandler
	{
	public:
		const MouseKeyboardState& GetState() const;

		void BeginFrame();
		void Apply(const MouseKeyboardEvent& event);
		void ReleaseAllHeld();
		
	private:
		MouseKeyboardState state_;
	};
	
	inline const MouseKeyboardState& MouseKeyboardHandler::GetState() const
	{
		return state_;
	}

	inline void MouseKeyboardHandler::BeginFrame()
	{
		state_.pressedKeys.reset();
		state_.releasedKeys.reset();
		state_.pressedButtons = MouseKeyboardEvent::ButtonMask::None;
		state_.releasedButtons = MouseKeyboardEvent::ButtonMask::None;
		state_.mouseDeltaX = 0;
		state_.mouseDeltaY = 0;
	}

	inline void MouseKeyboardHandler::ReleaseAllHeld()
	{
		state_.releasedKeys |= state_.heldKeys;
		state_.heldKeys.reset();

		state_.releasedButtons |= state_.heldButtons;
		state_.heldButtons = MouseKeyboardEvent::ButtonMask::None;
	}	
}
