#include "model.h"

Model::Model(const std::string filename, const std::string texture) {
  this->SetObject(filename, texture);
  this->SetModel(glm::mat4(1.0f));
}

void Model::SetObject(std::string filename, std::string texture) {
  object = std::make_unique<Object>(filename);
  object->load_to_buffer();
  object->load_texture(texture);
}

void Model::setShaderUniform(Shader* shader) {
  shader->SetUniformValue("model", modelMatrix);
  shader->SetUniformValue("normalMatrix", normalMatrix);
}

void Model::render() {
  if (!object) {
    return;
  }
  object->render();
}
