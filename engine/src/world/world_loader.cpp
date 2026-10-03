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
    std::unique_ptr<Node> WorldLoader::Load(
        Renderer& renderer,
        EntityManager& entityManager,
        const data::Model& data, 
        const std::filesystem::path& defaultTexture)
    {       
        WorldLoader loader(renderer, entityManager, defaultTexture);
        
        loader.LoadResources(data);

        loader.LoadEntities(data.entities);

        std::unique_ptr<Node> rootNode = std::make_unique<Node>();
        loader.LoadNode(*rootNode, data.rootnode);

        return rootNode;
    }

    void WorldLoader::LoadResources(const data::Model& data)
    {
        this->LoadCameras(data.resources.cameras);
        this->LoadLights(data.resources.lights);
        this->LoadTextures(data.resources.textures);
        this->LoadMaterials(data.resources.materials);
        this->LoadMeshes(data.resources.meshes);
        this->LoadSolids(data.resources.solids);
    }
  
    void WorldLoader::LoadCameras(const std::vector<data::Camera>& data)
    {
        // Create camera resources
        for (const auto& camera : data)
        {
            auto resource = renderer_.CreateCameraResource(
                static_cast<float>(camera.fov), 
                static_cast<float>(camera.nearClip), 
                static_cast<float>(camera.farClip));

            resourceMap_.AddCamera(camera.name, resource);
        }
    }

    void WorldLoader::LoadLights(const std::vector<data::Light>& data)
    {
        // Create light resources
        for (const auto& light : data)
        {
            auto resource = renderer_.CreateLightResource(ToFloat4(light.color));
            resourceMap_.AddLight(light.name, resource);
        }
    }

    void WorldLoader::LoadTextures(const std::vector<data::Texture>& data)
    {
        auto defaultResource = renderer_.CreateTextureResource(defaultTexture_.string());
        resourceMap_.AddTexture("", defaultResource);

        // Create texture resources
        for (const auto& texture : data)
        {
            auto path = texture.path;

            // The deserialized solid must have all relative paths resolved to absolute.
            assert(path.is_absolute());

            // For now, replace the extension to .dds.
            path.replace_extension(".dds");

            auto resource = renderer_.CreateTextureResource(path.string());
            if (!resource.IsValid())
            {
                resource = defaultResource;
            }

            resourceMap_.AddTexture(texture.name, resource);
        }
    }

    void WorldLoader::LoadMaterials(const std::vector<data::Material>& data)
    {
        // Create material resources
        for (const auto& material : data)
        {
            auto texture = resourceMap_.GetTexture(material.baseTexture);
            auto resource = renderer_.CreateMaterialResource(ToFloat4(material.diffuse), texture);
            resourceMap_.AddMaterial(material.name, resource);
        }
    }

    void WorldLoader::LoadMeshes(const std::vector<data::Mesh>& data)
    {
        // Create mesh resources
        for (const auto& mesh : data)
        {
            auto resource = CreateLightedMeshResource(renderer_, mesh);
            resourceMap_.AddMesh(mesh.name, resource);
        }
    }

    void WorldLoader::LoadSolids(const std::vector<data::Solid>& data)
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
            auto resource = renderer_.CreateSolidResource(mesh, std::move(materials));
            resourceMap_.AddSolid(solid.name, resource);
        }
    }

    void WorldLoader::LoadEntities(const std::vector<data::Entity>& data)
    {
        for (const auto& modelEntity : data)
        {
            ResourceHandle resource{};
            InstanceHandle instance{};

            switch (modelEntity.kind)
            {
            case data::EntityKind::kCamera:
                resource = resourceMap_.GetCamera(modelEntity.resource);
                instance = renderer_.CreateInstance(resource.As<CameraResourceHandle>());
                break;

            case data::EntityKind::kSolid:
                resource = resourceMap_.GetSolid(modelEntity.resource);
                instance = renderer_.CreateInstance(resource.As<SolidResourceHandle>());
                break;

            case data::EntityKind::kLight:
                resource = resourceMap_.GetLight(modelEntity.resource);
                instance = renderer_.CreateInstance(resource.As<LightResourceHandle>());
                break;

            case data::EntityKind::kEmpty:
                break;

            default:
                continue;
            }

            auto entityHandle = entityManager_.CreateInstance(
                modelEntity.name, 
                modelEntity.kind, 
                modelEntity.classname,
                instance);
        }
    }

    void WorldLoader::LoadNode(Node& node, const data::Node& data)
    {        
        node.SetEntityHandle(entityManager_.GetHandleByName(data.entity));

        for (const auto& dataNode : data.nodes)
        {
            auto& subnode = node.GetNodes().emplace_back();
            LoadNode(subnode, dataNode);
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