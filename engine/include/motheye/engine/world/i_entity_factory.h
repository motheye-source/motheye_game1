#pragma once
#include "entity.h"
#include <motheye/model/model.h>

#include <string>
#include <memory>

namespace motheye::engine::world
{
	class IEntityFactory
	{
	public:
		virtual std::unique_ptr<Entity> CreateInstance(
			const std::string& classname, const std::string& name, motheye::model::EntityKind kind) = 0;
		
		virtual bool HasClass(const std::string& classname) = 0;
	};
}