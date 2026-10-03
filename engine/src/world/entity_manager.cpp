#pragma once
#include "src/world/entity_manager.h"

namespace motheye::engine::world
{
	EntityHandle EntityManager::CreateInstance(
		const std::string& name,
		motheye::model::EntityKind kind,
		const std::string& classname,
		renderer::InstanceHandle instanceHandle)
	{
		std::unique_ptr<Entity> instance;

		for (auto& factory : factories_)
		{
			if (factory->HasClass(classname))
			{
				instance = factory->CreateInstance(name, kind, classname);
				break;
			}
		}

		if (!instance)
		{
			instance = std::make_unique<Entity>(name, kind, classname);
		}

		EntityHandle handle = static_cast<EntityHandle>(instances_.size());

		instances_.emplace_back(std::move(instance), instanceHandle);
		nameMap_.emplace(name, handle);

		return handle;
	}
}