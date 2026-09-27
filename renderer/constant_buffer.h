#pragma once
#include "framework.h"
#include "combo_descriptor_manager.h"
#include "linear_index_allocator.h"

#include <motheye/dx12/dx12.h>
#include <motheye/dx12/constant_buffer.h>

namespace motheye::renderer
{    
    using namespace Microsoft::WRL;

    using motheye::dx12::Device;

    struct Constant
    {
        int bufferIndex;
        int descriptorIndex;
    };

    template <typename T>
    class ConstantBuffer : public motheye::dx12::ConstantBuffer<T>
    {
    public:

        ConstantBuffer(Device& device, UINT capacity);

        Constant CreateConstant(Device& device, ComboDescriptorManager& heap);

    private:
        
        LinearIndexAllocator indexAllocator_;
    };

    template<typename T>
    inline ConstantBuffer<T>::ConstantBuffer(Device& device, UINT capacity) :
        motheye::dx12::ConstantBuffer<T>(device, capacity),
        indexAllocator_(capacity)
    {
    }
 
    template <typename T>
    inline Constant ConstantBuffer<T>::CreateConstant(Device& device, ComboDescriptorManager& heap)
    {    
        Constant constant;
        constant.bufferIndex = indexAllocator_.CreateIndex();

        auto cbvDesc = this->CreateViewDescription(constant.bufferIndex);
        constant.descriptorIndex = heap.CreateDescriptor(device, cbvDesc);
        
        return constant;
    }
}