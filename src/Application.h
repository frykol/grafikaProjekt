#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

#include <vector>

#include <glm/glm.hpp>

struct KeyEvent{
    int key, action, mods;
};

struct WinSize{
    int width;
    int height;
};

class Application{
    private:
        GLFWwindow* m_Window;
        WinSize m_WinSize;
        std::string m_Name;

        Mesh* m_TestMesh;
        glm::mat4 m_Model;

        Camera m_Camera;

        Shader* m_Shader;

        bool m_Keys[GLFW_KEY_LAST + 1]{};

        float m_LastTime;
        double m_LastX = 0.0;
        double m_LastY = 0.0;
        bool m_FirstMouse = true;

        void init();
        void update(float dt);
        void render();
        void onKey(int key, int scancode, int action, int mods);
        void onMouseMove(double x, double y);
    public:
        Application(int width, int height, const std::string& name);
        void run();
};