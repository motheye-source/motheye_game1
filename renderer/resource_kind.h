#pragma once
#include <cstdint>

namespace motheye::renderer
{
	enum class ResourceKind : uint16_t
	{
		Invalid = 0,
		Camera,
		Light,
		Material,
		Mesh,
		Solid,
		Texture
	};
}