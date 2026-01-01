#include "Application.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


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
    app->onMouseMove(x, y);
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
    -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
     0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,

    // -Z (back)
    -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
     0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,

    // +X (right)
     0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,

    // -X (left)
    -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, -1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,

    // +Y (top)
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,

    // -Y (bottom)
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,
     0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,
    };
    m_TestMesh = new Mesh(cubeVerts, sizeof(cubeVerts));
    m_TestMesh->addVertexAttribPointer(3);
    m_TestMesh->addVertexAttribPointer(3);
    m_TestMesh->enableVertexAttribPointer();

    float aspect = (float)m_WinSize.width / (float)m_WinSize.height;

    m_Model = glm::mat4(1.0f);
    

    // m_View = glm::lookAt(
    //     glm::vec3(0.0f, 0.0f, 3.0f),  // kamera
    //     glm::vec3(0.0f, 0.0f, 0.0f),  // patrzy na (0,0,0)
    //     glm::vec3(0.0f, 1.0f, 0.0f)   // up
    // );

    // m_Projection = glm::perspective(
    // glm::radians(60.0f),
    // aspect,
    // 0.1f,
    // 100.0f
    // );

    m_Shader = new Shader("resources/vertex.glsl","resources/fragment.glsl");
    m_Shader->compile();
}

void Application::run(){
    while(!glfwWindowShouldClose(m_Window)){
        float now = (float)glfwGetTime();
        float dt = now - m_LastTime;

        m_LastTime = now;

        update(dt);
        render();

        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}

void Application::update(float dt){
    m_Model = glm::rotate(m_Model, dt, glm::vec3(0,1,0));
    m_Model = glm::rotate(m_Model, dt, glm::vec3(1,0,0));

    if (m_Keys[GLFW_KEY_W]) m_Camera.moveForward(dt);
    if (m_Keys[GLFW_KEY_S]) m_Camera.moveBackward(dt);
    if (m_Keys[GLFW_KEY_D]) m_Camera.moveRight(dt);
    if (m_Keys[GLFW_KEY_A]) m_Camera.moveLeft(dt);
    if (m_Keys[GLFW_KEY_SPACE]) m_Camera.moveUp(dt);
    if (m_Keys[GLFW_KEY_LEFT_SHIFT]) m_Camera.moveDown(dt);

}

void Application::render(){
    glClearColor(0.2f,0.3f,0.3f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_Shader->bind();
    m_Shader->setMat4("uModel", m_Model);
    float aspect = (float)m_WinSize.width / (float)m_WinSize.height;
    glm::mat4 view = m_Camera.getView();
    glm::mat4 proj = m_Camera.getProjection(aspect);
    m_Shader->setMat4("uView", view);
    
    m_Shader->setMat4("uProjection", proj);

    m_TestMesh->bind();
    glDrawArrays(GL_TRIANGLES, 0, m_TestMesh->getVertexCount());
    m_TestMesh->unbind();
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