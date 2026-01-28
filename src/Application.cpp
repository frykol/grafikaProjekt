#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// #include <imgui.h>
// #include <backends/imgui_impl_glfw.h>
// #include <backends/imgui_impl_opengl3.h>

#include "ui/GeneralLayer.h"
#include "ui/ObjectLayer.h"

#include "MeshLoader.h"


Application::Application(int width, int height, const std::string& name): m_WinSize({width, height}), m_Name(name){
    init();
    initData();
}

Application::~Application(){
    if(m_Robot){
        std::cout << "Robot Deleted" << std::endl;
        delete m_Robot;
    }
    if(m_SecondRobot){
        std::cout << "Second robot Deleted" << std::endl;
        delete m_SecondRobot;
    }

    m_Objects.clear();
    m_Lights.clear();
    if(m_TestMesh){
        std::cout << "Cube Mesh Deleted" << std::endl;
        delete m_TestMesh;
    }
    if(m_LightMesh){
        std::cout << "Light Mesh Deleted" << std::endl;
        delete m_LightMesh;
    }
    if(m_RobotStand){
        std::cout << "Robot Stand Deleted" << std::endl;
        delete m_RobotStand;
    }
    if(m_RobotEngine){
        std::cout << "Robot Engine Deleted" << std::endl;
        delete m_RobotEngine;
    }
    if(m_RobotArm){
        std::cout << "Robot Arm Deleted" << std::endl;
        delete m_RobotArm;
    }
    if(m_RobotSecondArm){
        std::cout << "Robot Second Arm Deleted" << std::endl;
        delete m_RobotSecondArm;
    }

}


void Application::init(){
    if(!glfwInit()){
        std::cout << "GLFW init error" << std::endl;
        throw std::runtime_error("GLFW init error");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_WinSize.width, m_WinSize.height, m_Name.c_str(), NULL, NULL);

    if(!m_Window){
        glfwTerminate();
        throw std::runtime_error("Window init error");
    }

    glfwMakeContextCurrent(m_Window);
    initUI();
    m_IsFocused = true;
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(m_Window, this);
    glViewport(0,0, m_WinSize.width, m_WinSize.height);

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* win, int w, int h){
        Application* app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        app->m_WinSize.width = w;
        app->m_WinSize.height = h;
        glViewport(0,0,w,h);
    });

    glfwSetKeyCallback(m_Window, [](GLFWwindow* win, int key, int scancode, int action, int mods){
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        app->onKey(key, scancode, action, mods);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* win, double x, double y){
        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(win));
        if(!app->m_IsFocused){
            ImGui_ImplGlfw_CursorPosCallback(win, x, y);
        }
        else{
            app->onMouseMove(x, y);
        }
    });
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods){
        ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

        auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
            app->m_isLeftMousePress = true;
        }
    });
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		glfwTerminate();
        throw std::runtime_error("GLEW init error");
	}
    glEnable(GL_DEPTH_TEST);

}

// void createRobot(){
//     size_t standIndex = m_Objects.size();
//     m_Objects.emplace_back("Robot Stand", m_RobotStand, metalMat, glm::vec3(0, 0.4f, 0));
//     size_t engineIndex = m_Objects.size();
//     m_Objects.emplace_back("Robot Engine", m_RobotEngine, metalMat, glm::vec3(-0.69f, 0.39f, -0.02f));
//     size_t armIndex = m_Objects.size();
//     m_Objects.emplace_back("Robot Arm", m_RobotArm, metalMat, glm::vec3(0.18f, 0.76f, -1.04f));
//     size_t secondArmIndex = m_Objects.size();
//     m_Objects.emplace_back("Robot Second Arm", m_RobotSecondArm, metalMat, glm::vec3(-0.14f, 3.75f, 0.08f));

//     Object* stand = &m_Objects[standIndex];
//     Object* engine = &m_Objects[engineIndex];
//     Object* arm = &m_Objects[armIndex];
//     Object* secondArm = &m_Objects[secondArmIndex];
// }

testRobotS* createRobot(std::vector<Object>& objects,const std::string& name, robotMesh robotMesh, Material material, glm::vec3 position){
    testRobotS* robot = new testRobotS;

    size_t standIndex = objects.size();
    objects.emplace_back(name + " Stand", robotMesh.robotStand, material, position);
    size_t engineIndex = objects.size();
    objects.emplace_back(name + " Engine", robotMesh.robotEngine, material, glm::vec3(-0.69f, 0.39f, -0.02f));
    size_t armIndex = objects.size();
    objects.emplace_back(name+" Arm", robotMesh.robotArm, material, glm::vec3(0.18f, 0.76f, -1.04f));
    size_t secondArmIndex = objects.size();
    objects.emplace_back(name + " Second Arm", robotMesh.robotSecondArm, material, glm::vec3(-0.14f, 3.75f, 0.08f));

    Object* stand = &objects[standIndex];
    Object* engine = &objects[engineIndex];
    Object* arm = &objects[armIndex];
    Object* secondArm = &objects[secondArmIndex];


    robot->animTime = 0;
    robot->animDuration = 10.0f;
    robot->stand = stand;
    robot->engine = engine;
    robot->arm = arm;
    robot->secondArm = secondArm;


    engine->setParent(stand);
    arm->setParent(engine);
    secondArm->setParent(arm);

    return robot;
}

void Application::initData(){
    //TEST MESHLOADER
    m_LightMesh = MeshLoader::loadObj("resources/meshes/cube.obj");
    m_TestMesh = MeshLoader::loadObj("resources/meshes/plane.obj");
    m_WallMesh = MeshLoader::loadObj("resources/meshes/wall.obj");

    float aspect = (float)m_WinSize.width / (float)m_WinSize.height;

    
    m_LightShader = new Shader("resources/shaders/vertex.glsl","resources/shaders/lightFragment.glsl");
    m_LightShader->compile();
    m_LightShader->bind();
    
    glm::vec3 lightPos = glm::vec3(0, 10.0f, 0);
    Material dummy;
    m_Lights.emplace_back("Light 1", m_LightMesh, dummy, lightPos).setScale(0.2f);

    m_Shader = new Shader("resources/shaders/vertex.glsl","resources/shaders/fragment.glsl");
    m_Shader->compile();

    m_Texture = new Texture();
    m_Texture->compileTexture("resources/textures/pavement.jpg");
    m_Texture->compileSpecular("resources/textures/pavementSpec.jpg");

    m_MetalTexture = new Texture();
    m_MetalTexture->compileTexture("resources/textures/metal.jpg");
    m_MetalTexture->compileSpecular("resources/textures/metalSpec.jpg");

    m_CubeTexture = new Texture();
    m_CubeTexture->compileTexture("resources/textures/crate.jpg");
    m_CubeTexture->compileSpecular("resources/textures/crateSpec.jpg");

    m_WallTexture = new Texture();
    m_WallTexture->compileTexture("resources/textures/metal.jpg");
    m_WallTexture->compileSpecular("resources/textures/metalSpec.jpg");

    m_Shader->bind();
    m_Shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f,1.0f));

    m_Shader->setVec3("uLight[0].position", lightPos);
    m_Shader->setVec3("uLight[0].ambient", glm::vec3(0.3f,0.3f,0.3f));
    m_Shader->setVec3("uLight[0].diffuse", glm::vec3(0.4f,0.4f,0.4f));
    m_Shader->setVec3("uLight[0].specular", glm::vec3(1.0f,1.0f,1.0f));
    //m_Shader->setInt("uUsedDirLight", 1);

    m_Shader->setInt("uMaterial.diffuse", 0);
    m_Shader->setInt("uMaterial.specular", 1);
    m_Shader->unbind();

    m_Objects.reserve(128);

    Material geometry{ShaderID::Geometry, m_Texture, 10};
    Material metalMat{ShaderID::Geometry, m_MetalTexture, 256};

    Material cubeMat{ShaderID::Geometry, m_CubeTexture, 10};

    m_Objects.emplace_back("Plane", m_TestMesh, geometry);
    m_Objects.emplace_back("Wall1", m_WallMesh, metalMat, glm::vec3(0, 4, 30), glm::vec3(90,0,0));
    m_Objects.emplace_back("Wall2", m_WallMesh, metalMat, glm::vec3(0, 4, -30), glm::vec3(90,0,0));
    m_Objects.emplace_back("Wall3", m_WallMesh, metalMat, glm::vec3(30, 4, 0), glm::vec3(90,0,90));
    m_Objects.emplace_back("Wall3", m_WallMesh, metalMat, glm::vec3(-30, 4, 0), glm::vec3(90,0,90));
    
    m_RobotStand = MeshLoader::loadObj("resources/meshes/firstRobot/stand.obj");
    m_RobotEngine = MeshLoader::loadObj("resources/meshes/firstRobot/engine.obj");
    m_RobotArm = MeshLoader::loadObj("resources/meshes/firstRobot/arm.obj");
    m_RobotSecondArm = MeshLoader::loadObj("resources/meshes/firstRobot/second_arm.obj");

    robotMesh rMesh{.robotStand = m_RobotStand, .robotEngine = m_RobotEngine, .robotArm = m_RobotArm, .robotSecondArm = m_RobotSecondArm};
    m_SecondRobot = createRobot(m_Objects, "RobotoZiomo", rMesh, metalMat, glm::vec3(-13.2f, 0.4, -1.2f));
    m_SecondRobot->engine->setRotation(glm::vec3(0, 180.0f, 0));
    m_Robot = createRobot(m_Objects, "Pierwszy", rMesh, metalMat, glm::vec3(0, 0.4, 0));
    m_Robot->engineRotation = glm::vec3(0, 120.0f, 0);
    m_Robot->armRotation = glm::vec3(0,0, 11.0f);
    m_Robot->secondArmRotation = glm::vec3(0, 0, 44.0f);
    m_SecondRobot->engineRotation = glm::vec3(0, 90.0f, 0);
    m_SecondRobot->armRotation = glm::vec3(0, 0, 11.0f);
    m_SecondRobot->secondArmRotation = glm::vec3(0,0, 44.0f);
    // size_t standIndex = m_Objects.size();
    // m_Objects.emplace_back("Robot Stand", m_RobotStand, metalMat, glm::vec3(0, 0.4f, 0));
    // size_t engineIndex = m_Objects.size();
    // m_Objects.emplace_back("Robot Engine", m_RobotEngine, metalMat, glm::vec3(-0.69f, 0.39f, -0.02f));
    // size_t armIndex = m_Objects.size();
    // m_Objects.emplace_back("Robot Arm", m_RobotArm, metalMat, glm::vec3(0.18f, 0.76f, -1.04f));
    // size_t secondArmIndex = m_Objects.size();
    // m_Objects.emplace_back("Robot Second Arm", m_RobotSecondArm, metalMat, glm::vec3(-0.14f, 3.75f, 0.08f));

    // Object* stand = &m_Objects[standIndex];
    // Object* engine = &m_Objects[engineIndex];
    // Object* arm = &m_Objects[armIndex];
    // Object* secondArm = &m_Objects[secondArmIndex];


    // m_Robot->animTime = 0;
    // m_Robot->animDuration = 10.0f;
    // m_Robot->stand = stand;
    // m_Robot->engine = engine;
    // m_Robot->arm = arm;
    // m_Robot->secondArm = secondArm;
    // m_Robot->engineRotation = glm::vec3(0, 120.0f, 0);
    // m_Robot->armRotation = glm::vec3(0,0, 11.0f);
    // m_Robot->secondArmRotation = glm::vec3(0, 0, 44.0f);

    // engine->setParent(stand);
    // arm->setParent(engine);
    // secondArm->setParent(arm);

    size_t cubeIndex = m_Objects.size();
    m_Objects.emplace_back("Cube to pickup", m_LightMesh, cubeMat, glm::vec3(1.7f, 0.6f, 5.4f));
    m_CubeToPickup = &m_Objects[cubeIndex];
    m_CubeToPickup->setRotation(glm::vec3(0, 33.0f, 0));
}

void Application::initUI(){
    m_UI.init(m_Window);
}

void Application::updateUI(){
        
        m_UI.newFrame(m_IsFocused);
        glm::vec3 cameraPos = m_Camera.getPosition();
        m_UI.pushLayer<GeneralLayer>(cameraPos, m_Lights);
        m_UI.pushLayer<ObjectLayer>(m_Objects, m_Camera.getPosition());

        if(m_isLeftMousePress){
            m_isLeftMousePress = false;
            if(!m_IsFocused && !ImGui::GetIO().WantCaptureMouse){
                m_FirstMouse = true;
                m_IsFocused = true;
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }

        m_UI.render();    
}

bool animateStage(float& robotAngle, float robotDesireAngle, float angleSpeed, float negative = 1){
    bool direction = (negative > 0) ? (robotAngle < robotDesireAngle ) : (robotAngle > robotDesireAngle);
    bool secondDirection = (negative > 0) ? (robotAngle + angleSpeed >= robotDesireAngle) : (robotAngle + angleSpeed <= robotDesireAngle);

    // if(direction){
    //     if(secondDirection){
    //         robotAngle = robotDesireAngle;
    //         return true;
    //     }
    //     else{
    //         robotAngle += angleSpeed * negative;
    //         return false;
    //     }
    // }
    // return true;
    if(secondDirection){
        robotAngle = robotDesireAngle;
        return true;
    }
    robotAngle += angleSpeed * negative;
    return false;
}

void Application::testAnimate(float dt){
    float& ea = m_Robot->engine->getRotation().y;
    std::cout << ea << std::endl;
    std::cout << "Desired: " << m_Robot->engineRotation.y << std::endl;
    //NEW
    if(m_Robot->stage == 0){
        float& engineAngle = m_Robot->engine->getRotation().y;
        float addAngle = 20.0f * dt;
        if(animateStage(engineAngle, m_Robot->engineRotation.y, addAngle)){
            m_Robot->stage++;
        }
    }
    else if(m_Robot->stage == 1){
        float& armAngle = m_Robot->arm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(armAngle, m_Robot->armRotation.z, addAngle)){
            m_Robot->stage++;
        }
    }
    else if(m_Robot->stage == 2){
        float& secondArmAngle = m_Robot->secondArm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(secondArmAngle, m_Robot->secondArmRotation.z, addAngle)){
            m_Robot->stage++;
            m_CubeToPickup->setRotation(glm::vec3(0));
            m_CubeToPickup->setPosition(glm::vec3(-6.7f, 1.5f, 0.2f));
            m_CubeToPickup->setParent(m_Robot->secondArm);
        }
    }
    else if(m_Robot->stage == 3){
        m_Robot->secondArmRotation = glm::vec3(0);
        float& secondArmAngle = m_Robot->secondArm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(secondArmAngle, m_Robot->secondArmRotation.z, addAngle, -1)){
            m_Robot->stage++;
        }
    }
    else if(m_Robot->stage == 4){
        m_Robot->armRotation = glm::vec3(0);
        float& armAngle = m_Robot->arm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(armAngle, m_Robot->armRotation.z, addAngle, -1)){
            m_Robot->stage++;
        }
    }
    else if(m_Robot->stage == 5){
        m_Robot->engineRotation = glm::vec3(0);
        float& engineAngle = m_Robot->engine->getRotation().y;
        float addAngle = 20.0f * dt;
        if(animateStage(engineAngle, m_Robot->engineRotation.y, addAngle, -1)){
            m_Robot->stage++;
            m_SecondRobot->stage++;
            m_CubeToPickup->setParent(m_SecondRobot->secondArm);
        }
    }
    else if(m_SecondRobot->stage == 1){
        float& engineAngle = m_SecondRobot->engine->getRotation().y;
        float addAngle = 20.0f * dt;
        if(animateStage(engineAngle, m_SecondRobot->engineRotation.y, addAngle, -1)){
            m_SecondRobot->stage++;
        }
    }
    else if(m_SecondRobot->stage == 2){
        float& armAngle = m_SecondRobot->arm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(armAngle, m_SecondRobot->armRotation.z, addAngle, 1)){
            m_SecondRobot->stage++;
        }
    }
    else if(m_SecondRobot->stage == 3){
        float& secondArmAngle = m_SecondRobot->secondArm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(secondArmAngle, m_SecondRobot->secondArmRotation.z, addAngle, 1)){
            m_SecondRobot->stage++;
            m_SecondRobot->engineRotation = glm::vec3(0, 180.0f, 0);
            m_SecondRobot->armRotation = glm::vec3(0);
            m_SecondRobot->secondArmRotation = glm::vec3(0);
            m_CubeToPickup->setPositionToWorldMatrix();
            m_CubeToPickup->setParent(nullptr);
        }
    }
    else if(m_SecondRobot->stage == 4){
        float& secondArmAngle = m_SecondRobot->secondArm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(secondArmAngle, m_SecondRobot->secondArmRotation.z, addAngle, -1)){
            m_SecondRobot->stage++;
        }
    }
    else if(m_SecondRobot->stage == 5){
        float& armAngle = m_SecondRobot->arm->getRotation().z;
        float addAngle = 20.0f * dt;
        if(animateStage(armAngle, m_SecondRobot->armRotation.z, addAngle, -1)){
            m_SecondRobot->stage++;
        }
    }
    else if(m_SecondRobot->stage == 6){
        float& engineAngle = m_SecondRobot->engine->getRotation().y;
        float addAngle = 20.0f * dt;
        if(animateStage(engineAngle, m_SecondRobot->engineRotation.y, addAngle, 1)){
            m_Robot->engine->setRotation(glm::vec3(0));
            m_SecondRobot->engine->setRotation(glm::vec3(0,120, 0));
            m_Robot->stage = 0;
            m_SecondRobot->stage = 0;
        }
    }
    else if(m_SecondRobot->stage == 7){
        
    }
    //OLD
    // if(m_Robot->animTime > m_Robot->animDuration){
    //     m_Robot->animTime = 0;
    // }
    // else if(m_Robot->animTime >= 0.0f && m_Robot->animTime < 2.0f){
    //     glm::vec3& engineRot = m_Robot->engine->getRotation();
    //     engineRot.y += 50.0f * dt;
    // }
    // else if(m_Robot->animTime >= 2.0f && m_Robot->animTime < 3.0f){
    //     glm::vec3& armRot = m_Robot->arm->getRotation();
    //     armRot.z -= 30.0f * dt;
    // }
    // else if(m_Robot->animTime >= 3.0f && m_Robot->animTime < 5.0f){
    //     glm::vec3& secondArm = m_Robot->secondArm->getRotation();
    //     secondArm.z -= 30.0f * dt;
    // }
    // else if(m_Robot->animTime >= 5.0f && m_Robot->animTime < 7.0f){
    //     glm::vec3& secondArm = m_Robot->secondArm->getRotation();
    //     secondArm.z += 30.0f * dt;
    // }
    // else if(m_Robot->animTime >= 7.0f && m_Robot->animTime < 8.0f){
    //     glm::vec3& armRot = m_Robot->arm->getRotation();
    //     armRot.z += 30.0f * dt;
    // }
    // else if(m_Robot->animTime >= 8.0f && m_Robot->animTime < 10.0f){
    //     glm::vec3& engineRot = m_Robot->engine->getRotation();
    //     engineRot.y -= 50.0f * dt;
    // }
}

void Application::update(float dt){
    

    if (m_Keys[GLFW_KEY_W]) m_Camera.moveForward(dt);
    if (m_Keys[GLFW_KEY_S]) m_Camera.moveBackward(dt);
    if (m_Keys[GLFW_KEY_D]) m_Camera.moveRight(dt);
    if (m_Keys[GLFW_KEY_A]) m_Camera.moveLeft(dt);
    if (m_Keys[GLFW_KEY_SPACE]) m_Camera.moveUp(dt);
    if (m_Keys[GLFW_KEY_LEFT_CONTROL]) m_Camera.moveDown(dt);

    if(m_Keys[GLFW_KEY_M]){
        glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
    }

    if (m_Keys[GLFW_KEY_LEFT_SHIFT])
        m_Camera.fastMove();
    else
        m_Camera.normalMove();

    if(m_Keys[GLFW_KEY_ESCAPE]){
        m_IsFocused = false;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

   testAnimate(dt);
}


void Application::render(){
    //glClearColor(0.2f,0.3f,0.3f,1.0f);
    //glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClearColor(0.53f,0.81f,0.92f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = (float)m_WinSize.width / (float)m_WinSize.height;
    glm::mat4 view = m_Camera.getView();
    glm::mat4 proj = m_Camera.getProjection(aspect);

    //REWORK
    for(Object& object : m_Objects){
        if(object.getMaterial().shader == ShaderID::Geometry){
            m_Shader->bind();
            m_Shader->setMat4("uModel", object.worldMatrix());
            m_Shader->setMat4("uView", view);
            m_Shader->setMat4("uProjection", proj);
            m_Shader->setVec3("viewPos", m_Camera.getPosition());
            size_t numOfLights = m_Lights.size();
            for(int i = 0; i < numOfLights; i++){
                m_Shader->setVec3("uLight[" + std::to_string(i) + "].position", m_Lights[i].getPosition());
            }
            m_Shader->setInt("uUsedDirLight", numOfLights);
            m_Shader->setFloat("uMaterial.shininess", object.getMaterial().shinines);
            object.getMaterial().texture->bind();
            object.getMesh()->bind();
            glDrawArrays(GL_TRIANGLES, 0,  object.getMesh()->getVertexCount());
            object.getMesh()->unbind();
            object.getMaterial().texture->unBind();
            m_Shader->unbind();
        }
    }
    for(Object& object : m_Lights){
        m_LightShader->bind();
        m_LightShader->setMat4("uModel", object.worldMatrix());
        m_LightShader->setMat4("uView", view);
        m_LightShader->setMat4("uProjection", proj);
        object.getMesh()->bind();
        glDrawArrays(GL_TRIANGLES, 0,  object.getMesh()->getVertexCount());
        object.getMesh()->unbind();
    }
}

void Application::run(){
    while(!glfwWindowShouldClose(m_Window)){
        glfwPollEvents();
        float now = (float)glfwGetTime();
        float dt = now - m_LastTime;

        m_LastTime = now;

        update(dt);
        render();
        updateUI();

        glfwSwapBuffers(m_Window);
    }
}

void Application::onKey(int key, int scancode, int action, int mods){
    if(key < 0) 
        return;
    if(action == GLFW_PRESS)
        m_Keys[key] = true;
    if(action == GLFW_RELEASE)
        m_Keys[key] = false;
}

void Application::onMouseMove(double x, double y){
    
    if(m_FirstMouse){
        m_LastX = x;
        m_LastY = y;
        m_FirstMouse = false;
    }
    float dx = (float)x - m_LastX;
    float dy = (float)m_LastY - y;

    m_LastX = x;
    m_LastY = y;
    m_Camera.look(dx, dy);
}