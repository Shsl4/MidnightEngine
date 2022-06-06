#pragma once

#include <bgfx/bgfx.h>

#define MAX_LIGHTS 10

class Uniforms {
    
public:
    
    Uniforms() = delete;
 
    static inline bgfx::UniformHandle materialAmbient {};
    static inline bgfx::UniformHandle materialDiffuse {};
    static inline bgfx::UniformHandle materialSpecular {};
    static inline bgfx::UniformHandle materialShininess {};
    static inline bgfx::UniformHandle viewPosition {};
    static inline bgfx::UniformHandle textureHandle {};
    static inline bgfx::UniformHandle lightPosition {};
    static inline bgfx::UniformHandle lightDirection {};
    static inline bgfx::UniformHandle lightAmbient {};
    static inline bgfx::UniformHandle lightDiffuse {};
    static inline bgfx::UniformHandle lightSpecular {};
    static inline bgfx::UniformHandle lightAttenuation {};
    static inline bgfx::UniformHandle lightData {};

private:

    friend class ResourceLoader;

    static inline bool constructed = false;

    static void makeUniforms() {

        if (constructed) { return; }

        materialAmbient = createUniform("matAmbient", bgfx::UniformType::Vec4);
        materialDiffuse = createUniform("matDiffuse", bgfx::UniformType::Vec4);
        materialSpecular = createUniform("matSpecular", bgfx::UniformType::Vec4);
        materialShininess = createUniform("matShininess", bgfx::UniformType::Vec4);
        viewPosition = createUniform("viewPos", bgfx::UniformType::Vec4);
        textureHandle = createUniform("texDiffuse", bgfx::UniformType::Sampler);
        lightPosition = createUniform("lightPos", bgfx::UniformType::Vec4, MAX_LIGHTS);
        lightDirection = createUniform("lightDirection", bgfx::UniformType::Vec4, MAX_LIGHTS);
        lightAmbient = createUniform("lightAmbientColor", bgfx::UniformType::Vec4, MAX_LIGHTS);
        lightDiffuse = createUniform("lightDiffuseColor", bgfx::UniformType::Vec4, MAX_LIGHTS);
        lightSpecular = createUniform("lightSpecularColor", bgfx::UniformType::Vec4, MAX_LIGHTS);
        lightAttenuation = createUniform("lightAttenuation", bgfx::UniformType::Vec4, MAX_LIGHTS);
        lightData = createUniform("lightData", bgfx::UniformType::Vec4);

        constructed = true;

    }

    static void destroyUniforms() {

        if (!constructed) { return; }
        
        destroy(materialAmbient);
        destroy(materialDiffuse);
        destroy(materialSpecular);
        destroy(materialShininess);
        destroy(viewPosition);
        destroy(textureHandle);
        destroy(lightPosition);
        destroy(lightDirection);
        destroy(lightAmbient);
        destroy(lightDiffuse);
        destroy(lightSpecular);
        destroy(lightAttenuation);
        destroy(lightData);
        
        constructed = false;
        
    }
    
};