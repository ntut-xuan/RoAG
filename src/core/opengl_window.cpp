#include <spdlog/spdlog.h>
#include <memory>
#include <spdlog/spdlog.h>

#include <glad/glad.h>

#include "opengl_window.hpp"
#include "plane_framework.hpp"

void Window::KeyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  bool val = action == GLFW_PRESS || action == GLFW_REPEAT;
  auto framework = PlaneFramework::GetInstance();
  auto camera = framework->GetCameraPtr();
  // camera movement

  // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE.
  // Events with GLFW_PRESS and GLFW_RELEASE actions are emitted for every key
  // press. Most keys will also emit events with GLFW_REPEAT actions while a key
  // is held down. https://www.glfw.org/docs/3.3/input_guide.html

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (key == GLFW_KEY_W) {
    camera->MoveForward(val);
  }
  if (key == GLFW_KEY_A) {
    camera->MoveLeft(val);
  }
  if (key == GLFW_KEY_D) {
    camera->MoveRight(val);
  }
  if (key == GLFW_KEY_S) {
    camera->MoveBackward(val);
  }
  if (key == GLFW_KEY_SPACE) {
    camera->MoveUp(val);
  }
  if (key == GLFW_KEY_Z) {
    camera->MoveDown(val);
  }
  if (key == GLFW_KEY_Q) {
    camera->TurnLeft(val);
  }
  if (key == GLFW_KEY_E) {
    camera->TurnRight(val);
  }  
  if (key == GLFW_KEY_K) {
    camera->TurnDown(val);
  }
  if (key == GLFW_KEY_J) {
    camera->TurnUp(val);
  }
  if (key == GLFW_KEY_T) {
    spdlog::info("Camera Position: {}, {}, {}", 
                 camera->GetCameraPos().x, 
                 camera->GetCameraPos().y, 
                 camera->GetCameraPos().z);
  }
}


void Window::SetupWindowPerference(int width, int height) {
    glViewport(0, 0, width, height);
    glClearColor(135 / 255.0, 206 / 255.0, 235 / 255.0, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::FrameBufferSizeCallback(GLFWwindow* window, int width, int height){
    auto framework = PlaneFramework::GetInstance();
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    framework->setWindowSize(width, height);
}

void Window::Init(int width, int height) {
    spdlog::info("Window: Init Phase");

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(width, height, "ArchGen", NULL, NULL);

    if (window == NULL) {
        spdlog::error("Error: Create windows failed.");
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, Window::KeyCallback);
    glfwSetFramebufferSizeCallback(window, Window::FrameBufferSizeCallback);

    this->window = window;
}

