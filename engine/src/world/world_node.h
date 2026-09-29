#pragma once
#include <motheye/engine/world/transform.h>
#include <motheye/engine/world/entity.h>
#include "resource_map.h"

#include "renderer/renderer.h"

#include <motheye/model/model.h>

#include <memory>

namespace motheye::engine::world
{
    namespace model = motheye::model;
    using motheye::renderer::ResourceHandle;
    using motheye::renderer::InstanceHandle;

    class WorldNode
    {
    public:
        WorldNode() = default;
        WorldNode(size_t entityHandle, ResourceHandle resource, InstanceHandle instance);

        size_t GetEntityHandle() const;
        ResourceHandle GetResource() const;
        InstanceHandle GetInstance() const;
                
    private:    
        size_t entityHandle_;
        ResourceHandle resource_{};
        InstanceHandle instance_{};
    };

    inline WorldNode::WorldNode(size_t entityHandle, ResourceHandle resource, InstanceHandle instance) :
        entityHandle_(entityHandle),
        resource_(resource),
        instance_(instance)
    {
    }

    inline size_t WorldNode::GetEntityHandle() const
    {
        return this->entityHandle_;
    }

    inline ResourceHandle WorldNode::GetResource() const
    {
        return this->resource_;
    }
    
    inline InstanceHandle WorldNode::GetInstance() const
    {
        return this->instance_;
    } 
}
