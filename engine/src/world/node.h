#pragma once
#include <motheye/engine/world/transform.h>
#include <motheye/engine/world/entity.h>
#include "resource_map.h"

#include "renderer/renderer.h"

#include <motheye/model/model.h>

#include <memory>
#include <vector>

namespace motheye::engine::world
{
    namespace model = motheye::model;
    using motheye::renderer::ResourceHandle;
    using motheye::renderer::InstanceHandle;

    class Node
    {
    public:
        EntityHandle GetEntityHandle() const;
        void SetEntityHandle(EntityHandle value);
        
        std::vector<Node>& GetNodes();
        const std::vector<Node>& GetNodes() const;
                
    private:    
        EntityHandle entityHandle_{ InvalidEntityHandle };
        std::vector<Node> nodes_;
    };
    
    inline EntityHandle Node::GetEntityHandle() const    
    {
        return this->entityHandle_;
    }

    inline void Node::SetEntityHandle(EntityHandle value)
    {
        this->entityHandle_ = value;
    }

    inline std::vector<Node>& Node::GetNodes()
    {
        return this->nodes_;
    }
    
    inline const std::vector<Node>& Node::GetNodes() const
    {
        return this->nodes_;
    }

}
