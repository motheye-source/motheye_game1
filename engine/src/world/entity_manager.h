#pragma once
#include <motheye/model/model.h>
#include <motheye/engine/world/entity.h>
#include <motheye/engine/world/i_entity_factory.h>
//#include "lua_entity_factory.h"
//#include "lua/script_engine.h"
//#include "lua/ref.h"

#include "renderer/instance_handle.h"
#include "entity_instance.h"

#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <unordered_map>

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

		EntityHandle CreateInstance(
			const std::string& name, 
			motheye::model::EntityKind kind, 
			const std::string& classname,
			renderer::InstanceHandle instanceHandle);
		

		EntityInstance* GetInstance(size_t handle);
		EntityHandle GetHandleByName(const std::string& name);

	private:
		//LuaEntityFactory& GetLuaEntityFactory();

	private:
		//engine::lua::ScriptEngine& scriptEngine_;
		std::vector<std::unique_ptr<IEntityFactory>> factories_;
		std::vector<EntityInstance> instances_;
		std::unordered_map<std::string, EntityHandle> nameMap_;
	};

	//inline EntityManager::EntityManager(engine::lua::ScriptEngine& scriptEngine) :
	//	scriptEngine_(scriptEngine)
	//{
	//	factories_.emplace_back(std::make_unique<LuaEntityFactory>(scriptEngine));
	//}
	inline EntityManager::EntityManager()
	{
		// Reserve index 0 for invalid handle.
		instances_.emplace_back(nullptr, renderer::InstanceHandle());
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

	inline EntityInstance* EntityManager::GetInstance(EntityHandle handle)
	{
		if ((handle > 0) && (handle < instances_.size()))
		{
			return &instances_[handle];
		}
		return nullptr;
	}

	inline EntityHandle EntityManager::GetHandleByName(const std::string& name)
	{
		const auto it = nameMap_.find(name);
		if (it == nameMap_.end())
		{
			return InvalidEntityHandle;
		}
		return it->second;
	}
}