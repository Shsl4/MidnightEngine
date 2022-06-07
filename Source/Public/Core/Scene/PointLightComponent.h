#pragma once

#include <Scene/LightComponent.h>

#include "Math/Vector4.h"

class ENGINE_API PointLightComponent : public LightComponent {

public:
    
    void update(float deltaTime) override;

    FORCEINLINE Vector4 getAttenuation() const {
        return { constant, linear, quadratic, 1.0f };
    }

    void setConstant(float value) {
        constant = value;
    }

private:

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
        
};
