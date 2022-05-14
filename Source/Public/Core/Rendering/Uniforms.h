#pragma once

#include <bgfx/bgfx.h>

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
    static inline bgfx::UniformHandle lightAmbient {};
    static inline bgfx::UniformHandle lightDiffuse {};
    static inline bgfx::UniformHandle lightSpecular {};

private:

    friend class ResourceLoader;

    static void makeUniforms() {

        materialAmbient = createUniform("matAmbient", bgfx::UniformType::Vec4);
        materialDiffuse = createUniform("matDiffuse", bgfx::UniformType::Vec4);
        materialSpecular = createUniform("matSpecular", bgfx::UniformType::Vec4);
        materialShininess = createUniform("matShininess", bgfx::UniformType::Vec4);
        viewPosition = createUniform("viewPos", bgfx::UniformType::Vec4);
        textureHandle = createUniform("texDiffuse", bgfx::UniformType::Sampler);
        lightPosition = createUniform("lightPos", bgfx::UniformType::Vec4);
        lightAmbient = createUniform("lightAmbientColor", bgfx::UniformType::Vec4);
        lightDiffuse = createUniform("lightDiffuseColor", bgfx::UniformType::Vec4);
        lightSpecular = createUniform("lightSpecularColor", bgfx::UniformType::Vec4);

    }

    static void destroyUniforms() {

        destroy(materialAmbient);
        destroy(materialDiffuse);
        destroy(materialSpecular);
        destroy(materialShininess);
        destroy(viewPosition);
        destroy(textureHandle);
        destroy(lightPosition);
        destroy(lightAmbient);
        destroy(lightDiffuse);
        destroy(lightSpecular);
        
    }
    
};