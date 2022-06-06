﻿#include <Rendering/Model.h>

#include <Rendering/ShaderManager.h>

#include <Rendering/ShaderPrograms.h>
#include <Rendering/Uniforms.h>
#include <Scene/PointLightComponent.h>
#include <Scene/DirectionalLightComponent.h>

Model::Model(Array<SharedPointer<Mesh>> meshes, Array<SharedPointer<Texture>> textures, String name) :
    modelName(std::move(name)), boundTextures(std::move(textures)), meshes(std::move(meshes)) {

    for (const auto& e : this->meshes) {
        defaultHandles += ShaderPrograms::materialShader;
        defaultMaterials += Material();
    }
    
}

Array<WeakPointer<Texture>> Model::getTextures() const {
    
    auto textures = Array<WeakPointer<Texture>>(this->boundTextures.getSize());

    for (const auto& texture : boundTextures) {
            
        textures += texture.weak();
        
    }

    return textures;

}

void Model::render(UInt16 viewId,
                   Vector4 const& view,
                   Matrix4 const& transform,
                   Array<WeakPointer<Texture>> const& textures,
                   Array<Material> const& materials,
                   Array<bgfx::ProgramHandle> const& programs) const {


    Array<LightComponent*> lights = Engine::getInstance()->getActiveScene()->lights;
    const auto size = static_cast<UInt16>(lights.getSize());
    
    auto lightsPositions = Array<Vector4>(lights.getSize());
    auto lightsAmbients = Array<LinearColor>(lights.getSize());
    auto lightsDiffuses = Array<LinearColor>(lights.getSize());
    auto lightsSpecular = Array<LinearColor>(lights.getSize());
    auto lightsDirections = Array<Vector4>(lights.getSize());
    auto attenuations = Array<Vector4>(lights.getSize());
    
    for (auto* light : lights) {

        lightsPositions += Vector4(light->getWorldPosition());
        lightsAmbients += LinearColors::black;
        lightsDiffuses += light->getDiffuseColor();
        lightsSpecular += light->getSpecularColor();
        
        if (const auto* directionalLight = light->cast<DirectionalLightComponent>()) {
            
            lightsDirections += Vector4(directionalLight->getLightDirection());
            attenuations += Vector4(0.0f);
            
        }

        if (const auto* directionalLight = light->cast<PointLightComponent>()) {

            lightsDirections += Vector4(0.0f);
            attenuations += directionalLight->getAttenuation();
            
        }
            
    }

    size_t count = 0;
    
    for (auto const& mesh : meshes) {
        
        setUniform(Uniforms::viewPosition, &view);
        bgfx::setTransform(&transform);

        if (size > 0) {
            
            setUniform(Uniforms::lightPosition, lightsPositions.begin(), size);
            setUniform(Uniforms::lightAmbient, lightsAmbients.begin(), size);
            setUniform(Uniforms::lightDiffuse, lightsDiffuses.begin(), size);
            setUniform(Uniforms::lightSpecular, lightsSpecular.begin(), size);
            setUniform(Uniforms::lightDirection, lightsDirections.begin(), size);
            setUniform(Uniforms::lightAttenuation, attenuations.begin(), size);
            
        }
        
        auto lightCount = Vector4(size, 1.0f, 1.0f, 1.0f);

        setUniform(Uniforms::lightData, &lightCount);

        if (programs[count].idx == ShaderPrograms::wireframeShader.idx) {
            
            constexpr UInt64 state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                    BGFX_STATE_WRITE_Z | BGFX_STATE_MSAA | BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);

            bgfx::setState(state);
            
        }
        else {

            // Set our renderer state properties.
            constexpr UInt64 state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                    BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_MSAA;

            bgfx::setState(state);
            
        }
        
        mesh->render(viewId, materials[count], textures[count].valid() ? textures[count].raw() : nullptr, programs[count]);
        ++count;
        
    }   
    
}
