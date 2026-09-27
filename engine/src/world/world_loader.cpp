#include "world_loader.h"

#include "axes_geometry.h"

#include <motheye/blender/compiler.h>
#include <motheye/compiler/compiler.h>
#include <renderer/instance_handle.h>
#include <renderer/mesh_resource.h>

#include <SimpleMath.h>
#include <DirectXMathConvert.inl>

#include <utility>
#include <cassert>
#include <string>

using namespace motheye::renderer;

namespace motheye::engine::world
{
    std::unique_ptr<World> WorldLoader::Load(
        Renderer& renderer, 
        const data::Model& data, 
        const std::filesystem::path& defaultTexture)
    {       
        renderer.InitScene();

        WorldLoader loader(defaultTexture);
        auto world = loader.LoadData(data, renderer);

        loader.LoadFrame(renderer, *world);

        return world;
    }

    std::unique_ptr<World> WorldLoader::LoadData(const data::Model& data, Renderer& renderer)
    {
        std::unique_ptr<World> world = std::make_unique<World>();

        this->LoadCameras(renderer, data.resources.cameras);
        this->LoadLights(renderer, data.resources.lights);
        this->LoadTextures(renderer, data.resources.textures);
        this->LoadMaterials(renderer, data.resources.materials);
        this->LoadMeshes(renderer, data.resources.meshes);
        this->LoadSolids(renderer, data.resources.solids);
        this->LoadEntities(*world, data.entities);

        return world;
    }

    void WorldLoader::LoadFrame(Renderer& renderer, World& world)
    {
        // For now, push once into the frame and do not clear.
        for (auto& object : world.GetRoot())
        {
            switch (object.kind)
            {
            case data::EntityKind::kCamera:
                object.instance = renderer.CreateInstance(object.resource.As<CameraResourceHandle>());
                renderer.GetFrame().SetCamera(object.instance.As<CameraInstanceHandle>());
                break;
            
            case data::EntityKind::kSolid:
                object.instance = renderer.CreateInstance(object.resource.As<SolidResourceHandle>());
                renderer.GetFrame().Push(object.instance.As<SolidInstanceHandle>());
                break;
            
            case data::EntityKind::kLight:
                object.instance = renderer.CreateInstance(object.resource.As<LightResourceHandle>());
                renderer.GetFrame().Push(object.instance.As<LightInstanceHandle>());
                break;
            }
        }

        // Add unlighted meshes/solids here.
        auto axesMesh = renderer.CreateMeshResource<MeshKind::Unlighted>(AxesGeometry::vertices, AxesGeometry::indices);
        auto axesSolid = renderer.CreateSolidResource(axesMesh);
        auto instance = renderer.CreateInstance(axesSolid);
        auto axesMatrix = DirectX::SimpleMath::Matrix::CreateScale(100.0f);
        renderer.SetInstanceMatrix(instance, DirectX::XMLoadFloat4x4(&axesMatrix));
        renderer.GetFrame().Push(instance.As<SolidInstanceHandle>());
    }

    void WorldLoader::LoadCameras(Renderer& renderer, const std::vector<data::Camera>& data)
    {
        // Create camera resources
        for (const auto& camera : data)
        {
            auto resource = renderer.CreateCameraResource(
                static_cast<float>(camera.fov), 
                static_cast<float>(camera.nearClip), 
                static_cast<float>(camera.farClip));

            resourceMap_.AddCamera(camera.name, resource);
        }
    }

    void WorldLoader::LoadLights(Renderer& renderer, const std::vector<data::Light>& data)
    {
        // Create light resources
        for (const auto& light : data)
        {
            auto resource = renderer.CreateLightResource(ToFloat4(light.color));
            resourceMap_.AddLight(light.name, resource);
        }
    }

    void WorldLoader::LoadTextures(Renderer& renderer, const std::vector<data::Texture>& data)
    {
        auto defaultResource = renderer.CreateTextureResource(defaultTexture_.string());
        resourceMap_.AddTexture("", defaultResource);

        // Create texture resources
        for (const auto& texture : data)
        {
            auto path = texture.path;

            // The deserialized solid must have all relative paths resolved to absolute.
            assert(path.is_absolute());

            // For now, replace the extension to .dds.
            path.replace_extension(".dds");

            auto resource = renderer.CreateTextureResource(path.string());
            if (!resource.IsValid())
            {
                resource = defaultResource;
            }

            resourceMap_.AddTexture(texture.name, resource);
        }
    }

    void WorldLoader::LoadMaterials(Renderer& renderer, const std::vector<data::Material>& data)
    {
        // Create material resources
        for (const auto& material : data)
        {
            auto texture = resourceMap_.GetTexture(material.baseTexture);
            auto resource = renderer.CreateMaterialResource(ToFloat4(material.diffuse), texture);
            resourceMap_.AddMaterial(material.name, resource);
        }
    }

    void WorldLoader::LoadMeshes(Renderer& renderer, const std::vector<data::Mesh>& data)
    {
        // Create mesh resources
        for (const auto& mesh : data)
        {
            auto resource = CreateLightedMeshResource(renderer, mesh);
            resourceMap_.AddMesh(mesh.name, resource);
        }
    }

    void WorldLoader::LoadSolids(Renderer& renderer, const std::vector<data::Solid>& data)
    {
        // Create solid resources
        for (const auto& solid : data)
        {
            assert(solid.materials.size() > 0);

            std::vector<MaterialResourceHandle> materials;
            for (const std::string& materialName : solid.materials)
            {
                materials.push_back(resourceMap_.GetMaterial(materialName));
            }

            auto mesh = resourceMap_.GetMesh(solid.mesh);
            auto resource = renderer.CreateSolidResource(mesh, std::move(materials));
            resourceMap_.AddSolid(solid.name, resource);
        }
    }

    void WorldLoader::LoadEntities(World& world, const std::vector<data::Entity>& data)
    {
        auto& root = world.GetRoot();

        for (const auto& object : data)
        {
            ResourceHandle resource{};
            switch (object.kind)
            {
            case data::EntityKind::kCamera:
                resource = resourceMap_.GetCamera(object.resource);
                break;

            case data::EntityKind::kSolid:
                resource = resourceMap_.GetSolid(object.resource);
                break;

            case data::EntityKind::kLight:
                resource = resourceMap_.GetLight(object.resource);
                break;

            default:
                continue;
            }

            root.emplace_back(object, resource);
        }
    }

    MeshResourceHandle WorldLoader::CreateLightedMeshResource(Renderer& renderer, const data::Mesh& data)
    {
        std::vector<LightedVertex> vertices;
        std::vector<UINT16> indices;
        std::vector<MeshIndexSpan> spans;

        for (const auto& geometry : data.geometries)
        {
            size_t vertexOffset = vertices.size();

            for (const auto& vertex : geometry.vertices)
            {
                vertices.push_back(LightedVertex({
                    DirectX::XMFLOAT3(static_cast<float>(vertex.x), static_cast<float>(vertex.y), static_cast<float>(vertex.z)),
                    DirectX::XMFLOAT3(static_cast<float>(vertex.nx), static_cast<float>(vertex.ny), static_cast<float>(vertex.nz)),
                    DirectX::XMFLOAT2(static_cast<float>(vertex.u), static_cast<float>(vertex.v))
                    }));
            }

            auto tesselation = Tesselate(geometry, vertexOffset);
            spans.push_back({ static_cast<UINT>(indices.size()), static_cast<UINT>(tesselation.size()) });
            indices.insert(indices.end(), tesselation.begin(), tesselation.end());
        }

        return renderer.CreateMeshResource<MeshKind::Lighted>(vertices, indices, std::move(spans));
    }

    std::vector<UINT16> WorldLoader::Tesselate(const data::Geometry<data::SolidVertex>& geometry, size_t vertexOffset)
    {
        std::vector<UINT16> indices;

        for (const auto& polygon : geometry.polygons)
        {
            // Tesselate into a triangle fan...
            for (size_t midIndex = 1; midIndex < polygon.indices.size() - 1; midIndex++)
            {
                indices.push_back(static_cast<UINT16>(vertexOffset + polygon.indices[0]));
                indices.push_back(static_cast<UINT16>(vertexOffset + polygon.indices[midIndex]));
                indices.push_back(static_cast<UINT16>(vertexOffset + polygon.indices[midIndex + 1]));
            }
        }

        return indices;
    }
}