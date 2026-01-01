#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 normals;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

// uniform vec3 uColor;

out vec3 vertColor;

void main(){
  gl_Position = uProjection * uView * uModel * vec4(vertPos, 1.0);
  vertColor = vertPos;
}
