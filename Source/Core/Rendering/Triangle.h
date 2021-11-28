#pragma once

#include <Rendering/RenderObject.h>

class Triangle : public RenderObject{
    
public:
    
    Triangle(const char* shaderName);
    
    virtual bgfx::VertexLayout getVertexLayout() override;
    
    virtual void render(RenderData data) override;
    
};
