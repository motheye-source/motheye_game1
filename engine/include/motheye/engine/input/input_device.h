#pragma once

namespace motheye::engine::input
{
	// Identifies which physical device is currently authoritative for gameplay
	// input. Mutually exclusive: exactly one device is active at any given time.
	enum class InputDevice
	{
		MouseKeyboard,
		Controller
	};
}
