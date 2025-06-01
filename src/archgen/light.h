#ifndef LIGHT_H
#define LIGHT_H
#include "../core/opengl_shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>

class Light {
public:
  enum lightMode { ERROR, DIRECTIONAL, POINT };

private:
  glm::vec3 position;
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 direction;
  std::array<glm::mat4, 6> lightSpaceMatrixes;
  lightMode mode;
  void calculateLightSpaceMatrixes() {
    if (mode == ERROR) {
      return;
    }
    else if (mode == DIRECTIONAL) {
      glm::mat4 lightProjection =
        glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, -150.0f, 150.0f);
      glm::mat4 lightView = glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f),
                                        glm::vec3(0.0f, 1.0f, 0.0f));
      lightSpaceMatrixes[0] = lightProjection * lightView;
    } else if (mode == POINT) {
      float aspect = 1;
      float near = 1.0f;
      float far = 25.0f;
      glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);  
      lightSpaceMatrixes[0] = glm::lookAt(position, position + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
      lightSpaceMatrixes[1] = glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0));
      lightSpaceMatrixes[2] = glm::lookAt(position, position + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
      lightSpaceMatrixes[3] = glm::lookAt(position, position + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0));
      lightSpaceMatrixes[4] = glm::lookAt(position, position + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0));
      lightSpaceMatrixes[5] = glm::lookAt(position, position + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0));
      for (int i = 0; i < 6; ++i) {
        lightSpaceMatrixes[i] = shadowProj * lightSpaceMatrixes[i];
      }
    }
  }

public:
  void setup(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
             glm::vec3 specular, glm::vec3 direction) {
    this->position = position;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->direction = direction;
    calculateLightSpaceMatrixes();
  }
  Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
        glm::vec3 posOrDir, lightMode mode) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    if (mode == DIRECTIONAL) {
      this->direction = glm::normalize(posOrDir);
    } else if (mode == POINT) {
      this->position = posOrDir;
    }
    this->mode = mode;
    calculateLightSpaceMatrixes();
  }
  Light() {
    this->ambient = glm::vec3(0.0f);
    this->diffuse = glm::vec3(0.0f);
    this->specular = glm::vec3(0.0f);
    this->position = glm::vec3(0.0f);
    this->direction = glm::vec3(0.0f);
    mode = ERROR;
  }
  void SetPosition(glm::vec3 position) {
    if (mode != POINT) {
      return; // Only point lights can change position
    }
    this->position = position;
    calculateLightSpaceMatrixes();
  }
  void setShaderUniform(Shader* shader);
  void setShadowShaderUniform(Shader* shader);
};

#endif // !LIGHT_H
