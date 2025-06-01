#include "shadow_map.h"
#include <cstddef>

void ShadowMap::Init() {

  glGenFramebuffers(1, &directionDepthMapFBO);
  glGenFramebuffers(1, &pointDepthMapFBO);
  glGenTextures(1, &directionDepthMap);
  glGenTextures(1, &pointDepthMap);

  glBindTexture(GL_TEXTURE_2D, directionDepthMap);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH,
               SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glBindFramebuffer(GL_FRAMEBUFFER, directionDepthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         directionDepthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, pointDepthMap);
  for (unsigned int i = 0; i < 6; i++) {
    // for each face
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 NULL);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glBindFramebuffer(GL_FRAMEBUFFER, pointDepthMapFBO);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, pointDepthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::GenDirectionSetup() {
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, directionDepthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::GenDirectionCleanup() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::GenPointSetup() {
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, pointDepthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::GenPointCleanup() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::SetShaderUniform(Shader* shader) {
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, directionDepthMap);
  shader->SetUniformValue("dirShadowMap", 1);

  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_CUBE_MAP, pointDepthMap);
  shader->SetUniformValue("pointShadowMap", 2);
}
