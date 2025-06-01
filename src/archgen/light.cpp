#include "light.h"
#include <spdlog/spdlog.h>

void Light::setShaderUniform(Shader *shader) {
  if (this->mode == lightMode::ERROR) {
    spdlog::error("Light: Error light mode.");
    return;
  }
  std::string lightName = (this->mode == lightMode::DIRECTIONAL) ? "dirLight" : "pointLight";
  shader->SetUniformValue(lightName + ".ambient", ambient);
  shader->SetUniformValue(lightName + ".diffuse", diffuse);
  shader->SetUniformValue(lightName + ".specular", specular);

  if (this->mode == lightMode::DIRECTIONAL) {
    shader->SetUniformValue(lightName + ".direction", direction);
    shader->SetUniformValue(lightName + ".lightSpaceMatrix", this->lightSpaceMatrixes[0]);
  } else if (this->mode == lightMode::POINT) {
    shader->SetUniformValue(lightName + ".position", position);
    shader->SetUniformValue(lightName + ".farPlane", 25.0f);
  }
}

void Light::setShadowShaderUniform(Shader *shader) {
  if (this->mode == lightMode::ERROR) {
    spdlog::error("Light: Error light mode.");
    return;
  }
  if (this->mode == lightMode::DIRECTIONAL) {
    shader->SetUniformValue("lightSpaceMatrix", this->lightSpaceMatrixes[0]);
  } else if (this->mode == lightMode::POINT) {
    for (int i = 0; i < 6; ++i) {
      std::string name = "lightSpaceMatrix[" + std::to_string(i) + "]";
      shader->SetUniformValue(name, this->lightSpaceMatrixes[i]);
    }
    shader->SetUniformValue("lightPos", position);
    shader->SetUniformValue("farPlane", 25.0f);
  } else {
    spdlog::error("Light: Unsupported light mode for shadow mapping.");
    return;
  }
}
