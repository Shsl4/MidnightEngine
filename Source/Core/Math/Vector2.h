#pragma once

/*!
 *  A 2 dimensional vector structure.
 */
struct Vector2 {

    Vector2() = default;

    explicit Vector2(float v) : x(v), y(v) {

    }

    Vector2(float x, float y) : x(x), y(y) {

    }

    Vector2 operator+(Vector2 const &other);

    Vector2 operator-(Vector2 const &other);

    float x = 0.0f;
    float y = 0.0f;

};
