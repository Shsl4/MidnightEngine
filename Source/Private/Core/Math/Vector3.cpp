#include <Math/Vector3.h>

bool Vector3::operator==(Vector3 const &other) const{
    
    return (x == other.x) && (y == other.y) && (z == other.z);
    
}

bool Vector3::operator< (Vector3 const &other) const{
    
    return norm() < other.norm();
    
}

Vector3 Vector3::operator-(Vector3 const &other) const {
    return { this->x - other.x, this->y - other.y, this->z - other.z };
}


Vector3 Vector3::operator+(Vector3 const &other) const {
    return { this->x + other.x, this->y + other.y, this->z + other.z };
}

Vector3 Vector3::operator/(Vector3 const& other) const
{
    return { this->x / other.x, this->y / other.y, this->z / other.z };
}

Vector3 Vector3::operator*(Vector3 const& other) const
{
    return { this->x * other.x, this->y * other.y, this->z * other.z };
}

Vector3 Vector3::operator*(const float scale) const {
    return { x * scale, y * scale, z * scale };
}


Vector3 Vector3::operator/(const float scale) const {
    return Vector3(x / scale, y / scale, z / scale);
}


void Vector3::operator*=(const float scale) {
    this->x *= scale;
    this->y *= scale;
    this->z *= scale;
}

void Vector3::operator/=(const float scale) {
    this->x /= scale;
    this->y /= scale;
    this->z /= scale;
}

void Vector3::operator-=(Vector3 const &other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
}


void Vector3::operator+=(Vector3 const &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
}

const Vector3 Vector3::zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::down = Vector3(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::forward = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::backward = Vector3(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::right = Vector3(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::left = Vector3(0.0f, 0.0f, -1.0f);
