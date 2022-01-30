#include <Math/Vector2.h>

bool Vector2::operator==(Vector2 const &other) const {

    return (x == other.x) && (y == other.y);

}

bool Vector2::operator<(Vector2 const &other) const{
    
    return norm() < other.norm();
    
}


Vector2 Vector2::operator+(Vector2 const &other) {

    return Vector2(this->x + other.x, this->y + other.y);

}

Vector2 Vector2::operator-(Vector2 const &other) {

    return Vector2(this->x - other.x, this->y - other.y);

}

const Vector2 Vector2::zero = Vector2(0.0f, 0.0f);
