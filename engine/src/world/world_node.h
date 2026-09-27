#pragma once
#include "transform.h"
#include "resource_map.h"

#include "renderer/renderer.h"

#include <motheye/model/model.h>

namespace motheye::engine::world
{
    namespace data = motheye::model;
    using motheye::renderer::ResourceHandle;
    using motheye::renderer::InstanceHandle;

    struct WorldNode
    {
        WorldNode() = default;
        WorldNode(const data::Entity& data, ResourceHandle resource);

        DirectX::XMMATRIX ComputeWorldMatrix() const;
        
        data::EntityKind kind;
        const std::string name;
        const std::string classname;
        
        Transform transform;

        ResourceHandle resource{};
        InstanceHandle instance{};
    };

    inline WorldNode::WorldNode(const data::Entity& data, ResourceHandle resource) :
        kind(data.kind),
        name(data.name),
        classname(data.classname),
        transform(data.transform),
        resource(resource)
    {
    }

    inline DirectX::XMMATRIX WorldNode::ComputeWorldMatrix() const
    {
        DirectX::XMFLOAT4X4 transformMatrix;
        transform.ToMatrix(transformMatrix);
        return XMLoadFloat4x4(&transformMatrix);
    }
}
