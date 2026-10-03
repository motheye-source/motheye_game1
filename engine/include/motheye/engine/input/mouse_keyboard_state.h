#pragma once
#include <motheye/platform/input/mouse_keyboard_event.h>
#include <motheye/platform/input/key_code.h>

#include <bitset>
#include <cstdint>

namespace motheye::engine::input
{
	using motheye::platform::input::MouseKeyboardEvent;
	using motheye::platform::input::KeyCode;
	using motheye::platform::input::KeyCodeLimits;

	struct MouseKeyboardState
	{
		std::bitset<KeyCodeLimits::Size> heldKeys{};
		std::bitset<KeyCodeLimits::Size> pressedKeys{};
		std::bitset<KeyCodeLimits::Size> releasedKeys{};

		uint32_t heldButtons{ MouseKeyboardEvent::ButtonMask::None };
		uint32_t pressedButtons{ MouseKeyboardEvent::ButtonMask::None };
		uint32_t releasedButtons{ MouseKeyboardEvent::ButtonMask::None };

		int32_t mouseDeltaX{ 0 };
		int32_t mouseDeltaY{ 0 };
	};
}
