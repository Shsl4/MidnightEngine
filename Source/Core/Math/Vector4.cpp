
#include <Math/Vector4.h>

Vector4 Vector4::operator*(const float scale) {

    return Vector4(this->x * scale, this->y * scale, this->z * scale, this->w * scale);

}

Vector4 Vector4::operator+(const Vector4& other) {

    return Vector4(this->x + other.x, this->y + other.y, this->z + other.z, this->w + other.w);

}

Vector4 Vector4::operator-(const Vector4& other)
{
    return Vector4(this->x - other.x, this->y - other.y, this->z - other.z, this->w - other.w );
    
}
Vector4 Vector4::operator/(const float scale){

    return Vector4(this->x / scale, this->y / scale, this->z / scale, this->w / scale);
}

void Vector4::operator-=(Vector4& other){
    this->x-=other.x;
    this->y-=other.y;
    this->z-=other.z;
    this->w-=other.w;
}
void Vector4::operator+=(Vector4& other)
{
    this->x+=other.x;
    this->y+=other.y;
    this->z+=other.z;
    this->w+=other.w;
}
void Vector4::operator*=(const float scale)
{
    this->x*=scale;
    this->y*=scale;
    this->z*=scale;
    this->w*=scale;
}

void Vector4::operator/=(const float scale)
{
    this->x/=scale;
    this->y/=scale;
    this->z/=scale;
    this->w/=scale;
}