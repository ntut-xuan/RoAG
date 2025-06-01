#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

uniform sampler2D ourTexture;
uniform sampler2D dirShadowMap;
uniform samplerCube pointShadowMap;

uniform vec3 ViewPos;

struct dirLight_t
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  mat4 lightSpaceMatrix;
};

struct pointLight_t
{
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float farPlane;
};

uniform dirLight_t dirLight;
uniform pointLight_t pointLight;

float DirShadowCalculation(vec4 fragPosLightSpace, vec3 norm, vec3 lightDir)
{
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5; // [0, 1] range
  float closestDepth = texture(dirShadowMap, projCoords.xy).r; 
  float currentDepth = projCoords.z;
  float bias = max(0.001 * (1.0 - dot(norm, lightDir)), 0.0005);
  float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  
  return shadow;
}  

float PointShadowCalculation(vec3 FragPos, vec3 lightPos) {
    vec3 fragToLight = FragPos - lightPos;
    float closestDepth = texture(pointShadowMap, fragToLight).r;
    closestDepth *= pointLight.farPlane;
    float currentDepth = length(fragToLight);
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 1.0 : 0.0;
    return shadow;
}

vec3 DirLightCalculation(dirLight_t light, vec3 norm, vec3 viewDir) {
  float gloss = 2.0; 
  // uniform: light to frag
  // what we want: frag to light
  vec3 lightDir = -light.direction;

  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * light.ambient;
  
  float specularStrength = 0.7;
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * light.diffuse;

  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(norm, halfwayDir), 0.0), gloss);
  vec3 specular = specularStrength * spec * light.specular;

  float shadow = DirShadowCalculation(FragPosLightSpace, norm, lightDir);
  return ((1.0 - shadow) * (diffuse + specular)) + ambient;

}

vec3 PointLightCalculation(pointLight_t light, vec3 norm, vec3 viewDir) {
  float gloss = 2.0; 
  vec3 lightDir = normalize(light.position - FragPos);

  float ambientStrength = 0.2;
  vec3 ambient = ambientStrength * light.ambient;
  
  float specularStrength = 0.7;
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * light.diffuse;

  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(norm, halfwayDir), 0.0), gloss);
  vec3 specular = specularStrength * spec * light.specular;

  float shadow = PointShadowCalculation(FragPos, light.position);
  return ((1.0 - shadow) * (diffuse + specular)) + ambient;

}


void main()
{
  vec4 ObjectColor = texture(ourTexture, TexCoord);
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(ViewPos - FragPos);

  vec3 lighting = vec3(0.0);
  lighting += DirLightCalculation(dirLight, norm, viewDir);
  lighting += PointLightCalculation(pointLight, norm, viewDir);

  vec4 result = vec4(lighting, 1.0) * ObjectColor;
  FragColor = result;
}
