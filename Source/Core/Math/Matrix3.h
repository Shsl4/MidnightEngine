#pragma once

#include <Math/Vector3.h>

struct Matrix3 {
	
	Matrix3 operator+(Matrix3 const& other) const;
	Matrix3 operator-(Matrix3 const& other) const;
	Matrix3 operator*(Matrix3 const& other) const;
	Matrix3 operator*(float scale) const;
	Vector3 operator*(Vector3 other) const;

    void operator=(Matrix3 const& other);

	void operator+=(Matrix3 const& other);
	void operator-=(Matrix3 const& other);
	void operator*=(Matrix3 const& other);
	void operator*=(float scale);

	void operator=(const float newData[3][3]);

	bool operator==(Matrix3 const& other) const;

	static Matrix3 empty();
	static Matrix3 identity();
	static Matrix3 fill(float value);

	float data[3][3] = {};
	void print() const;

	Matrix3() = default;
};
