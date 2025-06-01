#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "opengl_shader.hpp"
#include <memory>

class Camera {
private:
  const double MOVE_UNIT = 0.5f;
  const double ROTATE_UNIT = 1.0f;
  glm::vec3 cameraPos = glm::vec3(10.0f, 0.05f, 10.0f);
  glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
  glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  bool turningLeft, turningRight, turningUp, turningDown;
  bool movingForward, movingBackward, movingLeft, movingRight, movingUp,
      movingDown;
  double yaw = -90.0f;
  double pitch = 0.0f;

  void CalculateFront() {
    CameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    CameraFront.y = sin(glm::radians(pitch));
    CameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    glm::normalize(CameraFront);
    CameraRight = glm::normalize(glm::cross(CameraFront, WorldUp));
  }

public:
  glm::vec3 GetCameraPos() { return this->cameraPos; }
  glm::vec3 GetCameraFront() { return this->CameraFront; }
  void TurnLeft(bool isTurning) { this->turningLeft = isTurning; }
  void TurnRight(bool isTurning) { this->turningRight = isTurning; }
  void TurnUp(bool isTurning) { this->turningUp = isTurning; }
  void TurnDown(bool isTurning) { this->turningDown = isTurning; }
  void MoveForward(bool isMoving) { this->movingForward = isMoving; }
  void MoveBackward(bool isMoving) { this->movingBackward = isMoving; }
  void MoveLeft(bool isMoving) { this->movingLeft = isMoving; }
  void MoveRight(bool isMoving) { this->movingRight = isMoving; }
  void MoveUp(bool isMoving) { this->movingUp = isMoving; }
  void MoveDown(bool isMoving) { this->movingDown = isMoving; }
  void AddYaw(double deltaYaw) {
    this->yaw += deltaYaw;
    if (this->yaw > 360.0f) {
      this->yaw -= 360.0f;
    } else if (this->yaw < -360.0f) {
      this->yaw += 360.0f;
    }
    this->CalculateFront();
  }
  void AddPitch(double deltaPitch) {
    this->pitch += deltaPitch;
    if (this->pitch > 89.0f) {
      this->pitch = 89.0f;
    } else if (this->pitch < -89.0f) {
      this->pitch = -89.0f;
    }
    this->CalculateFront();
  }
  void UpdateCameraPosition() {
    if (turningLeft ^ turningRight) {
      if (turningLeft) {
        AddYaw(-ROTATE_UNIT);
      } else {
        AddYaw(ROTATE_UNIT);
      }
    }
    if (turningUp ^ turningDown) {
      if (turningUp) {
        AddPitch(-ROTATE_UNIT);
      } else {
        AddPitch(ROTATE_UNIT);
      }
    }
    if (movingForward ^ movingBackward) {
      if (movingForward) {
        cameraPos += (float)MOVE_UNIT * CameraFront;
      } else {
        cameraPos -= (float)MOVE_UNIT * CameraFront;
      }
    }
    if (movingLeft ^ movingRight) {
      if (movingLeft) {
        cameraPos -= (float)MOVE_UNIT * CameraRight;
      } else {
        cameraPos += (float)MOVE_UNIT * CameraRight;
      }
    }

    if (movingUp ^ movingDown) {
      if (movingUp) {
        cameraPos += (float)MOVE_UNIT * WorldUp;
      } else {
        cameraPos -= (float)MOVE_UNIT * WorldUp;
      }
    }
  }
  void SetValueToShader(std::shared_ptr<Shader> shader, int width, int height) {
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)width / height, 0.1f, 1000.0f);
    shader->SetUniformValue("projection", projection);

    glm::mat4 view =
        glm::lookAt(cameraPos, cameraPos + GetCameraFront(), WorldUp);
    shader->SetUniformValue("view", view);

    shader->SetUniformValue("ViewPos", this->cameraPos);
  }
};

#endif
