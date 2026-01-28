#pragma once

#include "EditorLayer.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <vector>
#include "../Object.h"

class ObjectLayer : public EditorLayer{
    private:
        std::vector<Object>& m_Objects;
        glm::vec3 m_CameraPosition;
    public:

        ObjectLayer(std::vector<Object>& objects, glm::vec3& cameraPosition):m_Objects(objects), m_CameraPosition(cameraPosition) {}

        void draw() override{
            ImGuiIO& io = ImGui::GetIO(); 
            ImGui::Begin("Obiekty");                        
            for(Object& object : m_Objects){
                if(ImGui::TreeNode(object.getName().c_str())){
                    glm::vec3& objectPosition = object.getPosition();
                    ImGui::Text("Position: %.2f, %.2f, %.2f", objectPosition.x, objectPosition.y, objectPosition.z);
                    ImGui::SliderFloat3("Set Position", &objectPosition[0], -10, 10, "%.1f");
                    if(ImGui::Button("To camera")){
                        Object* objParent = object.getParent();
                        if(objParent != nullptr){
                            object.setParent(nullptr);
                        }
                        objectPosition = m_CameraPosition;
                        object.setParent(objParent);
                    }
                    glm::vec3& objectRotation = object.getRotation();
                    ImGui::Text("Rotation: %.0f, %.0f, %.0f", objectRotation.x, objectRotation.y, objectRotation.z);
                    ImGui::SliderFloat3("Set Rotation", &objectRotation[0], -360, 360, "%.0f");
                    Material& objectMaterial = object.getMaterial();
                    ImGui::Text("Shinines: %.0f", objectMaterial.shinines);
                    ImGui::SliderFloat("Set Shinies", &objectMaterial.shinines, 1, 512, "%.0f");
                    ImGui::TreePop();
                }
            }
            ImGui::End();
        }

};