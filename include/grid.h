#pragma once

#include <vertex.h>

namespace geo{

    class IGrid{
    public:
        IGrid(std::vector<INode>&& vs)
        : vertices{std::move(vs)}, volume(vertices.size()) {}

        virtual ~IGrid() = default;

        virtual INode& operator[](VertexId) = 0;

        virtual const INode& operator[](VertexId) const = 0;

        virtual uint32_t Volume() const = 0;

    protected:
        std::vector<INode> vertices;

        uint32_t volume;
    };
    
}