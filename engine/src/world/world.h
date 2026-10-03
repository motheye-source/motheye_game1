#pragma once
#include <motheye/engine/world/entity.h>
#include <motheye/engine/world/tick_context.h>
#include "node.h"
#include "engine/src/world/entity_manager.h"
#include "renderer/renderer.h"
#include "renderer/resource_handle.h"

#include <vector>
#include <memory>

namespace motheye::engine::world
{
    namespace data = motheye::model;

    class World
    {
    public:

        EntityManager& GetEntityManager();

        void Load(
            renderer::Renderer& renderer,
            const data::Model& data,
            const std::filesystem::path& defaultTexture);

        void Stage(renderer::Renderer& renderer, const TickContext& tickContext);

    private:
        void LoadFrame(renderer::Renderer& renderer);
        void LoadNode(renderer::Renderer& renderer, const Node& node);

        void StageNode(renderer::Renderer& renderer, const Node& node, const TickContext& tickContext);

    private:
        std::unique_ptr<Node> rootNode_;
        EntityManager entityManager_;
    };    

    inline EntityManager& World::GetEntityManager()
    {
        return this->entityManager_;
    }
}
