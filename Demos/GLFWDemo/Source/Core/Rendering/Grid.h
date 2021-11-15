//
//  Grid.hpp
//  GLFWDemo
//
//  Created by Pierre on 15/11/2021.
//

#pragma once

#include <Rendering/RenderObject.h>
#include <Rendering/ShaderProgram.h>
#include <Rendering/Vertex.h>
#include <Memory/Allocator.h>

class Grid : public RenderObject {
    
public:
    
    Grid(const char* shaderName = "Default", const int size = 8);
    void render(float deltaTime) override;
    virtual ~Grid();
    
private:
    
    Vertex* createGrid(int size);
    
    Vertex* lines = nullptr;

    unsigned int vertexBufferId = 0;
    unsigned int vertexArrayId = 0;
    int gridSize = 0;
    
    ShaderProgram* program;
    Allocator allocator;
    Logger logger = Logger("Temp");
    
    Vertex ls[18] =
    {
        
        Vertex(-1.0f, -0.2f, 0.0f, LinearColors::white),
        Vertex(1.0f, -0.2f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f,-0.4f, 0.0f, LinearColors::white),
        Vertex(1.0f, -0.4f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f, -0.6f, 0.0f, LinearColors::white),
        Vertex(1.0f, -0.6f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f, -0.8f, 0.0f, LinearColors::white),
        Vertex(1.0f, -0.8f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f, 0.0f, 0.0f, LinearColors::white),
        Vertex(1.0f, 0.0f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f, 0.2f, 0.0f, LinearColors::white),
        Vertex(1.0f, 0.2f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f, 0.4f, 0.0f, LinearColors::white),
        Vertex(1.0f, 0.4f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f, 0.6f, 0.0f, LinearColors::white),
        Vertex(1.0f, 0.6f, 0.0f, LinearColors::white),
        
        Vertex(-1.0f, 0.8f, 0.0f, LinearColors::white),
        Vertex(1.0f, 0.8f, 0.0f, LinearColors::white)

    };
    
};
