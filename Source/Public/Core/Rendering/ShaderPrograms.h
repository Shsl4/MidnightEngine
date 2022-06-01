#pragma once

#include <bgfx/bgfx.h>

#include "Rendering/ShaderManager.h"

class ShaderPrograms {

public:

    ShaderPrograms() = delete;

    static inline bgfx::ProgramHandle unlit {};
    static inline bgfx::ProgramHandle basicShader {};
    static inline bgfx::ProgramHandle materialShader {};
    static inline bgfx::ProgramHandle textureShader {};
    static inline bgfx::ProgramHandle directionalLightShader {};
    static inline bgfx::ProgramHandle pointLightShader {};
    static inline bgfx::ProgramHandle skyboxShader {};
    
private:

    friend class ResourceLoader;

    static void makePrograms() {
        
        unlit = ShaderManager::loadProgram("Unlit");
        basicShader = ShaderManager::loadProgram("Basic");
        materialShader = ShaderManager::loadProgram("Material");
        textureShader = ShaderManager::loadProgram("Texture");
        directionalLightShader = ShaderManager::loadProgram("DirectionalLight");
        pointLightShader = ShaderManager::loadProgram("PointLight");
        skyboxShader = ShaderManager::loadProgram("Skybox");
        
    }

    static void destroyPrograms() {
        
        destroy(unlit);
        destroy(basicShader);
        destroy(materialShader);
        destroy(textureShader);
        destroy(directionalLightShader);
        destroy(pointLightShader);
        destroy(skyboxShader);
                
    }
        
};
