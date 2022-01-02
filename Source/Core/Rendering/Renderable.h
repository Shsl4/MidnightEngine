#pragma once

#include <bgfx/bgfx.h>

class Renderable{
    
public:
    
    Renderable() = default;
    virtual ~Renderable() = default;

    virtual void render() = 0;
    
};
