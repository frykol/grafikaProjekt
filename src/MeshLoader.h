#pragma once
#include "Mesh.h"
#include <string>

class MeshLoader{
    private:


    public:
        static Mesh* loadObj(const std::string& path);
};