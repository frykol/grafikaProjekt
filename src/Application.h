#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Object.h"
#include "ui/EditorUI.h"

#include <vector>

#include <glm/glm.hpp>

struct KeyEvent{
    int key, action, mods;
};

struct WinSize{
    int width;
    int height;
};

struct robotMesh{
    Mesh* robotStand;
    Mesh* robotEngine;
    Mesh* robotArm;
    Mesh* robotSecondArm;
};

struct testRobotS{
    float animTime;
    float animDuration;
    Object* stand;
    Object* engine;
    Object* arm;
    Object* secondArm;

    int stage = 0;

    glm::vec3 engineRotation;
    glm::vec3 armRotation;
    glm::vec3 secondArmRotation;
};

class Application{
    private:
        GLFWwindow* m_Window;
        WinSize m_WinSize;
        std::string m_Name;

        Mesh* m_TestMesh;
        Mesh* m_WallMesh;
        //TEST ROBOT
        Mesh* m_RobotStand;
        Mesh* m_RobotEngine;
        Mesh* m_RobotArm;
        Mesh* m_RobotSecondArm;


        //END TEST ROBOT
        testRobotS* m_Robot;
        testRobotS* m_SecondRobot;

        Mesh* m_LightMesh;

        Camera m_Camera;

        Shader* m_Shader;
        Shader* m_LightShader;
        Texture* m_Texture;
        Texture* m_MetalTexture;
        Texture* m_CubeTexture;
        Texture* m_WallTexture;

        Object* m_CubeToPickup;

        std::vector<Object> m_Objects;
        std::vector<Object> m_Lights;

        EditorUI m_UI;
        

        bool m_Keys[GLFW_KEY_LAST + 1]{};

        float m_LastTime;
        double m_LastX = 0.0;
        double m_LastY = 0.0;
        bool m_FirstMouse = true;
        bool m_isLeftMousePress = false;

        bool m_IsFocused = false;

        void testAnimate(float dt);

        void init();
        void initData();
        void initUI();
        void update(float dt);
        void updateUI();
        void render();
        void onKey(int key, int scancode, int action, int mods);
        void onMouseMove(double x, double y);
    public:
        Application(int width, int height, const std::string& name);
        ~Application();
        void run();
};