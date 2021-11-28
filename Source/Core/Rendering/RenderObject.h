//
//  RenderObject.h
//  MidnightEngine
//
//  Created by Pierre on 24/11/2021.
//

#pragma once

#include <bgfx/bgfx.h>
#include <Rendering/RenderData.h>

class RenderObject{
    
public:
    
    RenderObject() { }
    
    virtual void render(RenderData data) = 0;
    
    virtual bgfx::VertexLayout getVertexLayout() = 0;
  
    virtual ~RenderObject() {
        
        bgfx::destroy(vertexBufferId);
        bgfx::destroy(programId);

    }
    
protected:
    
    bgfx::ProgramHandle programId;
    bgfx::VertexBufferHandle vertexBufferId;

    
};
