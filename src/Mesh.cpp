#include "Mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

Mesh::Mesh(float* data, size_t dataSize): m_DataSize(dataSize), m_Stride(0), m_Offset(0){
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    std::cout << "Data Size: " << dataSize << std::endl;
    std::cout << "Constructor Stride: " << m_Stride << std::endl;

    glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Mesh::~Mesh(){
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::addVertexAttribPointer(unsigned int numberOfElements){
    m_AttribSizes.push_back(numberOfElements);
    m_Stride += numberOfElements * sizeof(float);
}

void Mesh::enableVertexAttribPointer(){
    m_Offset = 0;
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    for(int i = 0; i < m_AttribSizes.size(); i++){
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, m_AttribSizes[i], GL_FLOAT, GL_FALSE, m_Stride, (void*)m_Offset);
        m_Offset += m_AttribSizes[i] * sizeof(float);
    }
    m_VertexCount = m_DataSize / m_Stride;
    std::cout << "Strinde: " << m_Stride << std::endl;
    std::cout << "Vertex Count: " << m_VertexCount << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    unbind();
}

unsigned int Mesh::getVertexCount(){
    return m_VertexCount;
}

void Mesh::bind(){
    glBindVertexArray(m_VAO);
}

void Mesh::unbind(){
    glBindVertexArray(0);
}

