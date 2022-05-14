#pragma once

#include <Scene/SceneComponent.h>

#include "Rendering/LinearColor.h"

class ENGINE_API BasicLightComponent : public SceneComponent {

public:
    
    FORCEINLINE void setAmbientColor(const LinearColor& color) {
        this->ambientColor = color;
    }
    
    FORCEINLINE void setDiffuseColor(const LinearColor& color) {
        this->diffuseColor = color;
    }
    
    FORCEINLINE void setSpecularColor(const LinearColor& color) {
        this->specularColor = color;
    }

    FORCEINLINE NODISCARD LinearColor getAmbientColor() const {
        return this->ambientColor;
    }
    
    FORCEINLINE NODISCARD LinearColor getDiffuseColor() const {
        return this->diffuseColor;
    }
    
    FORCEINLINE NODISCARD LinearColor getSpecularColor() const {
        return this->specularColor;
    }

    void update(float deltaTime) override;

private:
    
    LinearColor ambientColor = LinearColor::fromLinearRGB(0.2f, 0.2f, 0.2f);
    LinearColor diffuseColor = LinearColors::white;
    LinearColor specularColor = LinearColors::white;
    
};
