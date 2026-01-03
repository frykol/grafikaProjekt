#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Object.h"

#include <vector>

#include <glm/glm.hpp>

struct KeyEvent{
    int key, action, mods;
};

struct WinSize{
    int width;
    int height;
};

// enum class ShaderID{
//     Geometry = 0,
//     Light,
// };

// struct Material{
//     ShaderID shader; 
//     Texture* texture;
//     float shinines;
// };

// struct Object{
//     Mesh* mesh;
//     Material material;
//     glm::mat4 world;
//     glm::mat4 local;
// };

class Application{
    private:
        GLFWwindow* m_Window;
        WinSize m_WinSize;
        std::string m_Name;

        Mesh* m_CubeMesh;
        Mesh* m_TestMesh;
        Mesh* m_StandMesh;
        Mesh* m_ArmMesh;
        Mesh* m_ArmArmMesh;
        //glm::mat4 m_Model;


        Mesh* m_LightMesh;
        glm::mat4 m_LightModel;
        glm::vec3 m_LightPos;

        Camera m_Camera;

        Shader* m_Shader;
        Shader* m_LightShader;
        Texture* m_Texture;

        float m_Angle = 360;
        float m_ArmArmAngle = 360;
        Object* m_ArmTest;
        Object* m_ArmArmsTest;
        std::vector<Object> m_Objects;
        std::vector<Object> m_Lights;

        bool m_Keys[GLFW_KEY_LAST + 1]{};

        float m_LastTime;
        double m_LastX = 0.0;
        double m_LastY = 0.0;
        bool m_FirstMouse = true;
        bool m_isLeftMousePress = false;

        bool m_IsFocused = false;

        void init();
        void initUI();
        void update(float dt);
        void updateUI();
        void render();
        void onKey(int key, int scancode, int action, int mods);
        void onMouseMove(double x, double y);
    public:
        Application(int width, int height, const std::string& name);
        void run();
};