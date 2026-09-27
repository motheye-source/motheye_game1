#pragma once
#include "framework.h"
#include "resource_handle.h"

namespace renderer
{	
	class SolidResource
	{
	public:

		SolidResource() = default;
		SolidResource(MeshResourceHandle mesh);
		SolidResource(MeshResourceHandle mesh, MaterialResourceHandle material);
		SolidResource(MeshResourceHandle mesh, std::vector<MaterialResourceHandle>&& materials);

		const MeshResourceHandle GetMesh() const;
		const std::vector<MaterialResourceHandle>& GetMaterials() const;

	private:

		MeshResourceHandle mesh_;
		std::vector<MaterialResourceHandle> materials_;
	};

	inline SolidResource::SolidResource(MeshResourceHandle mesh) :
		mesh_(mesh)
	{
	}

	inline SolidResource::SolidResource(MeshResourceHandle mesh, MaterialResourceHandle material) :
		mesh_(mesh),
		materials_{ material }
	{
	}

	inline SolidResource::SolidResource(MeshResourceHandle mesh, std::vector<MaterialResourceHandle>&& materials) :
		mesh_(mesh),
		materials_(std::move(materials))
	{
	}

	inline const MeshResourceHandle SolidResource::GetMesh() const
	{
		return this->mesh_;
	}

	inline const std::vector<MaterialResourceHandle>& SolidResource::GetMaterials() const
	{
		return this->materials_;
	}
}