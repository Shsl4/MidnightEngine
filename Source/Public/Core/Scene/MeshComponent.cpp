#include <Scene/MeshComponent.h>
#include <Rendering/Mesh.h>
#include <Math/Matrix4.h>

#include "Engine.h"

PointLight createMaterialUniforms() {

    PointLight light{};
    light.lightPos = bgfx::createUniform("lightPos", bgfx::UniformType::Vec4);// Vector3(1.2f, 1.0f, 2.0f);
    light.lightColor = bgfx::createUniform("lightColor", bgfx::UniformType::Vec4); //Vector3(155.0f, 39.0f, 227.0f);
    light.objectColor = bgfx::createUniform("objectColor", bgfx::UniformType::Vec4); //Vector3(1.0f, 0.5f, 0.31f);
    light.viewPos = createUniform("viewPos", bgfx::UniformType::Vec4);
    return light;

}

void MeshComponent::update(float deltaTime) {

    Super::update(deltaTime);

    transform.rotation.y += 20.0f * deltaTime;

}

MeshComponent::MeshComponent(String const& name) {

    this->mesh = Engine::getInstance()->getResourceLoader()->getMesh(name);
    this->light = createMaterialUniforms();

}

void MeshComponent::construct(Transform const &relativeTransform) {

    Super::construct(relativeTransform);
}

MeshComponent::~MeshComponent() {

    destroy(light.lightPos);
    destroy(light.lightColor);
    destroy(light.objectColor);
    destroy(light.viewPos);
    
}


void MeshComponent::render() {

    // If a mesh is set
    if (mesh){

        // Render it.
        mesh->use();
        auto a = Vector4(1.2f, 1.0f, 2.0f,1);
        auto b = Vector4(155.0f, 39.0f, 227.0f,1);
        auto c = Vector4(1.0f, 0.5f, 0.31f,1);
        auto d = getScene()->getCameraManager()->getActiveCamera()->getWorldPosition();
        bgfx::setUniform(light.lightPos, &a);
        bgfx::setUniform(light.lightColor, &b);
        bgfx::setUniform(light.objectColor, &c);
        bgfx::setUniform(light.viewPos, &d);
        bgfx::setTransform(Matrix4::modelMatrix(transform).data);
        mesh->submit();

    }

}
