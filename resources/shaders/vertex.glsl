#version 330 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 texCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

// uniform vec3 uColor;

out vec3 vertColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main(){
  gl_Position = uProjection * uView * uModel * vec4(vertPos, 1.0);
  FragPos = vec3(uModel * vec4(vertPos, 1.0));
  vertColor = normals * 0.5 + 0.5;
  TexCoord = texCoord;
  Normal = normals;
}
