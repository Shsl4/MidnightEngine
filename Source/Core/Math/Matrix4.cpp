#include <Math/Matrix4.h>
#include <cmath>

Matrix4 Matrix4::operator+(const Matrix4& other) {

	Matrix4 m;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m.data[i][j] = this->data[i][j] + other.data[i][j];
		}
	}

	return m;

}

Matrix4 Matrix4::operator-(const Matrix4& other) {

	Matrix4 m;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m.data[i][j] = this->data[i][j] - other.data[i][j];
		}
	}

	return m;

}

Matrix4 Matrix4::operator*(const Matrix4& other)
{
	Matrix4 newMatrix = Matrix4();

	for (size_t i = 0; i < 4; i++) {

		for (size_t j = 0; j < 4; j++)
		{
			
			newMatrix.data[i][j] = 0;

			for (size_t k = 0; k < 4; k++)
			{
				newMatrix.data[i][j] += this->data[k][j] * other.data[i][k];
			}

		}

	}

	return newMatrix;
}


Matrix4 Matrix4::operator*(const float scale) {

	Matrix4 m;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m.data[i][j] = this->data[i][j] * scale;
		}
	}

	return m;

}

void Matrix4::operator*=(const float scale) {

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			this->data[i][j] *= scale;
		}
	}

}

void Matrix4::operator*=(const Matrix4& other)
{
	*this = *this * other;
}

void Matrix4::operator+=(const Matrix4& other) {

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			this->data[i][j] += other.data[i][j];
		}
	}

}

void Matrix4::operator-=(const Matrix4& other) {

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			this->data[i][j] -= other.data[i][j];
		}
	}

}

bool Matrix4::operator==(const Matrix4& other) {

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (this->data[i][j] != other.data[i][j]) {

				return false;

			}

		}
	}

	return true;

}


bool Matrix4::operator !=(Matrix4& other) {

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (this->data[i][j] != other.data[i][j]) {

				return true;

			}

		}
	}

	return false;

}

void Matrix4::rotateX(float radians)
{

	float cosT = std::cos(radians);
	float sinT = std::sin(radians);

	Matrix4 newMatrix = Matrix4::identity();

	newMatrix.data[1][1] = cosT;
	newMatrix.data[1][2] = -sinT;
	newMatrix.data[2][1] = sinT;
	newMatrix.data[2][2] = cosT;

	*this *= newMatrix;

}

void Matrix4::rotateY(float radians)
{

	float cosT = std::cos(radians);
	float sinT = std::sin(radians);

	Matrix4 newMatrix = Matrix4::identity();

	newMatrix.data[0][0] = cosT;
	newMatrix.data[0][2] = sinT;
	newMatrix.data[2][0] = -sinT;
	newMatrix.data[2][2] = cosT;

	*this *= newMatrix;

}

void Matrix4::rotateZ(float radians)
{

	float cosT = std::cos(radians);
	float sinT = std::sin(radians);

	Matrix4 newMatrix = Matrix4::identity();

	newMatrix.data[0][0] = cosT;
	newMatrix.data[0][1] = sinT;
	newMatrix.data[1][0] = -sinT;
	newMatrix.data[1][1] = cosT;

	*this *= newMatrix;

}


// https://en.wikipedia.org/wiki/Orthographic_projection
Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float near, float far)
{

	Matrix4 newMatrix;

	newMatrix.data[0][0] = 2.0f / (right - left);
	newMatrix.data[0][1] = 0.0f;
	newMatrix.data[0][2] = 0.0f;
	newMatrix.data[0][3] = 0.0f;

	newMatrix.data[1][0] = 0.0f;
	newMatrix.data[1][1] = 2.0f / (top - bottom);
	newMatrix.data[1][2] = 0.0f;
	newMatrix.data[1][3] = 0.0f;

	newMatrix.data[2][0] = 0.0f;
	newMatrix.data[2][1] = 0.0f;
	newMatrix.data[2][2] = -2.0f / (far - near);
	newMatrix.data[2][3] = 0.0f;

	newMatrix.data[3][0] = -(right + left) / (right - left);
	newMatrix.data[3][1] = -(top + bottom) / (top - bottom);
	newMatrix.data[3][2] = -(far + near) / (far - near);
	newMatrix.data[3][3] = 1.0f;

	return newMatrix;

}

Matrix4 Matrix4::perspective(float left, float right, float bottom, float top, float near, float far)
{

	Matrix4 newMatrix;

	float twoNear = 2.0f * near;

	newMatrix.data[0][0] = twoNear / (right - left);
	newMatrix.data[0][1] = 0.0f;
	newMatrix.data[0][2] = 0.0f;
	newMatrix.data[0][3] = 0.0f;

	newMatrix.data[1][0] = 0.0f;
	newMatrix.data[1][1] = twoNear / (top - bottom);
	newMatrix.data[1][2] = 0.0f;
	newMatrix.data[1][3] = 0.0f;

	newMatrix.data[2][0] = (right + left) / (right - left);
	newMatrix.data[2][1] = (top + bottom) / (top - bottom);
	newMatrix.data[2][2] = -(far + near) / (far - near);
	newMatrix.data[2][3] = 1.0f;

	newMatrix.data[3][0] = 0.0f;
	newMatrix.data[3][1] = 0.0f;
	newMatrix.data[3][2] = -twoNear * far / (far - near);
	newMatrix.data[3][3] = 0.0f;

	return newMatrix;

}

Matrix4 Matrix4::empty() {

	Matrix4 m;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m.data[i][j] = 0;
		}
	}

	return m;

}

Matrix4 Matrix4::identity() {

	Matrix4 m;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m.data[i][j] = (i == j);
		}
	}

	return m;

}

Matrix4 Matrix4::fill(float value) {

	Matrix4 m;

	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			m.data[i][j] = value;
		}
	}

	return m;

}

void Matrix4::print() {

	std::cout << data[0][0] << " " << data[0][1] << " " << data[0][2] << " " << data[0][3] << std::endl;
	std::cout << data[1][0] << " " << data[1][1] << " " << data[1][2] << " " << data[1][3] << std::endl;
	std::cout << data[2][0] << " " << data[2][1] << " " << data[2][2] << " " << data[2][3] << std::endl;
	std::cout << data[3][0] << " " << data[3][1] << " " << data[3][2] << " " << data[3][3] << std::endl;

}
