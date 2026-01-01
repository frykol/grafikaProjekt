#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Shader.h"

struct WinSize{
    int width;
    int height;
};


int main(){
    GLFWwindow* window;

    if(!glfwInit()){
        std::cout << "GLFW init error" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    WinSize size{800, 600};
    window = glfwCreateWindow(size.width, size.height, "proj", NULL, NULL);
    if(!window){
        std::cout << "Window error" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);


    glfwSetWindowUserPointer(window, &size);
    glViewport(0,0, size.width, size.height);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int w, int h){
        WinSize* size = static_cast<WinSize*>(glfwGetWindowUserPointer(win));
        size->width = w;
        size->height = h;
        glViewport(0,0,w,h);
    });


    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		glfwTerminate();
		return -1;
	}
    glEnable(GL_DEPTH_TEST);

    // 36 wierzchołków (12 trójkątów), bez indeksów
// layout: vec3 pos, vec3 normal
    static float cubeVerts[] = {
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

    Shader shader("resources/vertex.glsl","resources/fragment.glsl");
    shader.compile();
    shader.bind();
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float aspect = (float)size.width / (float)size.height;

    glm::mat4 model = glm::mat4(1.0f);
    

    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),  // kamera
        glm::vec3(0.0f, 0.0f, 0.0f),  // patrzy na (0,0,0)
        glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );

    glm::mat4 proj = glm::perspective(
    glm::radians(60.0f),
    aspect,
    0.1f,
    100.0f
    );

    while(!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        float t = (float)glfwGetTime();
        model = glm::mat4(1.0f);
        model = glm::rotate(model, t, glm::vec3(0,1,0));
        model = glm::rotate(model, t, glm::vec3(1,0,0));

        float aspect = (float)size.width / (float)size.height;
        proj = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);

        shader.setMat4("uModel", model);
        shader.setMat4("uView", view);
        shader.setMat4("uProjection", proj);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}