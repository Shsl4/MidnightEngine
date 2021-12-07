#pragma once

struct Vector4 {

public:

	Vector4 operator*(const float scale) {

		return Vector4(x * scale, y * scale, z * scale, w * scale);

	}

	Vector4 operator+(const Vector4& other) {

		return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);

	}

	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {
	}

	Vector4(float v) : x(v), y(v), z(v), w(v) {
	}

	Vector4(float x, float y, float z,float w) : x(x), y(y), z(z), w(w) {
	}

	float x;
	float y;
	float z;
	float w;

};