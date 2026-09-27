#pragma once
#include "resource_handle.h"

#include "camera_resource.h"
#include "light_resource.h"
#include "material_resource.h"
#include "mesh_resource.h"
#include "solid_resource.h"
#include "texture_resource.h"

#include "combo_descriptor_manager.h"
#include "constant_manager.h"

#include <type_traits>

namespace motheye::renderer
{
	class ResourceManager
	{
	public:

		ResourceManager(ComboDescriptorManager& descriptorManager, ConstantManager& constantManager);

		template<typename THandle>
		auto& GetResource(THandle handle);

		std::vector<MaterialResource>& GetMaterials();

		CameraResourceHandle CreateCamera(float fov, float nearClip, float farClip);
		LightResourceHandle CreateLight(const DirectX::XMFLOAT4& diffuseColor);
		MaterialResourceHandle CreateMaterial(const DirectX::XMFLOAT4& baseColor, TextureResourceHandle texture);
		SolidResourceHandle CreateSolid(MeshResourceHandle mesh, std::vector<MaterialResourceHandle>&& materials = {});
		TextureResourceHandle CreateTexture(motheye::dx12::CommandQueue& commandQueue, const std::string& textureFilePath);

		template<MeshKind Kind>
		MeshResourceHandle CreateMesh(
			motheye::dx12::CommandQueue& commandQueue,
			const std::vector<typename MeshTraits<Kind>::VertexType>& vertices, 
			const std::vector<MeshIndexType>& indices, 
			std::vector<MeshIndexSpan>&& spans = {});

	private:

		ComboDescriptorManager& descriptorManager_;
		ConstantManager& constantManager_;

		std::vector<CameraResource> cameras_;
		std::vector<LightResource> lights_;
		std::vector<MaterialResource> materials_;
		std::vector<MeshResource> meshes_;
		std::vector<SolidResource> solids_;
		std::vector<TextureResource> textures_;
	};

	inline ResourceManager::ResourceManager(ComboDescriptorManager& descriptorManager, ConstantManager& constantManager) :
		descriptorManager_(descriptorManager),
		constantManager_(constantManager)
	{
	}

	template<typename THandle>
	inline auto& ResourceManager::GetResource(THandle handle)
	{
		if constexpr (std::is_same_v<THandle, CameraResourceHandle>)
		{
			return cameras_[handle.GetValue()];
		}
		else if constexpr (std::is_same_v<THandle, LightResourceHandle>)
		{
			return lights_[handle.GetValue()];
		}
		else if constexpr (std::is_same_v<THandle, MaterialResourceHandle>)
		{
			return materials_[handle.GetValue()];
		}
		else if constexpr (std::is_same_v<THandle, MeshResourceHandle>)
		{
			return meshes_[handle.GetValue()];
		}
		else if constexpr (std::is_same_v<THandle, SolidResourceHandle>)
		{
			return solids_[handle.GetValue()];
		}
		else if constexpr (std::is_same_v<THandle, TextureResourceHandle>)
		{
			return textures_[handle.GetValue()];
		}
		else
		{
			static_assert(sizeof(THandle) == 0, "Unsupported resource handle type for GetResource");
		}
	}

	inline std::vector<MaterialResource>& ResourceManager::GetMaterials()
	{
		return materials_;
	}

	inline CameraResourceHandle ResourceManager::CreateCamera(float fov, float nearClip, float farClip)
	{
		cameras_.emplace_back(fov, nearClip, farClip);
		return CameraResourceHandle(cameras_.size() - 1);
	}
	
	inline LightResourceHandle ResourceManager::CreateLight(const DirectX::XMFLOAT4& diffuseColor)
	{
		lights_.emplace_back(diffuseColor);
		return LightResourceHandle(lights_.size() - 1);
	}

	inline MaterialResourceHandle ResourceManager::CreateMaterial(const DirectX::XMFLOAT4& baseColor, TextureResourceHandle texture)
	{
		assert(texture.GetValue() != ResourceHandle::InvalidValue); // For now, a texture is required.
		Constant constant = constantManager_.CreateMaterialResourceConstant();
		materials_.emplace_back(baseColor, texture, constant);
		return MaterialResourceHandle(materials_.size() - 1);
	}

	template<MeshKind Kind>
	inline MeshResourceHandle ResourceManager::CreateMesh(
		motheye::dx12::CommandQueue& commandQueue,
		const std::vector<typename MeshTraits<Kind>::VertexType>& vertices, 
		const std::vector<MeshIndexType>& indices, 
		std::vector<MeshIndexSpan>&& spans)
	{
		auto mesh = motheye::dx12::StaticMesh::Create(commandQueue, vertices.data(), vertices.size(), indices.data(), indices.size());
		meshes_.emplace_back(Kind, std::move(mesh), std::move(spans));
		return MeshResourceHandle(meshes_.size() - 1);
	}

	inline SolidResourceHandle ResourceManager::CreateSolid(MeshResourceHandle mesh, std::vector<MaterialResourceHandle>&& materials)
	{
		// TODO:  This could/should be an exception if false.
		assert(meshes_[mesh.GetValue()].GetSpans().size() == materials.size());
		solids_.emplace_back(mesh, std::move(materials));
		return SolidResourceHandle(solids_.size() - 1);
	}

	inline TextureResourceHandle ResourceManager::CreateTexture(motheye::dx12::CommandQueue& commandQueue, const std::string& textureFilePath)
	{
		std::unique_ptr<motheye::dx12::Texture> texture = motheye::dx12::Texture::CreateDDSTextureFromFile(commandQueue, textureFilePath);
		if (texture != nullptr)
		{
			const auto description = texture->CreateViewDescription();
			int descriptorIndex = descriptorManager_.CreateDescriptor(commandQueue.GetDevice(), *texture, description);
			textures_.emplace_back(std::move(texture), descriptorIndex);
			return TextureResourceHandle(textures_.size() - 1);
		}
		return TextureResourceHandle();
	}
}

