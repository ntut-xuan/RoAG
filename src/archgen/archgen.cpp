#include <spdlog/spdlog.h>

#include "archgen.hpp"
#include "glm/ext/vector_float3.hpp"
#include "ground.hpp"
#include <memory>

#define FLOOR_LEVEL 1

ArchGen::ArchGen() : IPlaneDrawable() {
  const std::string objDir = "./assets/obj/";
  const std::string textureDir = "./assets/texture/";
  const std::string groundTexture = textureDir + "ground.jpg";
  const std::string wallTexture = textureDir + "wall.jpg";
  glm::mat4 modelMatrix = glm::mat4(1.0f);
  modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
  for (int level = 1; level <= FLOOR_LEVEL; level++) {
    string groundName = to_string(level) + "F_ground";
    string wallName = to_string(level) + "F_wall";
    std::shared_ptr<Model> ground = std::make_shared<Model>(objDir + groundName + ".obj", groundTexture);
    std::shared_ptr<Model> wall = std::make_shared<Model>(objDir + wallName + ".obj", wallTexture);
    ground->SetModel(modelMatrix);
    wall->SetModel(modelMatrix);
    objects.emplace(groundName, ground);
    objects.emplace(wallName, wall);
  }

  glm::vec3 sunsetColor[3] = {
    glm::vec3(235.0, 90.0, 60.0) / 255.0f, 
    glm::vec3(223.0, 151.0, 85.0) / 255.0f,
    glm::vec3(231.0, 210.0, 131.0) / 255.0f
  };
  light = std::make_shared<Light>(sunsetColor[0], sunsetColor[1], sunsetColor[2],
                                  glm::vec3(-100.0, -50.0, 75.0), Light::DIRECTIONAL);
  pointLight = std::make_shared<Light>(glm::vec3(0.5), glm::vec3(0.5), glm::vec3(0.5),
                                           glm::vec3(9.0, 17.0, 68.0), Light::POINT);
}

void ArchGen::Update(std::shared_ptr<Window> window, std::shared_ptr<Shader> shader) {
  light->setShaderUniform(shader.get());
  pointLight->setShaderUniform(shader.get());
  for (auto &object : objects) {
    object.second->setShaderUniform(shader.get());
    object.second->render();
  }
}

void ArchGen::GenShadowMap(std::shared_ptr<Shader> shader, bool directional, Camera* camera) {
  if (directional) {
    light->setShadowShaderUniform(shader.get());
  }
  else {
    pointLight->setShadowShaderUniform(shader.get());
  }
  for (auto &object : objects) {
    object.second->setShaderUniform(shader.get());
    object.second->render();
  }
}

void ArchGen::Teardown() {
}
