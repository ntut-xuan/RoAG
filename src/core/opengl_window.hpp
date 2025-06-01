#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "camera.hpp"
#include <GLFW/glfw3.h>
#include <memory>

class Window {
private:
    GLFWwindow* window;
    std::shared_ptr<Camera> camera;
    static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
public:
    Window(std::shared_ptr<Camera> camera) : camera(camera){
        
    }
    void Init(int width, int height);
    bool IsWindowShouldClose() { return glfwWindowShouldClose(window); };
    void SetupWindowPerference(int width, int height);
    void Draw(){
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    std::shared_ptr<Camera> GetCamera() {return camera;}
    void Teardown() { 
        glfwTerminate(); 
    };
};

#endif
