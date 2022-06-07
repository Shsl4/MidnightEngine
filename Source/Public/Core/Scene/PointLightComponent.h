#pragma once

#include <Scene/LightComponent.h>

#include "Math/Vector4.h"

class ENGINE_API PointLightComponent : public LightComponent {

public:
    
    void update(float deltaTime) override;

    FORCEINLINE Vector4 getAttenuation() const {
        return { constant, linear, quadratic, 1.0f };
    }

    NODISCARD FORCEINLINE float getConstant() const { return constant; }
    NODISCARD FORCEINLINE float getLinear() const { return linear; }
    NODISCARD FORCEINLINE float getQuadratic() const { return quadratic; }

    void setConstant(const float value) { this->constant = value; }
    void setLinear(const float value) { this->linear = value; }
    void setQuadratic(const float value) { this->quadratic = value; }

private:

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
        
};
