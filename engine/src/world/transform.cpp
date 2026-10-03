#include <motheye/engine/world/transform.h>
#include <motheye/math/math.h>

using motheye::math::DegreesToRadians;

namespace motheye::engine::world
{
	void Transform::SetRotationFromEuler(DirectX::XMFLOAT4X4& result) const
	{
		// Pitch (about right/X axis)
		float Cp = cosf(DegreesToRadians(rx));
		float Sp = sinf(DegreesToRadians(rx));

		// Yaw (about up/Z axis)
		float Cr = cosf(DegreesToRadians(rz));
		float Sr = sinf(DegreesToRadians(rz));

		// Roll (about forward/Y axis)
		float Cy = cosf(DegreesToRadians(ry));
		float Sy = sinf(DegreesToRadians(ry));

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

	void Transform::ToMatrix(DirectX::XMFLOAT4X4& result) const
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

	DirectX::XMMATRIX Transform::ToXMMatrix() const
	{
		DirectX::XMFLOAT4X4 result;
		ToMatrix(result);
		return DirectX::XMLoadFloat4x4(&result);
}
}