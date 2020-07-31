#pragma once

#include <cstdio>
#include <string>
#include <mesh.h>

namespace geo{

    class Writer{
    public:

        Writer(std::string save_dir) : save_dir(std::move(save_dir)) {};

        template <typename T>
        void WriteMesh(std::string file_name, const Mesh<T>& mesh){
            std::string full_path = save_dir + "/" + file_name;
            FILE* file = fopen(full_path.c_str(), "wb");
            fwrite(mesh.Points().data(), sizeof(T), 3 * mesh.Volume(), file);
            fclose(file);
        }

    private:
        std::string save_dir;
    };

}