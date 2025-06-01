#ifndef SHADOW_MAP_H
#define SHADOW_MAP_H
#include <glad/glad.h>
#include "opengl_shader.hpp"

class ShadowMap {
  unsigned int directionDepthMapFBO;
  unsigned int directionDepthMap;
  unsigned int pointDepthMapFBO;
  unsigned int pointDepthMap;

  const unsigned int SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;

public:
  void Init();
  void GenDirectionSetup();
  void GenDirectionCleanup();
  void GenPointSetup();
  void GenPointCleanup();
  void SetShaderUniform(Shader* shader);
};
#endif // SHADOW_MAP_H
