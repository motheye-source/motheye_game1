#pragma once
#include "world_node.h"

#include <vector>

namespace motheye::engine::world
{
    namespace data = motheye::model;

    class World
    {
    public:
        const std::vector<WorldNode>& GetRoot() const;
        std::vector<WorldNode>& GetRoot();

    private:
        std::vector<WorldNode> root_;
    };

    inline const std::vector<WorldNode>& World::GetRoot() const
    {
        return root_;
    }

    inline std::vector<WorldNode>& World::GetRoot()
    {
        return root_;
    }
}
