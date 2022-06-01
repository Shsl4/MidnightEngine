#pragma once

#include <Scene/LightComponent.h>

#include "Engine.h"

class ENGINE_API DirectionalLightComponent : public LightComponent {

public:

    FORCEINLINE Vector3 getLightDirection() const {
        return lightDirection;
    }
    
    void FORCEINLINE setLightDirection(Vector3 const& direction) {
        this->lightDirection = direction;
    }

    void update(float deltaTime) override {
        
        SceneComponent::update(deltaTime);

        const float time = Engine::getInstance()->getTime();
    
        this->diffuseColor.setRed(abs(sin(time)));
        this->diffuseColor.setGreen(abs(cos(time / 2.0f)));
        this->diffuseColor.setBlue(abs(sin(time / 3.0f)));
        
    }
    
protected:

    Vector3 lightDirection = Vector3(0.5f, -1.0f, 0.0f);
    
};
