#include <spdlog/spdlog.h>

#include <glad/glad.h>
#ifdef __linux__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#include "plane_framework.hpp"
#include "opengl_glloader.hpp"

std::shared_ptr<PlaneFramework> PlaneFramework::instance = nullptr;
std::shared_ptr<PlaneFramework> PlaneFramework::GetInstance() {
    if (instance == nullptr) {
      instance = std::shared_ptr<PlaneFramework>(new PlaneFramework());
    }
    return instance;
}
PlaneFramework::PlaneFramework() {
    spdlog::info("PlaneFramework: Constructor");
    sceneShader = nullptr;
    dirShadowShader = nullptr;
    pointShadowShader = nullptr;
    camera = nullptr;
    window = nullptr;
}


void PlaneFramework::Init(){
    spdlog::info("PlaneFramework: Init Phase");
    sceneShader = std::make_shared<Shader>();
    dirShadowShader = std::make_shared<Shader>();
    pointShadowShader = std::make_shared<Shader>();
    shadowMap = std::make_shared<ShadowMap>();
    camera = std::make_shared<Camera>();
    window = std::make_shared<Window>(camera);
    window->Init(WINDOW_WIDTH, WINDOW_HEIGHT);
    GLLoader::Init();
    sceneShader->Init("blinn-phong");
    dirShadowShader->Init("dir-shadow");
    pointShadowShader->Init("point-shadow", true);
    shadowMap->Init();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
}

void PlaneFramework::Update(std::shared_ptr<IPlaneDrawable> drawable){
    spdlog::info("PlaneFramework: Update Phase");
    while(!window->IsWindowShouldClose()){
        
        camera->UpdateCameraPosition();

        dirShadowShader->Use();
        shadowMap->GenDirectionSetup(); 
        drawable->GenShadowMap(dirShadowShader, true, camera.get());
        shadowMap->GenDirectionCleanup();

        pointShadowShader->Use();
        shadowMap->GenPointSetup();
        drawable->GenShadowMap(pointShadowShader, false, camera.get());
        shadowMap->GenPointCleanup();

        sceneShader->Use();
        window->SetupWindowPerference(WINDOW_WIDTH, WINDOW_HEIGHT);

        camera->SetValueToShader(sceneShader, WINDOW_WIDTH, WINDOW_HEIGHT);
        shadowMap->SetShaderUniform(sceneShader.get());
        
        drawable->Update(window, sceneShader);
        
        window->Draw();
    }
}

void PlaneFramework::Teardown(){
    spdlog::info("PlaneFramework: Teardown Phase");
    window->Teardown();
}

