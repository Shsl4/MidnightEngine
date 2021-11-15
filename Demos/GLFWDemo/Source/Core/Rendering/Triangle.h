//
//  Triangle.hpp
//  SDLDemo
//
//  Created by Pierre on 15/11/2021.
//
#pragma once

#include <Rendering/RenderObject.h>
#include <Rendering/ShaderProgram.h>
#include <Rendering/Vertex.h>
#include <Memory/Allocator.h>

class Triangle : public RenderObject{
    
public:
    
    Triangle(const char* shaderName = "Default");
    
    virtual void render(float deltaTime) override;
    
    virtual ~Triangle();
    
private:
    
    Vertex triangle[3] =
    {

        Vertex(0.0f, 0.69282f - 0.23094f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vertex(-0.4f, -0.23094f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f),
        Vertex(0.4f, -0.23094f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),

    };

    unsigned int vertexBufferId = 0;
    unsigned int vertexArrayId = 0;
    ShaderProgram* program;
    Allocator allocator;
    
};
