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

Vector3 Matrix4::operator*(const Vector3 other)
{

	const float x = this->data[0][0] * other.x + this->data[1][0] * other.y + this->data[2][0] * other.z;
	const float y = this->data[0][1] * other.x + this->data[1][1] * other.y + this->data[2][1] * other.z;
	const float z = this->data[0][2] * other.x + this->data[1][2] * other.y + this->data[2][2] * other.z;

	return Vector3(x, y, z);

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

void Matrix4::operator=(const float newData[4][4])
{
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			this->data[i][j] += newData[i][j];
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

Vector4 Matrix4::operator[](const int i)
{
	return Vector4(data[i][0], data[i][1], data[i][2], data[i][3]);
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

void Matrix4::rotate(float radians, Vector3 axis) {

	Matrix4 rotation = Matrix4::rotation(radians, axis);
	Matrix4 temp = Matrix4::empty();

	auto r = rotation.data;

	Vector4 a = (*this)[0] * r[0][0] + (*this)[1] * r[0][1] + (*this)[2] * r[0][2];
	Vector4 b = (*this)[0] * r[1][0] + (*this)[1] * r[1][1] + (*this)[2] * r[1][2];
	Vector4 c = (*this)[0] * r[2][0] + (*this)[1] * r[2][1] + (*this)[2] * r[2][2];
	Vector4 d = (*this)[3];

	float da[4][4] = {
	
		{ a.x, a.y, a.z, a.w },
		{ b.x, b.y, b.z, a.w },
		{ c.x, c.y, c.z, c.w },
		{ d.x, d.y, d.z, d.w },

	};

	*this = Matrix4::copyingData(da);

}


Matrix4 Matrix4::rotation(float radians, Vector3 axis)
{

	axis.normalize();
	const float _c = cos(radians);
	const float _s = sin(radians);
	const float _o = 1.0f - _c;

	Matrix4 rotate = Matrix4::identity();

	rotate.data[0][0] = _c + (axis.x * axis.x) * _o;
	rotate.data[1][0] = (axis.x * axis.y * _o) - (axis.z * _s);
	rotate.data[2][0] = axis.x * axis.z * _o + axis.y * _s;

	rotate.data[0][1] = (axis.x * axis.y * _o) + (axis.z * _s);
	rotate.data[1][1] = _c + (axis.y * axis.y) * _o;
	rotate.data[2][1] = axis.y * axis.z * _o - axis.x * _s;

	rotate.data[0][2] = (axis.x * axis.z * _o) - (axis.y * _s);
	rotate.data[1][2] = axis.y * axis.z * _o + axis.x * _s;
	rotate.data[2][2] = _c + (axis.z * axis.z) * _o;

	return rotate;

}

// https://en.wikipedia.org/wiki/Orthographic_projection
Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float near, float far)
{

	Matrix4 newMatrix = Matrix4::empty();

	newMatrix.data[0][0] = 2.0f / (right - left);
	newMatrix.data[1][1] = 2.0f / (top - bottom);
	newMatrix.data[2][2] = -2.0f / (far - near);
	newMatrix.data[3][0] = -(right + left) / (right - left);
	newMatrix.data[3][1] = -(top + bottom) / (top - bottom);
	newMatrix.data[3][2] = -(far + near) / (far - near);
	newMatrix.data[3][3] = 1.0f;

	return newMatrix;

}

Matrix4 Matrix4::perspective(float fov, float aspectRatio, float near, float far)
{

	Matrix4 newMatrix = Matrix4::empty();

	const float tanfov = tan(Math::toRadians(fov) / 2.0f);
    const float fmn = (far - near);
    
	newMatrix.data[0][0] = 1.0f / (aspectRatio * tanfov);
	newMatrix.data[1][1] = 1.0f / tanfov;
	newMatrix.data[2][2] = (far + near) / fmn;
	newMatrix.data[2][3] = 1.0f;
	newMatrix.data[3][2] = -(near * far) / fmn;

	return newMatrix;

}

Matrix4 Matrix4::lookAt(Vector3 eye, Vector3 at, Vector3 up)
{

	Vector3 zAxis = Vector3::normalize(at - eye);
	Vector3 xAxis = Vector3::normalize(Vector3::cross(up, zAxis));
	Vector3 yAxis = Vector3::cross(zAxis, xAxis);

	zAxis.negate();

	Matrix4 newMatrix = Matrix4::empty();

	newMatrix.data[0][0] = xAxis.x;
	newMatrix.data[1][0] = xAxis.y;
	newMatrix.data[2][0] = xAxis.z;
	newMatrix.data[3][0] = -Vector3::dot(xAxis, eye);

	newMatrix.data[0][1] = yAxis.x;
	newMatrix.data[1][1] = yAxis.y;
	newMatrix.data[2][1] = yAxis.z;
	newMatrix.data[3][1] = -Vector3::dot(yAxis, eye);

	newMatrix.data[0][2] = -zAxis.x;
	newMatrix.data[1][2] = -zAxis.y;
	newMatrix.data[2][2] = -zAxis.z;
	newMatrix.data[3][2] = Vector3::dot(zAxis, eye);
	newMatrix.data[3][3] = 1.0f;

	return newMatrix;

}

Matrix4 Matrix4::empty() {

	Matrix4 m;
	memset(m.data, 0, 16 * sizeof(float));
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

Matrix4 Matrix4::copyingData(const float newData[4][4])
{

	Matrix4 newMatrix;
	memcpy(newMatrix.data, newData, 16 * sizeof(float));
	return newMatrix;

}

Matrix4 Matrix4::copyingData(const float* newData)
{

	Matrix4 newMatrix;
	memcpy(newMatrix.data, newData, 16 * sizeof(float));
	return newMatrix;

}

void Matrix4::print() {

	std::cout << data[0][0] << " " << data[0][1] << " " << data[0][2] << " " << data[0][3] << std::endl;
	std::cout << data[1][0] << " " << data[1][1] << " " << data[1][2] << " " << data[1][3] << std::endl;
	std::cout << data[2][0] << " " << data[2][1] << " " << data[2][2] << " " << data[2][3] << std::endl;
	std::cout << data[3][0] << " " << data[3][1] << " " << data[3][2] << " " << data[3][3] << std::endl;

}
