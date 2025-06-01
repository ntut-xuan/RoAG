#ifndef PLANE_FRAMEWORK_HPP
#define PLANE_FRAMEWORK_HPP

#include "camera.hpp"
#include "shadow_map.h"
#include "opengl_shader.hpp"
#include "opengl_window.hpp"
#include "plane_drawable.hpp"
#include <memory>

class PlaneFramework {
private:
    std::shared_ptr<Window> window;
    std::shared_ptr<Shader> sceneShader, dirShadowShader, pointShadowShader;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<ShadowMap> shadowMap;

    int WINDOW_WIDTH = 1600;
    int WINDOW_HEIGHT = 900;
    // singleton
    PlaneFramework();
    PlaneFramework(const PlaneFramework&) = delete;
    PlaneFramework& operator=(const PlaneFramework&) = delete;
    static std::shared_ptr<PlaneFramework> instance;
public: 
    static std::shared_ptr<PlaneFramework> GetInstance(); 
    void setWindowSize(int width, int height) {
        WINDOW_WIDTH = width;
        WINDOW_HEIGHT = height;
    }
    int getWindowWidth() { return WINDOW_WIDTH; }
    int getWindowHeight() { return WINDOW_HEIGHT; }
    void Init();
    void Update(std::shared_ptr<IPlaneDrawable> drawable);
    std::shared_ptr<Shader> GetSceneShaderPtr() { return sceneShader; }
    std::shared_ptr<Camera> GetCameraPtr() { return camera; }
    void Teardown();
};

#endif
