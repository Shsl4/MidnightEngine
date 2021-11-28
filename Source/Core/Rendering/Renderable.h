#pragma once

#include <bgfx/bgfx.h>
#include <Rendering/RenderData.h>

class Renderable{
    
public:
    
    Renderable() { }
    
    virtual void render() = 0;
    
    virtual bgfx::VertexLayout getVertexLayout() = 0;
  
    virtual ~Renderable() {
        
        bgfx::destroy(vertexBufferId);
        bgfx::destroy(programId);

    }
    
protected:
    
    bgfx::ProgramHandle programId;
    bgfx::VertexBufferHandle vertexBufferId;

    
};
