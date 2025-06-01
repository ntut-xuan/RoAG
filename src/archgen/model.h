#ifndef MODEL_H
#define MODEL_H
#include "glm/glm.hpp"
#include "../core/opengl_shader.hpp"
#include "../core/object.h"
#include <memory>
#include <string>

class Model{
private:
  std::unique_ptr<Object> object;
  glm::mat4 modelMatrix;
  glm::mat3 normalMatrix;
public:
  void SetObject(std::unique_ptr<Object> obj) {
    object = std::move(obj);
  }
  void SetObject(Object* obj) {
    object.reset(obj);
  }
  void SetObject(const std::string filename, const std::string texture);
  Object* GetObject() const {
    return object.get();
  }
  void SetModel(glm::mat4 model) {
    modelMatrix = model;
    normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
  }
  glm::mat4 GetModel() const {
    return modelMatrix;
  }
  void setShaderUniform(Shader* shader);
  void render();
  Model() : object(nullptr), modelMatrix(glm::mat4(1.0f)), normalMatrix(glm::mat3(1.0f)) {}
  Model(const std::string filename, const std::string texture);
};
#endif // !MODEL_H
