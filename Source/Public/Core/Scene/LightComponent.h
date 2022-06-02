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
    
    FORCEINLINE LinearColor getDiffuseColor() const {
        return this->diffuseColor;
    }
    
    FORCEINLINE LinearColor getSpecularColor() const {
        return this->specularColor;
    }

protected:
    
    LinearColor diffuseColor = LinearColors::white;
    LinearColor specularColor = LinearColors::white;
    
};