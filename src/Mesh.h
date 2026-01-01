#pragma once

#include <cstddef>
#include <string>
#include <vector>

//Only float data
class Mesh{
    private:
        unsigned int m_VAO;
        unsigned int m_VBO;
        unsigned int m_VertexCount;
        size_t m_DataSize;

        size_t m_Offset;
        unsigned int m_Stride;


        std::vector<int> m_AttribSizes;
    public:
        Mesh(float* data, size_t dataSize);
        ~Mesh();

        void addVertexAttribPointer(unsigned int numberOfElements);
        void enableVertexAttribPointer();

        void bind();
        void unbind();

        unsigned int getVertexCount();

};