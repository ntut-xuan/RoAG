#include "../core/plane_drawable.hpp"
#include "../core/opengl_drawable.hpp"
#include "model.h"
#include "light.h"
#include <memory>
#include <unordered_map>

class ArchGen : public IPlaneDrawable {
private:
    std::unordered_map<std::string, std::shared_ptr<Model>> objects;
    std::shared_ptr<Drawable> ground;
    std::shared_ptr<Light> light;
    std::shared_ptr<Light> pointLight;
public:
    ArchGen();
    virtual void Update(std::shared_ptr<Window> window, std::shared_ptr<Shader> shader) override;
    virtual void GenShadowMap(std::shared_ptr<Shader> shader, bool directional, Camera* camera) override;
    virtual void Teardown() override;
};
