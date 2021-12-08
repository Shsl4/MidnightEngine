#pragma once

#include <iostream>
#include <Math/Vector3.h>

struct Matrix3 {

public:

	Matrix3 operator+(const Matrix3& other);
	Matrix3 operator-(const Matrix3& other);
	Matrix3 operator*(const Matrix3& other);
	Matrix3 operator*(const float scale);
	Vector3 operator*(const Vector3 other);

	void operator+=(const Matrix3& other);
	void operator-=(const Matrix3& other);
	void operator*=(const Matrix3& other);
	void operator*=(const float scale);

	void operator=(const float newData[3][3]);

	bool operator==(const Matrix3& other);
	bool operator!=(Matrix3& other);

	static Matrix3 empty();
	static Matrix3 identity();
	static Matrix3 fill(float value);

	float data[3][3] = {};
	void print();

private:


	Matrix3() {

	}
};