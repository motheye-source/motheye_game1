#pragma once
#include <motheye/model/model.h>
#include <motheye/engine/world/i_entity_factory.h>

#include <filesystem>
#include <memory>

namespace motheye::engine
{
	struct IEngine
	{		
		virtual void RegisterEntityFactory(std::unique_ptr<world::IEntityFactory>&& factory) = 0;

		virtual void LoadWorld(const motheye::model::Model& model, const std::filesystem::path& defaultTexture) = 0;
	};
}