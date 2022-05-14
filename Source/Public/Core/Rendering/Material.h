#pragma once
#include "Math/Vector4.h"

struct Material {
    
    Material() = default;

    Material(const LinearColor& ambientColor, const LinearColor& diffuseColor, const Vector4& specular,
    const Vector4& shininess) : ambientColor(ambientColor), diffuseColor(diffuseColor),
        specular(specular), shininess(shininess) {
        
    }

    void setShininess(float value) {
        this->shininess = Vector4(value);
    }
    
    LinearColor ambientColor = LinearColors::black;
    LinearColor diffuseColor = LinearColors::white;
    Vector4 specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
    Vector4 shininess = Vector4(16.0f);
    
};