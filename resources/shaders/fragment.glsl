#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


uniform vec3 viewPos;

struct DirLight{
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

#define NUMBER_OF_MAX_DIR_LIGHTS 32
uniform DirLight    uLight[NUMBER_OF_MAX_DIR_LIGHTS];
uniform int         uUsedDirLight;

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir);

void main(){
  vec3 normal = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  vec3 result;
  for(int i = 0; i < uUsedDirLight; i++){
    result += calculateDirectionalLight(uLight[i], normal, viewDir);
  }

  FragColor = vec4(result, 1.0);

}

vec3 calculateDirectionalLight(DirLight light, vec3 normal, vec3 viewDir){
  vec3 lightDir = normalize(light.position - FragPos);

  float diff = max(dot(normal, lightDir), 0.0);
  
  vec3 reflectDir = reflect(-lightDir, normal);

  float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
  
  vec3 ambient = light.ambient * vec3(texture(uMaterial.diffuse, TexCoord));
  vec3 diffuse = light.diffuse * diff * vec3(texture(uMaterial.diffuse, TexCoord));
  vec3 specular = light.specular * spec * (1.0 - texture(uMaterial.specular, TexCoord).r);

  return(ambient + diffuse + specular);
}