#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera{
    private:
        glm::vec3 m_Position{0.0f, 0.0f, 3.0f};
        glm::vec3 m_WorldUp{0.0f, 1.0f, 0.0f};
        

        glm::vec3 m_CameraFront{0.0f, 0.0f, -1.0f};
        glm::vec3 m_CameraRight{1.0f, 0.0f, 0.0f};
        glm::vec3 m_CameraUp{0.0f, 1.0f, 0.0f};

        float yaw = -90.0f;
        float pitch = 0.0f;
        
        float fov = 60.0f;
        float nearZ = 0.1f;
        float farZ = 100.0f;

        float moveSpeed = 3.0f;
        float mouseSens = 0.12f;

        void updateVectors(){
            glm::vec3 f;
            f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            f.y = sin(glm::radians(pitch));
            f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            m_CameraFront = glm::normalize(f);

            m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
            m_CameraUp    = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
        }
    public:
        Camera() {updateVectors(); }

        glm::mat4 getProjection(float aspectRatio){
            return glm::perspective(glm::radians(fov), aspectRatio, nearZ, farZ);
        }
        glm::mat4 getView(){
            return glm::lookAt(m_Position, m_Position + m_CameraFront, m_CameraUp);
        }

        glm::vec3& getPosition(){
            return m_Position;
        }


        void look(float dx, float dy) {
            yaw   += dx * mouseSens;
            pitch += dy * mouseSens;

            pitch = std::clamp(pitch, -89.0f, 89.0f);
            updateVectors();
        }

        void fastMove(){
            moveSpeed = 8.0f;
        }
        void normalMove(){
            moveSpeed = 3.0f;
        }

        void moveForward(float dt){
            m_Position += m_CameraFront * (moveSpeed * dt);
        }
        void moveBackward(float dt){
            m_Position -= m_CameraFront * (moveSpeed * dt);
        }
        void moveLeft(float dt){
            m_Position -= m_CameraRight * (moveSpeed * dt);
        }
        void moveRight(float dt){
            m_Position += m_CameraRight * (moveSpeed * dt);
        }
        void moveUp(float dt){
            m_Position += m_WorldUp * (moveSpeed * dt);
        }  
        void moveDown(float dt){
            m_Position -= m_WorldUp * (moveSpeed * dt);
        }

};