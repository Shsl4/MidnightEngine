#pragma once

#include <Math/MathUtils.h>
#include <math.h>
#include <assimp/vector3.h>

/*!
 *  A 3 dimensional vector structure containing operators and primitives.
 */
struct Vector3 {

    Vector3() = default;

    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {

    }
    
    Vector3(aiVector3D const& other) : x(other.x), y(other.y), z(other.z) {
        
    }

    explicit Vector3(const float v) : Vector3(v, v, v) {

    }

    bool operator==(Vector3 const &other) const;
    
    bool operator< (Vector3 const &other) const;

    Vector3 operator-(Vector3 const &other) const;

    Vector3 operator+(Vector3 const &other) const;

    Vector3 operator*(float scale) const;

    Vector3 operator/(float scale) const;

    void operator-=(Vector3 const &other);

    void operator+=(Vector3 const &other);

    void operator*=(float scale);

    void operator/=(float scale);

    FORCEINLINE float dot(Vector3 const &other) const {

        return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);

    }

    FORCEINLINE void normalize() {

        *this *= Math::fastInvSqrt((x * x) + (y * y) + (z * z));

    }

    FORCEINLINE float norm() const {

        return sqrt((x * x) + (y * y) + (z * z));

    }

    FORCEINLINE void negate() {

        this->x = -this->x;
        this->y = -this->y;
        this->z = -this->z;

    }

    FORCEINLINE Vector3 cross(Vector3 const &other) const {

        Vector3 vec;

        vec.x = this->y * other.z - this->z * other.y;
        vec.y = this->z * other.x - this->x * other.z;
        vec.z = this->x * other.y - this->y * other.x;

        return vec;

    }

    FORCEINLINE static Vector3 cross(Vector3 const &a, Vector3 const &b) {

        Vector3 vec;

        vec.x = a.y * b.z - a.z * b.y;
        vec.y = a.z * b.x - a.x * b.z;
        vec.z = a.x * b.y - a.y * b.x;

        return vec;

    }

    FORCEINLINE static Vector3 normalize(Vector3 const &other) {

        const float norm = other.norm();

        if (norm == 0.0f) {return other;}

        return other / norm;

    }

    FORCEINLINE static float dot(Vector3 const &a, Vector3 const &b) {

        return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);

    }

    FORCEINLINE static float angle(Vector3 const &a, Vector3 const &b) {

        const float dp = Vector3::dot(a, b);
        const float aNorm = a.norm();
        const float bNorm = b.norm();

        return acos(dp / (aNorm * bNorm));

    }

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    static const Vector3 zero;
    static const Vector3 up;
    static const Vector3 down;
    static const Vector3 forward;
    static const Vector3 backward;
    static const Vector3 right;
    static const Vector3 left;

};
