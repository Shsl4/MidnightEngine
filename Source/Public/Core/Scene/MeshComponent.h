#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>
#include <Rendering/LinearColor.h>
#include <Math/Matrix4.h>
#include <bgfx/bgfx.h>
#include <Math/Vector4.h>

struct Material
{

    Material() {

        this->ambient = bgfx::createUniform("matAmbient", bgfx::UniformType::Vec4);
        this->textureDiffuse = bgfx::createUniform("texDiffuse", bgfx::UniformType::Sampler);
        this->diffuse = bgfx::createUniform("matDiffuse", bgfx::UniformType::Vec4);
        this->specular = bgfx::createUniform("matSpecular", bgfx::UniformType::Vec4);
        this->shininess = bgfx::createUniform("matShininess", bgfx::UniformType::Vec4);
        
    }

    ~Material()
    {
        destroy(this->ambient);
        destroy(this->textureDiffuse);
        destroy(this->diffuse);
        destroy(this->specular);
        destroy(this->shininess);
    }
    
    bgfx::UniformHandle ambient;
    bgfx::UniformHandle textureDiffuse;
    bgfx::UniformHandle diffuse;
    bgfx::UniformHandle specular;
    bgfx::UniformHandle shininess;
    
};

struct DefaultLight {

    DefaultLight() {
        
        this->lightPos = bgfx::createUniform("lightPos", bgfx::UniformType::Vec4);
        this->ambientColor = bgfx::createUniform("lightAmbientColor", bgfx::UniformType::Vec4);
        this->diffuseColor = bgfx::createUniform("lightDiffuseColor", bgfx::UniformType::Vec4);
        this->specularColor = bgfx::createUniform("lightSpecularColor", bgfx::UniformType::Vec4);

    }
    
    ~DefaultLight()
    {
        destroy(this->lightPos);
        destroy(this->ambientColor);
        destroy(this->diffuseColor);
        destroy(this->specularColor);
    }    
    
    bgfx::UniformHandle lightPos;
    bgfx::UniformHandle ambientColor;
    bgfx::UniformHandle diffuseColor;
    bgfx::UniformHandle specularColor;
    
};

/*!
 *  \brief A renderable component that represents a mesh.
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

    const struct Mesh *mesh = nullptr;
    const struct Texture *texture = nullptr;

    DefaultLight light;
    Material material;

    bgfx::UniformHandle viewPos;    

    Matrix4 model;
    
};

