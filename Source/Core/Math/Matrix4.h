#pragma once

#include <Core/EngineMacros.h>
#include <Math/Transform.h>
#include <Math/Vector4.h>

/*!
 * A 4x4 Matrix structure implementing operators and primivites.
 */
struct ENGINE_API Matrix4 {

    Matrix4();

    Matrix4 operator+(Matrix4 const &other) const;

    Matrix4 operator-(Matrix4 const &other) const;

    Matrix4 operator*(Matrix4 const &other) const;

    Matrix4 operator*(float scale) const;

    Vector3 operator*(Vector3 other) const;

    void operator+=(Matrix4 const &other);

    void operator-=(Matrix4 const &other);

    void operator*=(Matrix4 const &other);

    void operator*=(float scale);

    Matrix4 &operator=(const float newData[4][4]);

    bool operator==(Matrix4 const &other) const;

    Vector4 operator[](size_t i) const;

    void rotateX(float radians);

    void rotateY(float radians);

    void rotateZ(float radians);

    void rotate(float radians, Vector3 axis);

    void scale(Vector3 const &factor);

    void translate(Vector3 const &translate);

    static Matrix4 translation(Vector3 const &translate);

    static Matrix4 rotation(float radians, Vector3 axis);

    static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);

    static Matrix4 perspective(float fov, float aspectRatio, float near, float far);

    static Matrix4 lookAt(Vector3 eye, Vector3 at, Vector3 up = Vector3::up);

    static Matrix4 modelMatrix(Transform const &transform);

    static Matrix4 empty();

    static Matrix4 identity();

    static Matrix4 fill(float value);

    static Matrix4 copyingData(const float newData[4][4]);

    static Matrix4 copyingData(const float *newData);

    float data[4][4] = {};

};
