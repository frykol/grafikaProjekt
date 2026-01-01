#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class Shader
 * @brief Create shaders for renderer
 * @details
 *   Create two Shaders, vertexShader and fragmentShader
 */
class Shader {
private:
  unsigned int m_programId;

  unsigned int m_vertexId;
  unsigned int m_fragmentId;

  std::string m_vertexSource;
  std::string m_fragmentSource;

  /**
   * @brief Reading shader file
   * 
   * @param path path to a shader file
   * @return std::string returns shader in form of string 
   */
  std::string readShader(const std::string &path);
  /**
   * @brief Compiling shader for OpenGL
   * 
   * @param shaderType type of shader (vertex or fragment)
   * @param shaderSource shader in from of string
   * @return int status code infroming if compiling is success or failure
   */
  int compileShader(GLenum shaderType, const std::string &shaderSource);

public:
  Shader(const std::string &vertexS, const std::string &fragmentS)
      : m_vertexSource(vertexS), m_fragmentSource(fragmentS) {}

  void compile();

  void setMat4(const std::string& uniformName, const glm::mat4& matrix);
  void setVec3(const std::string& uniformName, const glm::vec3& vector);

  void bind();
  void unbind();
};
