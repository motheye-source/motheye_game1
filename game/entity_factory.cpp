#include "entity_factory.h"
#include "fps_camera.h"

using motheye::engine::world::Entity;

namespace game
{
	std::unique_ptr<motheye::engine::world::Entity> EntityFactory::CreateInstance(
		const std::string& name,
		motheye::model::EntityKind kind,
		const std::string& classname)
	{
		if (classname == "fps_camera")
		{
			return std::make_unique<FPSCamera>(name, kind, classname);
		}

		return nullptr;
	}


	bool EntityFactory::HasClass(const std::string& classname)
	{
		if (classname == "fps_camera")
		{
			return true;
		}

		return false;
	}
}