#pragma once
#include <motheye/platform/input/controller_analog_state.h>
#include <motheye/platform/input/controller_event.h>

#include <cstdint>

namespace motheye::engine::input
{
	using motheye::platform::input::ControllerAnalogState;
	using motheye::platform::input::ControllerEvent;

	struct ControllerState
	{
		uint32_t heldButtons{ ControllerEvent::ButtonMask::None };
		uint32_t pressedButtons{ ControllerEvent::ButtonMask::None };
		uint32_t releasedButtons{ ControllerEvent::ButtonMask::None };

		ControllerAnalogState analog{};
	};
}
