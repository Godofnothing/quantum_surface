#pragma once

#include <grid.h>

namespace geo{

    template <size_t DIM>
    using GridVec = std::array<Coord, DIM>;

    constexpr bool FW = 1;
    constexpr bool BW = 0;

    template <size_t DIM>
    class SquareGrid : public IGrid{
    public:
        explicit SquareGrid(GridVec<DIM>&& sizes);

        Vertex& operator[](VertexId vid) override{
            return vertices[vid];
        }

        const Vertex& operator[](VertexId vid) const override{
            return vertices[vid];
        }

        uint32_t Volume() const override{
            return vertices.size();
        }


    private:
        GridVec<DIM> GetVec(VertexId vid);

        VertexId GetId(const GridVec<DIM>& v);

        template<bool BF>
        VertexId GetNeighborId(const GridVec<DIM>& v, size_t mu);

        GridVec<DIM> sizes;

        std::array<uint32_t, DIM> size_products;
    };

    template <size_t DIM>
    SquareGrid<DIM>::SquareGrid(GridVec<DIM>&& sizes) : sizes(std::move(sizes)){
        size_products[DIM - 1] = 1;
        for(size_t i = 1; i < DIM; i++){
            size_products[DIM - i - 1] = sizes[DIM - i] * size_products[DIM - i];
        }

        volume = size_products[0] * sizes[0];

        vertices.resize(volume);   

        for(size_t vid = 0; vid < volume; vid++){
            GridVec<DIM> grid_vec = GetVec(vid);
            for(size_t mu = 0; mu < DIM; mu++){
                vertices[vid].push_back(GetNeighborId<FW>(grid_vec, mu));
                vertices[vid].push_back(GetNeighborId<BW>(grid_vec, mu));
            }
        }
    }

    template <size_t DIM>
    GridVec<DIM> SquareGrid<DIM>::GetVec(VertexId vid){
        GridVec<DIM> vec;
        for(size_t i = 0; i < DIM; i++){
            vec[i] = vid / size_products[i];
            vid   %= size_products[i];
        }
        return vec;
    }

    template <size_t DIM>
    VertexId SquareGrid<DIM>::GetId(const GridVec<DIM>& v){
        VertexId id = 0;
        for(size_t i = 0; i < DIM; i++){
            id += v[i] * size_products[i]; 
        }
        return id;
    }

    template <size_t DIM>
    template <bool BF>
    VertexId SquareGrid<DIM>::GetNeighborId(const GridVec<DIM>& v, size_t mu){
        Coord shifted = (BF) ? (v[mu] + 1) % sizes[mu] 
                             : (v[mu] + sizes[mu] - 1) % sizes[mu];

        return GetId(v) + (shifted - v[mu]) * size_products[mu]; 
    }
    
}