#pragma once

#include <vertex.h>

namespace geo{

    void Vertex::AddNeighbor(VertexId vid){
        neighbors.push_back(vid);
    }

    const AdjList& Vertex::NeighborHood() const{
        return neighbors;
    }

    AdjList& Vertex::NeighborHood(){
        return neighbors;
    }

    uint32_t Vertex::NumNeighbors() const{
        return neighbors.size();
    }

}