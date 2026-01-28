#pragma once

#include "EditorLayer.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include "../Object.h"

class GeneralLayer : public EditorLayer{
    private:
        glm::vec3& m_CameraPos;
        std::vector<Object>& m_Lights;
    public:

        GeneralLayer(glm::vec3& cameraPos, std::vector<Object>& lights): m_CameraPos(cameraPos), m_Lights(lights) {}

        void draw() override{
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Begin("Ogólne");
            ImGui::Text("Camera Position: %.2f, %.2f, %.2f", m_CameraPos.x, m_CameraPos.y, m_CameraPos.z);
            glm::vec3& lightPos = m_Lights[0].getPosition();
            ImGui::SliderFloat3("Light Position", &lightPos[0], -10, 20, "%.1f");
            // ImGui::SliderFloat("Shinines", &m_Objects[0].getMaterial().shinines, 1, 300, "%.0f");
            // ImGui::SliderFloat("Angle", &m_Angle, 360, 0, "%.0f");
            // ImGui::SliderFloat("Angle Arm", &m_ArmArmAngle, 360, 0, "%.0f");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

};