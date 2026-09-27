#pragma once

namespace renderer
{
	struct ResourceLimits
	{
		size_t MaxLights = 16;
		size_t MaxSolids = 1000;
		size_t MaxTextures = 500;
		size_t MaxMaterials = 500;
		size_t MaxMeshes = 500;
	};
}