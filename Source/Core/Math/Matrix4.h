#pragma once

#include <iostream>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

struct Matrix4 {

public:

	Matrix4 operator+(const Matrix4& other);
	Matrix4 operator-(const Matrix4& other);
	Matrix4 operator*(const Matrix4& other);
	Matrix4 operator*(const float scale);

	Vector3 operator*(const Vector3 other);

	void operator+=(const Matrix4& other);
	void operator-=(const Matrix4& other);
	void operator*=(const Matrix4& other);
	void operator*=(const float scale);

	void operator=(const float newData[4][4]);

	bool operator==(const Matrix4& other);
	bool operator!=(const Matrix4& other);

	Vector4 operator[](const int i);

	void rotateX(float radians);
	void rotateY(float radians);
	void rotateZ(float radians);
	void rotate(float radians, Vector3 axis);

	static Matrix4 rotation(float radians, Vector3 axis);
	static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);
	static Matrix4 perspective(float fov, float aspectRatio, float near, float far);
	static Matrix4 lookAt(Vector3 eye, Vector3 at, Vector3 up = Vector3::Up);

	static Matrix4 empty();
	static Matrix4 identity();
	static Matrix4 fill(float value);
	static Matrix4 copyingData(const float newData[4][4]);
	static Matrix4 copyingData(const float* newData);

	void print();

	float data[4][4] = {};

	Matrix4() {

	}

};
