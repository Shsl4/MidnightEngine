#pragma once

#include <Math/MathUtils.h>
#include <math.h>

struct Vector4 {
	
	Vector4 operator*(float scale) const;
	Vector4 operator/(float scale) const;
	
	Vector4 operator+(Vector4 const& other) const;
	Vector4 operator-(Vector4 const& other) const;

	void operator-=(Vector4 const& other);
	void operator+=(Vector4 const& other);
	void operator*=(float scale);
	void operator/=(float scale);

	FORCEINLINE float dot(Vector4 const& other) const
	{
		return (this->x * other.x + this->y * other.y + this->z * other.z + this->w + other.w);
	}

	FORCEINLINE void normalize()
	{
		const float norm = this->norm();

		if (norm == 0){ return; }
		
		*this/=norm;
	}

	FORCEINLINE float norm() const
	{
		return sqrt((x * x) + (y * y) + (z * z) + (w * w));
	}

		
	Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {
	}

	explicit Vector4(const float v) : x(v), y(v), z(v), w(v) {
	}

	Vector4(const float x, const float y, const float z,const float w) : x(x), y(y), z(z), w(w) {
	}

	float x;
	float y;
	float z;
	float w;

};
