#pragma once
#include <motheye/model/model.h>
#include <DirectXMath.h>

namespace motheye::engine::world
{
	struct Transform
	{
		Transform();
		Transform(const motheye::model::Transform& data);
		
		void Reset();
		void SetRotationFromEuler(DirectX::XMFLOAT4X4& result) const;
		void ToMatrix(DirectX::XMFLOAT4X4& result) const;
		DirectX::XMMATRIX ToXMMatrix() const;
		
		float sx, sy, sz;
		float rx, ry, rz, rw;
		float x, y, z;
	};

	inline Transform::Transform()
	{
		Reset();
	}

	inline Transform::Transform(const motheye::model::Transform& data)
	{
		x = data.x;
		y = data.y;
		z = data.z;

		rx = data.rx;
		ry = data.ry;
		rz = data.rz;
		rw = (data.rotationKind == motheye::model::RotationKind::kQuaternion) ? data.rw : 1.0;

		sx = data.sx;
		sy = data.sy;
		sz = data.sz;
	}

	inline void Transform::Reset()
	{
		x = y = z = 0.0f;
		rx = ry = rz = 0.0f; rw = 1.0f;
		sx = sy = sz = 1.0f;
	}	
}