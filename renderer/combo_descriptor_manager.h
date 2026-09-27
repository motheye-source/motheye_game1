#pragma once
#include "framework.h"
#include "linear_index_allocator.h"

#include <motheye/dx12/dx12.h>

namespace motheye::renderer
{
    using namespace Microsoft::WRL;

    using motheye::dx12::Device;
    using motheye::dx12::ComboDescriptorHeap;
    using motheye::dx12::Texture;

    class ComboDescriptorManager : public ComboDescriptorHeap
    {
    public:

        ComboDescriptorManager(Device& device, const UINT capacity);

        size_t CreateDescriptor(Device& device, const D3D12_CONSTANT_BUFFER_VIEW_DESC& description);
        size_t CreateDescriptor(Device& device, Texture& texture, const D3D12_SHADER_RESOURCE_VIEW_DESC& description);
        
    private:

        LinearIndexAllocator indexAllocator_;
    };

    inline ComboDescriptorManager::ComboDescriptorManager(Device& device, const UINT capacity) :
        ComboDescriptorHeap(device, capacity),
        indexAllocator_(capacity)
    {
    }

    inline size_t ComboDescriptorManager::CreateDescriptor(Device& device, const D3D12_CONSTANT_BUFFER_VIEW_DESC& description)
    {
        size_t index = indexAllocator_.CreateIndex();
        ComboDescriptorHeap::CreateConstantBufferView(device, index, description);
        return index;
    }

    inline size_t ComboDescriptorManager::CreateDescriptor(Device& device, Texture& texture, const D3D12_SHADER_RESOURCE_VIEW_DESC& description)
    {
        size_t index = indexAllocator_.CreateIndex();
        ComboDescriptorHeap::CreateShaderResourceView(device, index, texture, description);
        return index;
    }

}