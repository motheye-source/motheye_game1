#pragma once
#include "world.h"
#include "resource_map.h"

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
        static std::unique_ptr<World> Load(
            Renderer& renderer, 
            const data::Model& model,
            const std::filesystem::path& defaultTexture
        );

    private:
        WorldLoader(const std::filesystem::path& defaultTexture);

        std::unique_ptr<World> LoadData(const data::Model& data, Renderer& renderer);
        void LoadCameras(Renderer& renderer, const std::vector<data::Camera>& data);
        void LoadLights(Renderer& renderer, const std::vector<data::Light>& data);
        void LoadTextures(Renderer& renderer, const std::vector<data::Texture>& data);
        void LoadMaterials(Renderer& renderer, const std::vector<data::Material>& data);
        void LoadMeshes(Renderer& renderer, const std::vector<data::Mesh>& data);
        void LoadSolids(Renderer& renderer, const std::vector<data::Solid>& data);
        void LoadEntities(World& world, const std::vector<data::Entity>& data);
                
        void LoadDefaultCamera(data::Model& data);
        void LoadFrame(Renderer& renderer, World& world);

        static MeshResourceHandle CreateLightedMeshResource(Renderer& renderer, const data::Mesh& data);
        static std::vector<UINT16> Tesselate(const data::Geometry<data::SolidVertex>& geometry, size_t vertexOffset);
        static DirectX::XMFLOAT4 ToFloat4(const motheye::math::Vector4& from);
        
    private:
        const std::filesystem::path assetsPath_;
        const std::filesystem::path defaultTexture_;
        ResourceMap resourceMap_;
    };

    inline WorldLoader::WorldLoader(const std::filesystem::path& defaultTexture) :
        defaultTexture_(defaultTexture)
    {
    }

    inline DirectX::XMFLOAT4 WorldLoader::ToFloat4(const motheye::math::Vector4& from)
    {
        return DirectX::XMFLOAT4(
            static_cast<float>(from.x), static_cast<float>(from.y),
            static_cast<float>(from.z), static_cast<float>(from.w));
    }
}