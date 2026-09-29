#pragma once
#include "src/world/entity_manager.h"

namespace motheye::engine::world
{
	EntityHandle EntityManager::CreateInstance(
		const std::string& classname,
		const std::string& name,
		motheye::model::EntityKind kind)
	{
		std::unique_ptr<Entity> instance;

		for (auto& factory : factories_)
		{
			if (factory->HasClass(classname))
			{
				instance = factory->CreateInstance(classname, name, kind);
				break;
			}
		}

		if (!instance)
		{
			instance = std::make_unique<Entity>(classname, name, kind);
		}

		instances_.push_back(std::move(instance));

		return static_cast<EntityHandle>(instances_.size() - 1);
	}
}