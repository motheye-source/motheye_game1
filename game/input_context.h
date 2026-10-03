#pragma once

namespace game
{	
	// Identifies which input "mode" raw input is currently being interpreted under.
	// Determines which GameAction values are meaningful/reachable at any given time.
	enum class InputContext
	{
		Undefined,
		Gameplay,
		Menu
	};
}