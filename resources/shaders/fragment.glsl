#version 330 core

out vec4 FragColor;

in vec3 vertColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;

uniform vec3 viewPos;

struct Light{
  vec3 position;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct Material{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};

uniform Material uMaterial;
uniform Light    uLight;

void main(){
  //ambient
  vec3 ambient = uLight.ambient * vec3(texture(uMaterial.diffuse, TexCoord));

  
  //diff
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(uLight.position - FragPos);

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = uLight.diffuse * diff * vec3(texture(uMaterial.diffuse, TexCoord));

  //spec
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
  vec3 specular = uLight.specular * spec * (1.0 - texture(uMaterial.specular, TexCoord).r);

  vec3 result = (ambient + diffuse + specular);

  FragColor = vec4(result, 1.0);
  //FragColor = vec4(vec3(texture(uMaterial.specular, TexCoord).r), 1.0);

}
