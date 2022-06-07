#pragma once

#include <Scene/LightComponent.h>

#include <Math/Vector4.h>

class SpotLightComponent : public LightComponent {
    
public:
   
    FORCEINLINE Vector4 getAttenuation() const {
        return { constant, linear, quadratic, 1.0f };
    }
   
    FORCEINLINE Vector4 getSpotlightData() const {
        return { cutoff, outerCutoff, 1.0f, 1.0f };
    }

    NODISCARD FORCEINLINE float getCutoff() const { return cutoff; }
    NODISCARD FORCEINLINE float getOuterCutoff() const { return outerCutoff; }
    NODISCARD FORCEINLINE float getConstant() const { return constant; }
    NODISCARD FORCEINLINE float getLinear() const { return linear; }
    NODISCARD FORCEINLINE float getQuadratic() const { return quadratic; }
    NODISCARD FORCEINLINE Vector3 getLightDirection() const { return lightDirection; }
    
    void setCutoff(const float degrees) { this->cutoff = cos(Math::toRadians(degrees)); }
    void setOuterCutoff(const float degrees) { this->outerCutoff = cos(Math::toRadians(degrees)); }
    void setConstant(const float value) { this->constant = value; }
    void setLinear(const float value) { this->linear = value; }
    void setQuadratic(const float value) { this->quadratic = value; }
    void setLightDirection(Vector3 const& direction) { this->lightDirection = direction; }

private:

    float cutoff = cos(Math::toRadians(25.0f));
    float outerCutoff = cos(Math::toRadians(35.0f));
    
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    Vector3 lightDirection = Vector3(0.0f, -1.0f, 0.0f);
    
};
