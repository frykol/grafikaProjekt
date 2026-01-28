#pragma once

#include <vector>
#include <memory>
#include "EditorLayer.h"

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

class EditorUI{
    private:
        std::vector<std::unique_ptr<EditorLayer>> m_Layers;
    public:
        void init(GLFWwindow* window){
             ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO();
            (void)io;

            ImGui::StyleColorsDark();

            ImGui_ImplGlfw_InitForOpenGL(window, false);
            ImGui_ImplOpenGL3_Init("#version 330 core");

            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
        }

        void newFrame(bool& isFocused){
            m_Layers.clear();
            ImGuiIO& io = ImGui::GetIO();

            if (isFocused) {
            io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
            
            } else {
                io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
            }   

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
        template<typename T, typename... Args>
        void pushLayer(Args&&... args){
            auto p = std::make_unique<T>(std::forward<Args>(args)...);
            m_Layers.emplace_back(std::move(p));
        }
        void render(){
            for(auto& layer : m_Layers){
                layer->draw();
            }
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }
};