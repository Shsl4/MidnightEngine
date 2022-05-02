#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>
#include <Rendering/LinearColor.h>
#include <bgfx/bgfx.h>
#include <Math/Vector4.h>

struct PointLight {

    bgfx::UniformHandle lightPos;
    bgfx::UniformHandle lightColor;
    bgfx::UniformHandle viewPos;
    bgfx::UniformHandle objectColor;
};

/*!
 * A renderable component that represents a mesh.
 */
class ENGINE_API MeshComponent : public SceneComponent, public Renderable {

public:

    explicit MeshComponent(String const &name);

    void update(float deltaTime) override;

    void construct(Transform const &relativeTransform) override;

    ~MeshComponent() override;


private:

    using Super = SceneComponent;

    void render() override;

    const struct Mesh *mesh;

    PointLight light;

};

