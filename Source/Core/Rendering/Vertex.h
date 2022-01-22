#pragma once

#include <Math/Vector3.h>
#include <Math/Vector2.h>
#include <Rendering/LinearColor.h>
#include <Memory/Array.h>

struct Vertex {

public:

    Vertex(float x, float y, float z, float r, float g, float b, float a)
            : position(Vector3(x, y, z)), color(LinearColor::fromLinearRGBA(r, g, b, a)) {
    }

    Vertex(Vector3 const &pos, Vector3 const &norm, Vector2 const &texCoords, LinearColor const &col)
            : position(pos), normal(norm), textureCoordinates(texCoords), color(col) {

    }

    Vertex(float x, float y, float z, LinearColor color) : position(Vector3(x, y, z)), color(color) {

    }

    Vertex(float x, float y, float z) : position(Vector3(x, y, z)), color(LinearColors::white) {

    }

    Vector3 position;
    Vector3 normal;
    Vector2 textureCoordinates;
    LinearColor color;

};
