#pragma once

#include <cassert>

#include <grid.h>
#include <vector.h>

namespace geo{

    template <typename T>
    class Mesh : public IGrid{
    public:
        using Vec3D = Vector<T, 3>;

        Mesh() {}

        Mesh(std::vector<INode>&& vs, std::vector<Vec3D>&& ps) 
        : IGrid{std::move(vs)}, points{ps} {}

        INode& operator[](VertexId) override;

        const INode& operator[](VertexId) const override;

        Vec3D& Point(VertexId);

        const Vec3D& Point(VertexId) const;

        std::vector<Vec3D>& Points();

        const std::vector<Vec3D>& Points() const;

        uint32_t Volume() const override;

        T Area() const;

        T AdjacentArea(VertexId id) const;

        Vec3D AreaGrad(VertexId id) const;

    private:
        std::vector<Vec3D> points;
    };

    template <typename T>
    INode& Mesh<T>::operator[](VertexId vid){
        return vertices[vid];
    }

    template <typename T>
    const INode& Mesh<T>::operator[](VertexId vid) const{
        return vertices[vid];
    }

    template <typename T>
    Vector<T, 3>& Mesh<T>::Point(VertexId vid){
        return points[vid];
    }

    template <typename T>
    const Vector<T, 3>& Mesh<T>::Point(VertexId vid) const{
        return points[vid];
    }

    template <typename T>
    std::vector<Vector<T, 3>>& Mesh<T>::Points(){
        return points;     
    }

    template <typename T>
    const std::vector<Vector<T, 3>>& Mesh<T>::Points() const{
        return points;     
    }

    template <typename T>
    uint32_t Mesh<T>::Volume() const{
        return volume;
    }

    template <typename T>
    T Mesh<T>::AdjacentArea(VertexId id) const{
        const INode& node = vertices[id];
        size_t num_neighbors = node.size();

        assert(num_neighbors > 2);

        Vector<T, 3> r_a, r_b;

        r_a = points[id] -  points[node[num_neighbors - 1]];
        r_b = points[id] -  points[node[0]];
        T area = triangle_area(r_a, r_b);

        for(size_t i = 0; i < num_neighbors - 1; i++){
            r_a = r_b;
            r_b = points[id] -  points[node[i + 1]];
            area += triangle_area(r_a, r_b);
        }

        return area;
    }

    template <typename T>
    T Mesh<T>::Area() const{
        T area = 0;
        for(size_t idx = 0; idx < vertices.size(); idx++){
            area += AdjacentArea(idx);
        }
        return area / 3;
    }

    template <typename T>
    Vector<T, 3> Mesh<T>::AreaGrad(VertexId id) const{
        const INode& node = vertices[id];
        size_t num_neighbors = node.size();

        assert(num_neighbors > 2);

        Vector<T, 3> r_a, r_b, r_c;

        r_a = points[id]      -  points[node[num_neighbors - 1]];
        r_b = points[id]      -  points[node[0]];
        r_c = points[node[0]] -  points[node[num_neighbors - 1]];
        Vector<T, 3> grad = (r_a * cos(r_a, r_c) - r_b * cos(r_b, r_c)).normalize() * r_c.norm() / 2;

        for(size_t i = 0; i < num_neighbors - 1; i++){
            r_a = r_b;
            r_b = points[id] - points[node[i+1]];
            r_c = points[node[i+1]] - points[node[i]];
            grad += (r_a * cos(r_a, r_c) - r_b * cos(r_b, r_c)).normalize() * r_c.norm() / 2;
        }

        return grad;
    }
    
}