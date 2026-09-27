#pragma once
#include "renderer/renderer.h"

namespace motheye::engine::world
{
    using motheye::renderer::UnlightedVertex;

    struct AxesGeometry
    {
        static const std::vector<UnlightedVertex> vertices;
        static const std::vector<UINT16> indices;
    };

    inline const std::vector<UnlightedVertex> AxesGeometry::vertices =
    {
        UnlightedVertex({ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }),
        UnlightedVertex({ DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }),
        UnlightedVertex({ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }),
        UnlightedVertex({ DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }),
        UnlightedVertex({ DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }),
        UnlightedVertex({ DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) })
    };

    inline const std::vector<UINT16> AxesGeometry::indices =
    {
        0, 1,
        2, 3,
        4, 5,
    };
}
