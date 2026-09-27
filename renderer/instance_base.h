#pragma once
#include "framework.h"
#include "constant_buffer.h"

namespace renderer
{
	class InstanceBase
	{
	public:

		InstanceBase() = default;
		InstanceBase(Constant constant);

		Constant GetConstant() const;

		const DirectX::XMMATRIX& GetWorldMatrix() const;
		void SetWorldMatrix(const DirectX::XMMATRIX& matrix);

	private:

		Constant constant_{};
		DirectX::XMMATRIX worldMatrix_ = DirectX::XMMatrixIdentity();
	};

	inline InstanceBase::InstanceBase(Constant constant) :
		constant_(constant)
	{
	}

	inline Constant InstanceBase::GetConstant() const
	{
		return this->constant_;
	}

	inline const DirectX::XMMATRIX& InstanceBase::GetWorldMatrix() const
	{
		return this->worldMatrix_;
	}

	inline void InstanceBase::SetWorldMatrix(const DirectX::XMMATRIX& matrix)
	{
		this->worldMatrix_ = matrix;
	}
}