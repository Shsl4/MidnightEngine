#pragma once

#include <Math/Vector3.h>

/*!
 *  A structure containing information for transformation in space. Transforms contains position, rotation and scale information.
 */
struct Transform {

    Transform() = default;

    explicit Transform(Vector3 const &position) : position(position), rotation(), scale(1.0f) {

    }

    Transform(Vector3 const &position, Vector3 const &rotation) : position(position), rotation(rotation), scale(1.0f) {

    }

    Transform(Vector3 const &position, Vector3 const &rotation, Vector3 const &scale)
            : position(position), rotation(rotation), scale(scale) {

    }

    Vector3 position = Vector3();
    Vector3 rotation = Vector3();
    Vector3 scale = Vector3(1.0f);

};
