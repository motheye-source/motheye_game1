#pragma once
#include "framework.h"

namespace motheye::renderer
{
	class LinearIndexAllocator
	{
	public:

		LinearIndexAllocator(size_t size);

		size_t GetSize() const;
		size_t CreateIndex();

	private:
		size_t next_{ 0 };
		size_t size_;
	};

	inline LinearIndexAllocator::LinearIndexAllocator(size_t size) :
		size_(size)
	{
	}

	inline size_t LinearIndexAllocator::GetSize() const
	{
		return size_;
	}

	inline size_t LinearIndexAllocator::CreateIndex()
	{
		if (next_ >= size_)
		{
			throw std::out_of_range("Index allocator size exceeded");
		}
		return next_++;
	}
}