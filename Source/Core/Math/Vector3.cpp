#include <Math/Vector3.h>

Vector3 Vector3::operator-(Vector3& other)
{
	return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
}


Vector3 Vector3::operator+(Vector3& other)
{
	return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
}

Vector3 Vector3::operator*(const float scale)
{
	return Vector3(x * scale, y * scale, z * scale);
}


Vector3 Vector3::operator/(const float scale)
{
	return Vector3(x / scale, y / scale, z / scale);
}


void Vector3::operator*=(const float scale)
{
	this->x *= scale;
	this->y *= scale;
	this->z *= scale;
}

void Vector3::operator/=(const float scale)
{
	this->x /= scale;
	this->y /= scale;
	this->z /= scale;
}

void Vector3::operator-=(Vector3& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z -= other.z;
}


void Vector3::operator+=(Vector3& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::Forward = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Backward = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Right = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Left = Vector3(0.0f, 0.0f, -1.0f);
