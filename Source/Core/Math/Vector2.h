#pragma once

#include <Core/EngineTypes.h>
#include <cmath>
#include <assimp/vector3.h>

/*!
 *  A 2 dimensional vector structure.
 */
struct Vector2 {

    Vector2() = default;

    explicit Vector2(float v) : x(v), y(v) {

    }

    Vector2(float x, float y) : x(x), y(y) {

    }
    
    explicit Vector2(aiVector3D const& other) : x(other.x), y(other.y) {
        
    }

    FORCEINLINE float norm() const{
        return sqrt(x * x + y * y);
    }
    
    bool operator==(Vector2 const &other) const;
    
    bool operator<(Vector2 const &other) const;

    Vector2 operator+(Vector2 const &other);

    Vector2 operator-(Vector2 const &other);

    float x = 0.0f;
    float y = 0.0f;
    
    static const Vector2 zero;

};
