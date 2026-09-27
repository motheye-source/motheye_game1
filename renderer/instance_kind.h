#pragma once
#include <cstdint>

namespace motheye::renderer
{
	enum class InstanceKind : uint16_t
	{
		Invalid = 0,
		Camera,
		Light,
		Solid
	};
}