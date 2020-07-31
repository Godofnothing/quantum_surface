#pragma once

#include <numeric>

#include <vertex.h>
#include <vector.h>    

namespace geo{

    template <typename T>
    class Sphere{
    public:
        Sphere(uint32_t lon_steps, uint32_t lat_steps);

        std::vector<Vector<T, 3>>& Points();

        std::vector<INode>& Vertices();
    
    private:
        uint32_t lon_steps;

        uint32_t lat_steps;

        std::vector<Vector<T, 3>> points;

        std::vector<INode>        vertices;
    };

    template <typename T>
    Sphere<T>::Sphere(uint32_t lon_steps, uint32_t lat_steps)
    : lon_steps(lon_steps), lat_steps(lat_steps){

        vertices.resize(lon_steps * lat_steps + 2);
        points.reserve(lon_steps * lat_steps + 2);

        //Initialization of the North pole
        INode& north_pole = vertices.front();
        north_pole.resize(lon_steps);
        std::iota(north_pole.begin(), north_pole.end(), 1);

        //Initialization of the South pole
        INode& south_pole = vertices.back();
        south_pole.resize(lon_steps);
        std::iota(south_pole.begin(), south_pole.end(), (lat_steps - 1) * lon_steps + 1);

        //Initialization of the closest to North pole layer
        for(uint32_t idx = 0; idx < lon_steps; idx++){
            vertices[idx + 1] = {
                0,
                1 + (idx + 1) % lon_steps,
                1 + idx + lon_steps,
                1 + (idx + lon_steps - 1) % lon_steps + lon_steps,
                1 + (idx + lon_steps - 1) % lon_steps
            };
        }

        //Initialization of the intermediate layers
        for(uint32_t x = 1; x < lat_steps - 1; x++){
            uint32_t idx_offset = x * lon_steps + 1;
            for(uint32_t y = 0; y < lon_steps; y++){
                vertices[idx_offset + y] = {
                    idx_offset + y - lon_steps,
                    idx_offset + (y + 1) % lon_steps - lon_steps,
                    idx_offset + (y + 1) % lon_steps,
                    idx_offset + y + lon_steps,
                    idx_offset + (y + lon_steps - 1) % lon_steps + lon_steps,
                    idx_offset + (y + lon_steps - 1) % lon_steps
                };
            }
        }

        //Initialization of the closest to South pole layer
        if(lat_steps > 1){
            uint32_t idx_offset = (lat_steps - 1) * lon_steps + 1;
            for(uint32_t y = 0; y < lon_steps; y++){
                vertices[idx_offset + y] = {
                    idx_offset + y - lon_steps,
                    idx_offset + (y + 1) % lon_steps - lon_steps,
                    idx_offset + (y + 1) % lon_steps,
                    lon_steps * lat_steps + 1,
                    idx_offset + (y + lon_steps - 1) % lon_steps,
                };
            }
        }

        //Initialization of Sphere coordinates
        points.push_back({0, 0, 1});

        for(uint32_t x = 0; x < lat_steps; x++){
            T theta = M_PI * (x + 1) / (lat_steps + 2);
            for(uint32_t y = 0; y < lon_steps; y++){
                T phi = M_PI * (2 * y + x) / lon_steps;
                points.push_back({
                    std::sin(theta) * std::cos(phi),
                    std::sin(theta) * std::sin(phi),
                    std::cos(theta)
                });
            }
        }

        points.push_back({0, 0, -1});
    }

    template <typename T>
    std::vector<Vector<T, 3>>& Sphere<T>::Points(){
        return points;
    }

    template <typename T>
    std::vector<INode>& Sphere<T>::Vertices(){
        return vertices;
    }

}