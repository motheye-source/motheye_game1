#pragma once
#include "src/world/world.h"
#include "src/world/resource_map.h"
#include "src/world/entity_manager.h"

#include "renderer/renderer.h"
#include "renderer/resource_handle.h"

#include <motheye/model/model.h>
#include <motheye/math/vector4.h>
#include <DirectXMath.h>

#include <memory>
#include <vector>
#include <filesystem>

namespace motheye::engine::world
{
    using namespace motheye::renderer;
    namespace data = motheye::model;

    class WorldLoader
    {
    public:
        static std::unique_ptr<Node> Load(
            Renderer& renderer,
            EntityManager& entityManager,
            const data::Model& model,
            const std::filesystem::path& defaultTexture
        );

    private:
        WorldLoader(
            Renderer& renderer,
            EntityManager& entityManager,
            const std::filesystem::path& defaultTexture
        );

        void LoadResources(const data::Model& data);
        void LoadCameras(const std::vector<data::Camera>& data);
        void LoadLights(const std::vector<data::Light>& data);
        void LoadTextures(const std::vector<data::Texture>& data);
        void LoadMaterials(const std::vector<data::Material>& data);
        void LoadMeshes(const std::vector<data::Mesh>& data);
        void LoadSolids(const std::vector<data::Solid>& data);                
        void LoadEntities(const std::vector<data::Entity>& data);
        void LoadNode(Node& node, const data::Node& data);
        
        static MeshResourceHandle CreateLightedMeshResource(Renderer& renderer, const data::Mesh& data);
        static std::vector<UINT16> Tesselate(const data::Geometry<data::SolidVertex>& geometry, size_t vertexOffset);
        static DirectX::XMFLOAT4 ToFloat4(const motheye::math::Vector4& from);
        
    private:
        const std::filesystem::path assetsPath_;
        const std::filesystem::path defaultTexture_;
        Renderer& renderer_;
        EntityManager& entityManager_;

        ResourceMap resourceMap_;
    };

    inline WorldLoader::WorldLoader(
        Renderer& renderer, 
        EntityManager& entityManager, 
        const std::filesystem::path& defaultTexture) 
        : renderer_(renderer)
        , entityManager_(entityManager)
        , defaultTexture_(defaultTexture)
    {
    }

    inline DirectX::XMFLOAT4 WorldLoader::ToFloat4(const motheye::math::Vector4& from)
    {
        return DirectX::XMFLOAT4(
            static_cast<float>(from.x), static_cast<float>(from.y),
            static_cast<float>(from.z), static_cast<float>(from.w));
    }
}