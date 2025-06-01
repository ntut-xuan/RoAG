#ifndef I_PLANE_DRWABLE_HPP
#define I_PLANE_DRWABLE_HPP

#include "camera.hpp"
#include "opengl_window.hpp"
#include <memory>

class IPlaneDrawable {
public:
    virtual void Update(std::shared_ptr<Window> window, std::shared_ptr<Shader> shader) = 0;
    virtual void GenShadowMap(std::shared_ptr<Shader> shader, bool directional, Camera* camera) = 0;
    virtual void Teardown() = 0;
};

#endif
