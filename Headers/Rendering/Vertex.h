#pragma once

#include <Math/Vector3.h>
#include <Rendering/LinearColor.h>

struct Vertex {

public:

	Vertex(float x, float y, float z, float r, float g, float b, float a) : position(Vector3(x, y, z)), color(LinearColor::fromLinearRGBA(r, g, b, a)) {

	}

	Vector3 position;
	LinearColor color;

};