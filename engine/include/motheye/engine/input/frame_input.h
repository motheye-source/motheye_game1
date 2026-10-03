#pragma once
#include <motheye/platform/input/key_code.h>
#include <motheye/platform/input/controller_analog_state.h>

#include "input_device.h"
#include "mouse_keyboard_state.h"
#include "controller_state.h"

#include <cstdint>

namespace motheye::engine::input
{
	using motheye::platform::input::KeyCode;
	using motheye::platform::input::ControllerAnalogState;

	class FrameInput
	{
	public:

		class MouseKeyboard
		{
		public:
			MouseKeyboard(const MouseKeyboardState& state);

			bool IsKeyHeld(KeyCode key) const;
			bool WasKeyPressed(KeyCode key) const;
			bool WasKeyReleased(KeyCode key) const;

			uint32_t HeldButtonState() const;
			uint32_t ButtonsPressedThisFrame() const;
			uint32_t ButtonsReleasedThisFrame() const;

			int32_t MouseDeltaX() const;
			int32_t MouseDeltaY() const;
		
		private:
			const MouseKeyboardState& state_;
		};

		class Controller
		{
		public:
			Controller(const ControllerState& state);

			bool IsButtonHeld(uint32_t buttonMask) const;
			bool WasButtonPressed(uint32_t buttonMask) const;
			bool WasButtonReleased(uint32_t buttonMask) const;

			uint32_t HeldButtonState() const;
			uint32_t ButtonsPressedThisFrame() const;
			uint32_t ButtonsReleasedThisFrame() const;

			const ControllerAnalogState& Analog() const;
		
		private:
			const ControllerState& state_;
		};

	public:	

		FrameInput(
			const InputDevice& activeDevice,
			const MouseKeyboardState& mouseKeyboardState,
			const ControllerState& controllerState);

		InputDevice GetActiveDevice() const;
		const MouseKeyboard& GetMouseKeyboard() const;
		const Controller& GetController() const;
		
	private:
		const InputDevice& activeDevice_{ InputDevice::MouseKeyboard };
		const MouseKeyboard mouseKeyboard_;
		const Controller controller_;
	};

	inline FrameInput::FrameInput(
		const InputDevice& activeDevice,
		const MouseKeyboardState& mouseKeyboardState,
		const ControllerState& controllerState)
		: activeDevice_(activeDevice)
		, mouseKeyboard_(mouseKeyboardState)
		, controller_(controllerState)
	{
	}

	inline InputDevice FrameInput::GetActiveDevice() const
	{
		return this->activeDevice_;
	}

	inline const FrameInput::MouseKeyboard& FrameInput::GetMouseKeyboard() const
	{
		return this->mouseKeyboard_;
	}

	inline const FrameInput::Controller& FrameInput::GetController() const
	{
		return this->controller_;
	}

	inline FrameInput::MouseKeyboard::MouseKeyboard(const MouseKeyboardState& state) :
		state_(state)
	{
	}

	inline bool FrameInput::MouseKeyboard::IsKeyHeld(KeyCode key) const
	{
		return state_.heldKeys.test(static_cast<size_t>(key));
	}

	inline bool FrameInput::MouseKeyboard::WasKeyPressed(KeyCode key) const
	{
		return state_.pressedKeys.test(static_cast<size_t>(key));
	}

	inline bool FrameInput::MouseKeyboard::WasKeyReleased(KeyCode key) const
	{
		return state_.releasedKeys.test(static_cast<size_t>(key));
	}

	inline uint32_t FrameInput::MouseKeyboard::HeldButtonState() const
	{
		return state_.heldButtons;
	}

	inline uint32_t FrameInput::MouseKeyboard::ButtonsPressedThisFrame() const
	{
		return state_.pressedButtons;
	}

	inline uint32_t FrameInput::MouseKeyboard::ButtonsReleasedThisFrame() const
	{
		return state_.releasedButtons;
	}
	
	inline int32_t FrameInput::MouseKeyboard::MouseDeltaX() const
	{
		return state_.mouseDeltaX;
	}
	
	inline int32_t FrameInput::MouseKeyboard::MouseDeltaY() const
	{
		return state_.mouseDeltaY;
	}

	inline FrameInput::Controller::Controller(const ControllerState& state) :
		state_(state)
	{
	}

	inline bool FrameInput::Controller::IsButtonHeld(uint32_t buttonMask) const
	{
		return (state_.heldButtons & buttonMask) != 0;
	}

	inline bool FrameInput::Controller::WasButtonPressed(uint32_t buttonMask) const
	{
		return (state_.pressedButtons & buttonMask) != 0;
	}

	inline bool FrameInput::Controller::WasButtonReleased(uint32_t buttonMask) const
	{
		return (state_.releasedButtons & buttonMask) != 0;
	}

	inline uint32_t FrameInput::Controller::HeldButtonState() const
	{
		return state_.heldButtons;
	}

	inline uint32_t FrameInput::Controller::ButtonsPressedThisFrame() const
	{
		return state_.pressedButtons;
	}

	inline uint32_t FrameInput::Controller::ButtonsReleasedThisFrame() const
	{
		return state_.releasedButtons;
	}

	inline const ControllerAnalogState& FrameInput::Controller::Analog() const
	{
		return state_.analog;
	}



}