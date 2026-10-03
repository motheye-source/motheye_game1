#pragma once
#include <motheye/model/model.h>
#include "transform.h"
#include "tick_context.h"

#include <string>

namespace motheye::engine::world
{
	namespace model = motheye::model;

	using EntityHandle = size_t;
	static constexpr  EntityHandle InvalidEntityHandle = EntityHandle{ 0 };

	class Entity
	{
	public:

		Entity(const std::string& name, motheye::model::EntityKind kind, const std::string& classname);

		const std::string& GetName() const;
		model::EntityKind GetKind() const;
		const std::string& ClassName() const;
		
		const Transform& GetTransform() const;

		DirectX::XMMATRIX ComputeWorldMatrix() const;

		virtual void Initialize();
		virtual void Tick(const TickContext& context);

	private:
		const std::string name_;
		model::EntityKind kind_{ model::EntityKind::kUnknown };
		const std::string classname_;
		Transform transform_;
	};

	inline Entity::Entity(const std::string& name, motheye::model::EntityKind kind, const std::string& classname) :
		name_(name),
		kind_(kind),
		classname_(classname)
	{
	}

	inline const std::string& Entity::GetName() const
	{
		return this->name_;
	}

	inline model::EntityKind Entity::GetKind() const
	{
		return this->kind_;
	}

	inline const std::string& Entity::ClassName() const
	{
		return this->classname_;
	}

	inline const Transform& Entity::GetTransform() const
	{
		return transform_;
	}

	inline DirectX::XMMATRIX Entity::ComputeWorldMatrix() const
	{
		DirectX::XMFLOAT4X4 transformMatrix;
		this->transform_.ToMatrix(transformMatrix);
		return XMLoadFloat4x4(&transformMatrix);
	}

	inline void Entity::Initialize()
	{
	}

	inline void Entity::Tick(const TickContext& context)
	{
		(void)context;
	}
}