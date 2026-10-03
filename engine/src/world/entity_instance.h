#pragma once
#include <motheye/engine/world/entity.h>
#include "renderer/instance_handle.h"

#include <memory>

namespace motheye::engine::world
{
	class EntityInstance
	{
	public:

		EntityInstance(std::unique_ptr<Entity>&& entity, renderer::InstanceHandle instanceHandle);

		Entity& GetEntity();
		renderer::InstanceHandle GetInstanceHandle() const;

	private:
		std::unique_ptr<Entity> entity_;
		renderer::InstanceHandle instanceHandle_;
	};

	inline EntityInstance::EntityInstance(std::unique_ptr<Entity>&& entity, renderer::InstanceHandle instanceHandle)
		: entity_(std::move(entity))
		, instanceHandle_(instanceHandle)		
	{
	}

	inline Entity& EntityInstance::GetEntity()
	{
		return *this->entity_;
	}

	inline renderer::InstanceHandle EntityInstance::GetInstanceHandle() const
	{
		return this->instanceHandle_;
	}

}