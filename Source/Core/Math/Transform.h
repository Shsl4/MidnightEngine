#pragma once

#include <Math/Vector3.h>

struct Transform{
    
public:
    
    Transform(){
        
        this->position = Vector3();
        this->rotation = Vector3();
        this->scale = Vector3(1.0f, 1.0f, 1.0f);

    }
    
    Transform(Vector3 position){
        
        this->position = position;
        this->rotation = Vector3();
        this->scale = Vector3(1.0f, 1.0f, 1.0f);

    }
    
    Transform(Vector3 position, Vector3 rotation){
        
        this->position = position;
        this->rotation = rotation;
        this->scale = Vector3(1.0f, 1.0f, 1.0f);

    }
    
    Transform(Vector3 position, Vector3 rotation, Vector3 scale){
        
        this->position = position;
        this->rotation = rotation;
        this->scale = scale;

    }
    
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    
};
