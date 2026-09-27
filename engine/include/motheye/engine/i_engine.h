#pragma once
#include <motheye/model/model.h>

#include <filesystem>

namespace motheye::engine
{
	struct IEngine
	{
		virtual void LoadWorld(const motheye::model::Model& model, const std::filesystem::path& defaultTexture) = 0;
	};
}