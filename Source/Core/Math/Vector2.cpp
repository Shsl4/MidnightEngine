#include <Math/Vector2.h>

Vector2 Vector2::operator+(Vector2 const& other){
    
    return Vector2(this->x + other.x, this->y + other.y);
    
}

Vector2 Vector2::operator-(Vector2 const& other){
    
    return Vector2(this->x - other.x, this->y - other.y);
    
}
