#include "world.h"
#include "world_loader.h"
#include "axes_geometry.h"

#include <SimpleMath.h>
#include <DirectXMathConvert.inl>

namespace motheye::engine::world
{
    void World::Load(
        renderer::Renderer& renderer,
        const data::Model& data,
        const std::filesystem::path& defaultTexture)
    {
        renderer.InitScene();

        this->rootNode_ = WorldLoader::Load(renderer, entityManager_, data, defaultTexture);

        LoadFrame(renderer);
    }

    void World::LoadFrame(renderer::Renderer& renderer)
    {
        // For now, push once into the frame and do not clear.
        LoadNode(renderer, *this->rootNode_);
        
        // Add unlighted meshes/solids here.
        const auto axesMesh = renderer.CreateMeshResource<MeshKind::Unlighted>(AxesGeometry::vertices, AxesGeometry::indices);
        const auto axesSolid = renderer.CreateSolidResource(axesMesh);
        const auto instance = renderer.CreateInstance(axesSolid);
        const auto axesMatrix = DirectX::SimpleMath::Matrix::CreateScale(100.0f);
        renderer.SetInstanceMatrix(instance, DirectX::XMLoadFloat4x4(&axesMatrix));
        renderer.GetFrame().Push(instance.As<SolidInstanceHandle>());
    }

    void World::LoadNode(renderer::Renderer& renderer, const Node& node)
    {
        const auto entity = entityManager_.GetInstance(node.GetEntityHandle());

        if (entity)
        {
            switch (entity->GetEntity().GetKind())
            {
            case data::EntityKind::kCamera:
                renderer.GetFrame().SetCamera(entity->GetInstanceHandle().As<CameraInstanceHandle>());
                break;

            case data::EntityKind::kSolid:
                renderer.GetFrame().Push(entity->GetInstanceHandle().As<SolidInstanceHandle>());
                break;

            case data::EntityKind::kLight:
                renderer.GetFrame().Push(entity->GetInstanceHandle().As<LightInstanceHandle>());
                break;
            }
        }

        const auto& subnodes = node.GetNodes();
        for (const auto& subnode : subnodes)
        {
            LoadNode(renderer, subnode);
        }
    }

    void World::Stage(renderer::Renderer& renderer)
    {
        StageNode(renderer, *this->rootNode_);
    }

    void World::StageNode(renderer::Renderer& renderer, const Node& node)
    {
        const auto instance = entityManager_.GetInstance(node.GetEntityHandle());
        if (instance)
        {
            TickContext context;
            instance->GetEntity().Tick(context);

            const auto worldMatrix = instance->GetEntity().ComputeWorldMatrix();

            switch (instance->GetEntity().GetKind())
            {
                case motheye::model::EntityKind::kCamera:
                {
                    const auto camera = instance->GetInstanceHandle().As<renderer::CameraInstanceHandle>();
                    renderer.SetInstanceMatrix(camera, worldMatrix);
                    renderer.GetFrame().SetCamera(camera);
                }
                break;

                case motheye::model::EntityKind::kSolid:
                {
                    const auto solid = instance->GetInstanceHandle().As<renderer::SolidInstanceHandle>();
                    renderer.SetInstanceMatrix(solid, worldMatrix);
                }
                break;
           
                case motheye::model::EntityKind::kLight:
                {
                    const auto light = instance->GetInstanceHandle().As<renderer::LightInstanceHandle>();
                    renderer.SetInstanceMatrix(light, worldMatrix);
                }
                break;
            }
        }

        const auto& subnodes = node.GetNodes();
        if (!subnodes.empty())
        {
            for (const auto& subnode : subnodes)
            {
                StageNode(renderer, subnode);
            }
        }
    }

}