#pragma once

#include <iostream>

struct Matrix4 {

public:

	Matrix4 operator+(const Matrix4& other);
	Matrix4 operator-(const Matrix4& other);
	Matrix4 operator*(const Matrix4& other);
	Matrix4 operator*(const float scale);

	void operator+=(const Matrix4& other);
	void operator-=(const Matrix4& other);
	void operator*=(const Matrix4& other);
	void operator*=(const float scale);

	bool operator==(const Matrix4& other);
	bool operator!=(Matrix4& other);

	void rotateZ(float radians);

	static Matrix4 orthographic(float near, float far, float bottom, float top, float left, float right);
	static Matrix4 empty();
	static Matrix4 identity();
	static Matrix4 fill(float value);

	void print();

	float data[4][4] = {};

private:


	Matrix4() {

	}

};