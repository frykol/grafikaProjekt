#pragma once

#include "Texture.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

enum class ShaderID{
    Geometry = 0,
    Light,
};

struct Material{
    ShaderID shader; 
    Texture* texture;
    float shinines;
};

class Object{
    private:
        std::string m_Name;

        glm::vec3 m_Position;

        Object* m_Parent = nullptr;

        Mesh* m_Mesh;
        Material m_Material;

        glm::vec3 m_Rotation;
        float m_Scale;

        glm::mat4 m_WorldMatrix;
        glm::mat4 m_LocalMatrix;

    public:
        Object(const std::string& name, Mesh* mesh, Material material, glm::vec3 position = glm::vec3(0,0,0), glm::vec3 rotation = glm::vec3(0,0,0)): m_Name(name), m_Mesh(mesh), m_Material(material), m_Position(position), m_Rotation(rotation){
           // m_Rotation = glm::vec3(0,0,0);
            m_Scale = 1.0f;
        }

        void setPosition(glm::vec3 position) {m_Position = position;}
        glm::vec3& getPosition() {return m_Position;}

        void setParent(Object* parent) {m_Parent = parent; }
        Object* getParent() { return m_Parent; }

        void setRotation(glm::vec3 rotation) { m_Rotation = rotation; }
        glm::vec3& getRotation() { return m_Rotation; }

        void setScale(float scale) { m_Scale = scale; }
        float& getScale() { return m_Scale; }

        std::string getName(){
            return m_Name;
        }

        Mesh* getMesh(){ return m_Mesh; }
        Material& getMaterial(){ return m_Material; }

        void setPositionToWorldMatrix(){
            m_Position = worldMatrix()[3];
        }

        glm::mat4 localMatrix(){
            glm::mat4 m(1.0f);
            m = glm::translate(m, m_Position);
            m = glm::rotate(m, glm::radians(m_Rotation.x), glm::vec3(1,0,0));
            m = glm::rotate(m, glm::radians(m_Rotation.y), glm::vec3(0,1,0));
            m = glm::rotate(m, glm::radians(m_Rotation.z), glm::vec3(0,0,1));
            m = glm::scale(m, glm::vec3(m_Scale));
            return m;
        }

        glm::mat4 worldMatrix(){
            return m_Parent ? m_Parent->worldMatrix() * localMatrix() : localMatrix(); 
        }

};