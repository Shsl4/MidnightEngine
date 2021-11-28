#pragma once

struct RenderData{
    
    RenderData(float t, float dt, unsigned int w, unsigned int h){
        
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
