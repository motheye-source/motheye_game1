#pragma once
#include "framework.h"
#include <motheye/dx12/dx12.h>

namespace motheye::renderer
{
	using motheye::dx12::StaticMesh;

	using MeshIndexSpan = std::pair<UINT, UINT>;
	using MeshIndexType = UINT16;

	struct LightedVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex;
	};

	struct UnlightedVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	// Note: The enum values will be used as array indices.
	enum class MeshKind : unsigned
	{
		Lighted = 0,
		Unlighted,

		// Must always be last.
		_Size
	};

	constexpr size_t kMeshKindSize = static_cast<size_t>(MeshKind::_Size);

	template<MeshKind Kind>
	struct MeshTraits;

	template<>
	struct MeshTraits<MeshKind::Lighted>
	{
		using VertexType = LightedVertex;
	};

	template<>
	struct MeshTraits<MeshKind::Unlighted>
	{
		using VertexType = UnlightedVertex;
	};

	class MeshResource
	{
	public:

		MeshResource() = default;
		MeshResource(MeshKind kind, std::unique_ptr<StaticMesh>&& mesh, std::vector<MeshIndexSpan>&& spans = {});

		const MeshKind GetKind() const;
		const StaticMesh& GetMesh() const;
		const std::vector<MeshIndexSpan>& GetSpans() const;

	private:

		MeshKind kind_;
		std::unique_ptr<StaticMesh> mesh_;
		std::vector<MeshIndexSpan> spans_;
	};

	inline MeshResource::MeshResource(MeshKind kind, std::unique_ptr<StaticMesh>&& mesh, std::vector<MeshIndexSpan>&& spans) :
		kind_(kind),
		mesh_(std::move(mesh)),
		spans_(std::move(spans))
	{
	}

	inline const MeshKind MeshResource::GetKind() const
	{
		return this->kind_;
	}
	
	inline const StaticMesh& MeshResource::GetMesh() const
	{
		return *this->mesh_;
	}

	inline const std::vector<MeshIndexSpan>& MeshResource::GetSpans() const
	{
		return this->spans_;
	}
}