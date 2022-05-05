#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>
#include <Rendering/LinearColor.h>
#include <Math/Matrix4.h>
#include <bgfx/bgfx.h>
#include <Math/Vector4.h>

struct DirectionalLight {

    bgfx::UniformHandle lightPos;
    bgfx::UniformHandle lightColor;
    bgfx::UniformHandle viewPos;
    bgfx::UniformHandle objectColor;
    bgfx::UniformHandle model;
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

    DirectionalLight light;

    Matrix4 model;
    
};

