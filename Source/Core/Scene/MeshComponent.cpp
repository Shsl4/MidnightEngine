#include <Scene/MeshComponent.h>
#include <Rendering/Mesh.h>
#include <Math/Matrix4.h>

#include "Engine.h"

MaterialHandles createMaterialUniforms() {

    MaterialHandles handles{};
    handles.ambient = createUniform("m_ambient", bgfx::UniformType::Vec4);
    handles.diffuse = createUniform("m_diffuse", bgfx::UniformType::Vec4);
    handles.specular = createUniform("m_specular", bgfx::UniformType::Vec4);
    handles.texRef = createUniform("m_texRef", bgfx::UniformType::Vec4);
    return handles;

}

PointLightHandles createPointLightUniforms() {

    PointLightHandles handles{};
    handles.color = createUniform("l_color", bgfx::UniformType::Vec4);
    handles.position = createUniform("l_position", bgfx::UniformType::Vec4);
    handles.icle = createUniform("icle", bgfx::UniformType::Vec4);
    return handles;

}


DirectionalLightHandles createDirectionalLightUniforms() {

    DirectionalLightHandles handles{};
    handles.direction = createUniform("l_direction", bgfx::UniformType::Vec4);
    handles.ambient = createUniform("l_ambient", bgfx::UniformType::Vec4);
    handles.diffuse = createUniform("l_diffuse", bgfx::UniformType::Vec4);
    handles.specular = createUniform("l_specular", bgfx::UniformType::Vec4);
    return handles;

}

void MeshComponent::setUniforms() const {
    const float texRef[4] = {static_cast<float>(mat.hasTexture), mat.reflectance, 0.0f, 0.0f};
    const float pos[4] = {light.position.x, light.position.y, light.position.z, 1.0f};
    const float icle[4] = {light.intensity, light.attenuation.constant, light.attenuation.linear, light.attenuation.exponent};
    const float ambient[4] = {0.3f, 0.3f, 0.3f, 1.0f};

    setUniform(materialHandles.ambient, &mat.ambient);
    setUniform(materialHandles.diffuse, &mat.diffuse);
    setUniform(materialHandles.specular, &mat.specular);
    setUniform(materialHandles.texRef, texRef);

    /*
    bgfx::setUniform(dirLightHandles.direction, &dirLight.direction);
    bgfx::setUniform(dirLightHandles.ambient, &dirLight.ambient);
    bgfx::setUniform(dirLightHandles.diffuse, &dirLight.diffuse);
    bgfx::setUniform(dirLightHandles.specular, &dirLight.specular);
*/

    setUniform(pointLightHandles.color, &light.color);
    setUniform(pointLightHandles.position, pos);
    setUniform(pointLightHandles.icle, icle);
    setUniform(ambientLightHandle, ambient);

    bgfx::setTransform(Matrix4::modelMatrix(transform).data);

}


MeshComponent::MeshComponent(String const &name)
        : mat(LinearColors::white, LinearColors::white, LinearColors::white, false, 1.0f),
          light(LinearColor::fromRGB(155.0f, 39.0f, 227.0f), Vector3(0.0f, 2.0f, 0.0f), 5.0f, Attenuation(0.0f, 0.0f, 0.5f)) {

    this->mesh = Engine::getInstance()->getResourceLoader()->getMesh(name);
              
    this->ambientLightHandle = createUniform("ambientLight", bgfx::UniformType::Vec4);
    this->pointLightHandles = createPointLightUniforms();
    this->dirLightHandles = createDirectionalLightUniforms();
    this->materialHandles = createMaterialUniforms();

}

void MeshComponent::update(float deltaTime) {

    Super::update(deltaTime);

    transform.rotation.y += 20.0f * deltaTime;

}

void MeshComponent::construct(Transform const &relativeTransform) {

    Super::construct(relativeTransform);
    Vector3 lightPos = transform.position;
    lightPos.y += 3.0f;
    this->light = PointLight(LinearColor::fromRGB(155.0f, 39.0f, 227.0f), lightPos, 5.0f, Attenuation(0.0f, 0.0f, 0.5f));

}

MeshComponent::~MeshComponent() {
    
    destroy(ambientLightHandle);

    destroy(dirLightHandles.ambient);
    destroy(dirLightHandles.specular);
    destroy(dirLightHandles.diffuse);
    destroy(dirLightHandles.direction);

    destroy(materialHandles.ambient);
    destroy(materialHandles.specular);
    destroy(materialHandles.diffuse);
    destroy(materialHandles.texRef);

    destroy(pointLightHandles.color);
    destroy(pointLightHandles.icle);
    destroy(pointLightHandles.position);
    
}


void MeshComponent::render() {

    setIndexBuffer(mesh->indexBuffer);
    setVertexBuffer(0, mesh->vertexBuffer);
    
    setUniforms();

    submit(0, mesh->programHandle);
}
