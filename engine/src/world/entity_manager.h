#pragma once
#include <motheye/model/model.h>
#include <motheye/engine/world/entity.h>
#include <motheye/engine/world/i_entity_factory.h>
//#include "lua_entity_factory.h"
//#include "lua/script_engine.h"
//#include "lua/ref.h"

#include <memory>
#include <string>
#include <filesystem>
#include <vector>

namespace motheye::engine::world
{
	class EntityManager
	{
	public:

		//EntityManager(engine::lua::ScriptEngine& scriptEngine);
		EntityManager();

		//bool RegisterClass(const std::string& classname, const std::filesystem::path& path);
		void RegisterFactory(std::unique_ptr<IEntityFactory>&& factory);

		//engine::lua::Ref* GetClassRef(const std::string& classname);

		EntityHandle CreateInstance(const std::string& classname, const std::string& name, motheye::model::EntityKind kind);
		Entity* GetInstance(size_t handle);

	private:
		//LuaEntityFactory& GetLuaEntityFactory();

	private:
		//engine::lua::ScriptEngine& scriptEngine_;
		std::vector<std::unique_ptr<IEntityFactory>> factories_;
		std::vector<std::unique_ptr<Entity>> instances_;
	};

	//inline EntityManager::EntityManager(engine::lua::ScriptEngine& scriptEngine) :
	//	scriptEngine_(scriptEngine)
	//{
	//	factories_.emplace_back(std::make_unique<LuaEntityFactory>(scriptEngine));
	//}
	inline EntityManager::EntityManager()
	{
		// Reserve index 0 for invalid handle.
		instances_.push_back(nullptr);
	}

	//inline LuaEntityFactory& EntityManager::GetLuaEntityFactory()
	//{
	//	return static_cast<LuaEntityFactory&>(*factories_[0].get());
	//}

	//inline bool EntityManager::RegisterClass(const std::string& classname, const std::filesystem::path& path)
	//{
	//	return GetLuaEntityFactory().RegisterClass(classname, path);
	//}

	inline void EntityManager::RegisterFactory(std::unique_ptr<IEntityFactory>&& factory)
	{
		factories_.push_back(std::move(factory));
	}

	//inline engine::lua::Ref* EntityManager::GetClassRef(const std::string& classname)
	//{
	//	return GetLuaEntityFactory().GetClassRef(classname);
	//}

	inline Entity* EntityManager::GetInstance(EntityHandle handle)
	{
		if ((handle > 0) && (handle < instances_.size()))
		{
			return instances_[handle].get();
		}
		return nullptr;
	}
}