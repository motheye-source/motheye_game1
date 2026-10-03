#pragma once
#include <motheye/model/model.h>
#include <motheye/engine/world/entity.h>
#include <motheye/engine/world/i_entity_factory.h>

#include <memory>
#include <string>

namespace game
{
	class EntityFactory : public motheye::engine::world::IEntityFactory
	{
	public:

		std::unique_ptr<motheye::engine::world::Entity> CreateInstance(
			const std::string& name,
			motheye::model::EntityKind kind,
			const std::string& classname) override;

		bool HasClass(const std::string& classname) override;
	};

}