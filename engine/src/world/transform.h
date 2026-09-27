#pragma once
#include <motheye/model/model.h>
#include <motheye/math/math.h>
#include <DirectXMath.h>

#define DEGTORAD(a) (((a) * 2.0f * motheye::math::PI) / 360.0f)

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

	inline void Transform::SetRotationFromEuler(DirectX::XMFLOAT4X4& result) const
	{
		// Pitch (about right/X axis)
		float Cp = cosf(DEGTORAD(rx));
		float Sp = sinf(DEGTORAD(rx));

		// Yaw (about up/Z axis)
		float Cr = cosf(DEGTORAD(rz));
		float Sr = sinf(DEGTORAD(rz));

		// Roll (about forward/Y axis)
		float Cy = cosf(DEGTORAD(ry));
		float Sy = sinf(DEGTORAD(ry));

		result._11 *= (Cr * Cy);
		result._12 *= (Sr);
		result._13 *= -(Cr * Sy);

		result._21 *= -(Cp * Sr * Cy) + (Sp * Sy);
		result._22 *= (Cp * Cr);
		result._23 *= (Cp * Sr * Sy) + (Sp * Cy);

		result._31 *= (Sp * Sr * Cy) + (Cp * Sy);
		result._32 *= -(Sp * Cr);
		result._33 *= -(Sp * Sr * Sy) + (Cp * Cy);
	}

	inline void Transform::ToMatrix(DirectX::XMFLOAT4X4& result) const
	{
		// M = SRT

		// Scale
		result._11 = sx;
		result._12 = sx;
		result._13 = sx;

		result._21 = sy;
		result._22 = sy;
		result._23 = sy;

		result._31 = sz;
		result._32 = sz;
		result._33 = sz;

		// Rotation
		SetRotationFromEuler(result);

		// Translation
		result._41 = x;
		result._42 = y;
		result._43 = z;

		// Non-homogenous fill
		result._14 = 0.0f;
		result._24 = 0.0f;
		result._34 = 0.0f;
		result._44 = 1.0f;
	}

	inline DirectX::XMMATRIX Transform::ToXMMatrix() const
	{
		DirectX::XMFLOAT4X4 result;
		ToMatrix(result);
		return DirectX::XMLoadFloat4x4(&result);
	}
}