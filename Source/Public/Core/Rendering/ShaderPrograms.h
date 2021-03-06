#pragma once

#include <bgfx/bgfx.h>

#include "Rendering/ShaderManager.h"

class ENGINE_API ShaderPrograms {

public:

    ShaderPrograms() = delete;

    static inline bgfx::ProgramHandle unlit {};
    static inline bgfx::ProgramHandle basicShader {};
    static inline bgfx::ProgramHandle materialShader {};
    static inline bgfx::ProgramHandle textureShader {};
    static inline bgfx::ProgramHandle directionalLightShader {};
    static inline bgfx::ProgramHandle pointLightShader {};
    static inline bgfx::ProgramHandle skyboxShader {};
    static inline bgfx::ProgramHandle wireframeShader {};
    
private:

    friend class ResourceLoader;

    static inline bool constructed = false;
    
    static void makePrograms() {

        if (constructed) { return; }
        
        unlit = ShaderManager::loadProgram("Unlit");
        basicShader = ShaderManager::loadProgram("Basic");
        materialShader = ShaderManager::loadProgram("Material");
        textureShader = ShaderManager::loadProgram("Texture");
        directionalLightShader = ShaderManager::loadProgram("DirectionalLight");
        pointLightShader = ShaderManager::loadProgram("PointLight");
        skyboxShader = ShaderManager::loadProgram("Skybox");
        wireframeShader = ShaderManager::loadProgram("Wireframe");

        constructed = true;
        
    }

    static void destroyPrograms() {

        if (!constructed) { return; }
        
        destroy(unlit);
        destroy(basicShader);
        destroy(materialShader);
        destroy(textureShader);
        destroy(directionalLightShader);
        destroy(pointLightShader);
        destroy(skyboxShader);
        destroy(wireframeShader);

        constructed = false;
                
    }
        
};
