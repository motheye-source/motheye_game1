#pragma once
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <format>

#include <renderer/resource_handle.h>

namespace motheye::engine::world
{
	using namespace motheye::renderer;

	class ResourceMap
	{
	public:

		TextureResourceHandle GetTexture(const std::string& name) const
		{
			return Get(name, textures_);
		}

		MaterialResourceHandle GetMaterial(const std::string& name) const
		{
			return Get(name, materials_);
		}

		MeshResourceHandle GetMesh(const std::string& name) const
		{
			return Get(name, meshes_);
		}

		SolidResourceHandle GetSolid(const std::string& name) const
		{
			return Get(name, solids_);
		}

		CameraResourceHandle GetCamera(const std::string& name) const
		{
			return Get(name, cameras_);
		}

		LightResourceHandle GetLight(const std::string& name) const
		{
			return Get(name, lights_);
		}

		void AddTexture(const std::string& name, TextureResourceHandle handle)
		{
			textures_.emplace(name, handle);
		}

		void AddMaterial(const std::string& name, MaterialResourceHandle handle)
		{
			materials_.emplace(name, handle);
		}

		void AddMesh(const std::string& name, MeshResourceHandle handle)
		{
			meshes_.emplace(name, handle);
		}

		void AddSolid (const std::string& name, SolidResourceHandle handle)
		{
			solids_.emplace(name, handle);
		}

		void AddCamera(const std::string& name, CameraResourceHandle handle)
		{
			cameras_.emplace(name, handle);
		}

		void AddLight(const std::string& name, LightResourceHandle handle)
		{
			lights_.emplace(name, handle);
		}

	private:

		template<typename T>
		T Get(const std::string& name, const std::unordered_map<std::string, T>& map) const
		{
			const auto& it = map.find(name);
			if (it == map.end())
			{
				throw std::runtime_error(std::format("Resource name ({}) not found", name));
			}
			return it->second;
		}

	private:

		std::unordered_map<std::string, TextureResourceHandle> textures_;
		std::unordered_map<std::string, MaterialResourceHandle> materials_;
		std::unordered_map<std::string, MeshResourceHandle> meshes_;
		std::unordered_map<std::string, SolidResourceHandle> solids_;
		std::unordered_map<std::string, LightResourceHandle> lights_;
		std::unordered_map<std::string, CameraResourceHandle> cameras_;
	};
}