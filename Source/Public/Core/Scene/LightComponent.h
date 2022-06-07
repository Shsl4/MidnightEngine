#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/LinearColor.h>

class ENGINE_API LightComponent : public SceneComponent {

public:
        
    FORCEINLINE void setDiffuseColor(const LinearColor& color) {
        this->diffuseColor = color;
    }
    
    FORCEINLINE void setSpecularColor(const LinearColor& color) {
        this->specularColor = color;
    }
    
    FORCEINLINE LinearColor& getDiffuseColor() {
        return this->diffuseColor;
    }
    
    FORCEINLINE LinearColor& getSpecularColor() {
        return this->specularColor;
    }

protected:
    
    LinearColor diffuseColor = LinearColors::white;
    LinearColor specularColor = LinearColor::fromRGB(100.0f, 100.0f, 100.0f);
    
};