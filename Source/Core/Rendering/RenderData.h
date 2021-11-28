#pragma once

struct RenderData{
    
    RenderData(float t, float dt, float w, float h){
        
        this->time = t;
        this->deltaTime = dt;
        this->width = w;
        this->height = h;
        
    }
    
    float time;
    float deltaTime;
    unsigned int width;
    unsigned int height;
    
};
