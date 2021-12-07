#pragma once

#include <Math/MathUtils.h>

struct Vector3 {

public:

	Vector3() : x(0.0f), y(0.0f), z(0.0f){
	}

	Vector3(float v) : x(v), y(v), z(v) {
	}

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {
	}

	Vector3 operator-(Vector3& other);
	Vector3 operator+(Vector3& other);
	Vector3 operator*(const float scale);
	Vector3 operator/(const float scale);

	void operator-=(Vector3& other);
	void operator+=(Vector3& other);
	void operator*=(const float scale);
	void operator/=(const float scale);

	FORCEINLINE static Vector3 up() {

		return Vector3(0.0f, 1.0f, 0.0f);

	}

	FORCEINLINE float dot(Vector3 other) {

		return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
		
	}

	FORCEINLINE void normalize() {

		float norm = this->norm();

		if (norm == 0.0f) { return; }

		*this /= norm;

	}

	FORCEINLINE float norm() {

		return sqrt((x * x) + (y * y) + (z * z));

	}

	FORCEINLINE void negate() {

		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;

	}

	FORCEINLINE Vector3 cross(Vector3 other) {

		Vector3 vec;

		vec.x = this->y * other.z - this->z * other.y;
		vec.y = this->z * other.x - this->x * other.z;
		vec.z = this->x * other.y - this->y * other.x;

		return vec;

	}

	FORCEINLINE static Vector3 cross(Vector3 a, Vector3 b) {

		Vector3 vec;

		vec.x = a.y * b.z - a.z * b.y;
		vec.y = a.z * b.x - a.x * b.z;
		vec.z = a.x * b.y - a.y * b.x;

		return vec;

	}

	FORCEINLINE static Vector3 normalize(Vector3 other) {

		float norm = other.norm();

		if (norm == 0.0f) { return other; }

		return other / norm;

	}

	FORCEINLINE static float dot(Vector3 a, Vector3 b) {

		return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);

	}

	FORCEINLINE static float angle(Vector3 a, Vector3 b) {

		float dp = Vector3::dot(a, b);
		float aNorm = a.norm();
		float bNorm = b.norm();

		return acos(dp / (aNorm * bNorm));

	}

	float x;
	float y;
	float z;

	static const Vector3 Zero;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Forward;
	static const Vector3 Backward;
	static const Vector3 Right;
	static const Vector3 Left;

    
};