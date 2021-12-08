#pragma once

#include <Math/MathUtils.h>
#include <math.h>

struct Vector4 {

public:

	Vector4 operator*(const float scale);
	Vector4 operator+(const Vector4& other);
	Vector4 operator-(const Vector4& other);
	Vector4 operator/(const float scale);

	void operator-=(Vector4& other);
	void operator+=(Vector4& other);
	void operator*=(const float scale);
	void operator/=(const float scale);

	FORCEINLINE float dot(Vector4 other)
	{
		return (this->x * other.x + this->y * other.y + this->z * other.z + this->w + other.w);
	}

	FORCEINLINE void normalize()
	{
		float norm = this->norm();

		if (norm==0){ return; }
		
		*this/=norm;
	}

	FORCEINLINE float norm() {
		
		return (sqrt(x*x)+sqrt(y*y)+sqrt(z*z)+sqrt(w*w));
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