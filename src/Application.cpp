#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "MeshLoader.h"


Application::Application(int width, int height, const std::string& name): m_WinSize({width, height}), m_Name(name){
    init();
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

    

    float cubeVerts[] = {
    // +Z (front)
    -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
     0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
    -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

    // -Z (back)
    -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,
     0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f,
    -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,

    // +X (right)
     0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
     0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
     0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
     0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
     0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

    // -X (left)
    -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, 0.5f,-0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

    // +Y (top)
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

    // -Y (bottom)
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f,
     0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 1.0f,
     0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f,
     0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,  1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,  0.0f, 1.0f,
    };
    //TEST MESHLOADER
    m_TestMesh = MeshLoader::loadObj("resources/meshes/plane.obj");
    m_StandMesh = MeshLoader::loadObj("resources/meshes/stand.obj");
    m_ArmMesh = MeshLoader::loadObj("resources/meshes/testArm.obj");
    m_ArmArmMesh = MeshLoader::loadObj("resources/meshes/testArm.obj");
    // m_CubeMesh = new Mesh(cubeVerts, sizeof(cubeVerts));
    // m_CubeMesh->addVertexAttribPointer(3);
    // m_CubeMesh->addVertexAttribPointer(3);
    // m_CubeMesh->addVertexAttribPointer(2);
    // m_CubeMesh->enableVertexAttribPointer();

    float aspect = (float)m_WinSize.width / (float)m_WinSize.height;

    m_LightMesh = new Mesh(cubeVerts, sizeof(cubeVerts));
    m_LightMesh->addVertexAttribPointer(3);
    m_LightMesh->addVertexAttribPointer(3);
    m_LightMesh->addVertexAttribPointer(2);
    m_LightMesh->enableVertexAttribPointer();

    m_LightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    m_LightModel = glm::mat4(1.0f);
    m_LightModel = glm::translate(m_LightModel, m_LightPos);
    m_LightModel = glm::scale(m_LightModel, glm::vec3(0.2f));


    m_LightShader = new Shader("resources/shaders/vertex.glsl","resources/shaders/lightFragment.glsl");
    m_LightShader->compile();
    m_LightShader->bind();
    

    m_Shader = new Shader("resources/shaders/vertex.glsl","resources/shaders/fragment.glsl");
    m_Shader->compile();

    m_Texture = new Texture();
    m_Texture->compileTexture("resources/textures/pavement.jpg");
    m_Texture->compileSpecular("resources/textures/pavementSpec.jpg");
    m_Shader->bind();
    m_Shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f,1.0f));

    m_Shader->setVec3("uLight.position", m_LightPos);
    m_Shader->setVec3("uLight.ambient", glm::vec3(0.2f,0.2f,0.2f));
    m_Shader->setVec3("uLight.diffuse", glm::vec3(0.5f,0.5f,0.5f));
    m_Shader->setVec3("uLight.specular", glm::vec3(1.0f,1.0f,1.0f));

    //m_Shader->setVec3("viewPos", m_Camera.getPosition());
    m_Shader->setInt("uMaterial.diffuse", 0);
    m_Shader->setInt("uMaterial.specular", 1);
    m_Shader->unbind();

    m_Objects.reserve(128);

    Material geometry{ShaderID::Geometry, m_Texture, 10};
    // Object obj{m_TestMesh, geometry, glm::mat4(1.0)};
    // m_Objects.emplace_back(obj);

    // glm::mat4 secondModel = glm::mat4(1.0f);
    // secondModel = glm::translate(secondModel, glm::vec3(0, 5.0f, -3.0f));

    // Object sObj{m_LightMesh, geometry, secondModel};
    // m_Objects.emplace_back(sObj);
    

    // Object stand{m_StandMesh, geometry, glm::mat4(1.0f),glm::mat4(1.0f)};
    // m_Objects.emplace_back(stand);

    // glm::mat4 armLocal = glm::translate(glm::mat4(1.0f), glm::vec3(-0.62f, 5.4f, 0.0f));
    // glm::mat4 armWorld = stand.world * armLocal;

    // Object arm{m_ArmMesh, geometry, armWorld, armLocal};
    // size_t armIndex = m_Objects.size();
    // m_Objects.emplace_back(arm);
    // m_ArmTest = &m_Objects[armIndex];

    // glm::mat4 armArmLocal = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, -3.9f));
    // glm::mat4 armArmWorld = armWorld * armArmLocal;
    // Object armArm{m_ArmArmMesh, geometry, armArmWorld, armArmLocal};
    // size_t armArmIndex = m_Objects.size();
    // m_Objects.emplace_back(armArm);
    // m_ArmArmsTest = &m_Objects[armArmIndex];

    m_Objects.emplace_back("Plane", m_TestMesh, geometry);
    m_Objects.emplace_back("Kostka", m_LightMesh, geometry, glm::vec3(0, 5.0f, -3.0f));
    size_t standIndex = m_Objects.size();
    m_Objects.emplace_back("Stand", m_StandMesh, geometry);
    size_t ArmIndex = m_Objects.size();
    m_Objects.emplace_back("First Arm", m_ArmMesh, geometry, glm::vec3(-0.62f, 5.4f, 0.40f));
    size_t ArmArmIndex = m_Objects.size();
    m_Objects.emplace_back("Second Arm", m_ArmMesh, geometry, glm::vec3(0, 0, -3.9f));

    Object* stand = &m_Objects[standIndex];
    Object* arm = &m_Objects[ArmIndex];
    arm->setParent(stand);
    Object* armArm = &m_Objects[ArmArmIndex];
    armArm->setParent(arm);
}

void Application::initUI(){
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_Window, false);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; // opcjonalnie

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

void Application::update(float dt){
    

    if (m_Keys[GLFW_KEY_W]) m_Camera.moveForward(dt);
    if (m_Keys[GLFW_KEY_S]) m_Camera.moveBackward(dt);
    if (m_Keys[GLFW_KEY_D]) m_Camera.moveRight(dt);
    if (m_Keys[GLFW_KEY_A]) m_Camera.moveLeft(dt);
    if (m_Keys[GLFW_KEY_SPACE]) m_Camera.moveUp(dt);
    if (m_Keys[GLFW_KEY_LEFT_CONTROL]) m_Camera.moveDown(dt);

    if (m_Keys[GLFW_KEY_LEFT_SHIFT])
        m_Camera.fastMove();
    else
        m_Camera.normalMove();

    if(m_Keys[GLFW_KEY_ESCAPE]){
        m_IsFocused = false;
        glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }


    m_LightModel = glm::mat4(1.0f);
    m_LightModel = glm::translate(m_LightModel, m_LightPos);
    m_LightModel = glm::scale(m_LightModel, glm::vec3(0.2f));
}

void Application::updateUI(){
        ImGuiIO& io = ImGui::GetIO();

        if (m_IsFocused) {
        io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
        // opcjonalnie: usuń mysz z UI żeby nic nie było hoverowane
        
        } else {
            io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        }   

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Ogólne");
            glm::vec3& cameraPos = m_Camera.getPosition();    
            ImGui::Text("Camera Position: %.2f, %.2f, %.2f", cameraPos.x, cameraPos.y, cameraPos.z);
            ImGui::SliderFloat3("Light Position", &m_LightPos[0], -10, 10, "%.1f");
            ImGui::SliderFloat("Shinines", &m_Objects[0].getMaterial().shinines, 1, 300, "%.0f");
            ImGui::SliderFloat("Angle", &m_Angle, 360, 0, "%.0f");
            ImGui::SliderFloat("Angle Arm", &m_ArmArmAngle, 360, 0, "%.0f");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        {
            ImGui::Begin("Obiekty");                        
            for(Object& object : m_Objects){
                if(ImGui::TreeNode(object.getName().c_str())){
                    glm::vec3& objectPosition = object.getPosition();
                    ImGui::Text("Position: %.2f, %.2f, %.2f", objectPosition.x, objectPosition.y, objectPosition.z);
                    ImGui::SliderFloat3("Set Position", &objectPosition[0], -10, 10, "%.1f");
                    glm::vec3& objectRotation = object.getRotation();
                    ImGui::Text("Rotation: %.0f, %.0f, %.0f", objectRotation.x, objectRotation.y, objectRotation.z);
                    ImGui::SliderFloat3("Set Rotation", &objectRotation[0], -360, 360, "%.0f");
                    ImGui::TreePop();
                }
            }

            
            ImGui::End();
        }

        if(m_isLeftMousePress){
            m_isLeftMousePress = false;
            if(!m_IsFocused && !ImGui::GetIO().WantCaptureMouse){
                m_FirstMouse = true;
                m_IsFocused = true;
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        
}

void Application::render(){
    //glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float aspect = (float)m_WinSize.width / (float)m_WinSize.height;
    glm::mat4 view = m_Camera.getView();
    glm::mat4 proj = m_Camera.getProjection(aspect);

    m_LightShader->bind();
    m_LightShader->setMat4("uModel", m_LightModel);
    m_LightShader->setMat4("uView", view);
    
    m_LightShader->setMat4("uProjection", proj);

    m_LightMesh->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_LightMesh->getVertexCount());
    m_LightMesh->unbind();

   

    // m_ArmTest->local = glm::mat4(1.0f);
    // m_ArmTest->local = glm::translate(glm::mat4(1.0f), glm::vec3(-0.62f, 5.4f, 0.48f));
    // m_ArmTest->local = glm::rotate(m_ArmTest->local, glm::radians(m_Angle), glm::vec3(1.0f,0.0f,0.0f));
    // m_ArmTest->world = glm::mat4(1.0f) * m_ArmTest->local;

    // //m_ArmArmsTest->local = glm::mat4(1.0f);`
    // m_ArmArmsTest->local = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, -3.9f));
    // m_ArmArmsTest->local = glm::rotate(m_ArmArmsTest->local, glm::radians(m_ArmArmAngle), glm::vec3(1.0f,0.0f,0.0f));
    // m_ArmArmsTest->world = m_ArmTest->world * m_ArmArmsTest->local;
    
    //REWORK
    for(Object& object : m_Objects){
        if(object.getMaterial().shader == ShaderID::Geometry){
            m_Shader->bind();
            m_Shader->setMat4("uModel", object.worldMatrix());
            m_Shader->setMat4("uView", view);
            m_Shader->setMat4("uProjection", proj);
            m_Shader->setVec3("viewPos", m_Camera.getPosition());
            m_Shader->setVec3("uLight.position", m_LightPos);
            m_Shader->setFloat("uMaterial.shininess", object.getMaterial().shinines);
            object.getMaterial().texture->bind();
            object.getMesh()->bind();
            glDrawArrays(GL_TRIANGLES, 0,  object.getMesh()->getVertexCount());
            object.getMesh()->unbind();
            object.getMaterial().texture->unBind();
            m_Shader->unbind();
        }

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