//
//  Grid.cpp
//  GLFWDemo
//
//  Created by Pierre on 15/11/2021.
//

#include <Rendering/Grid.h>
#include <Core/GLFWEngine.h>
#include <Math/Matrix4.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Grid::Grid(const char* shaderName, const int size){
    
    lines = createGrid(size);
    
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * gridSize, lines, GL_STATIC_DRAW);
    glGenVertexArrays(1, &this->vertexArrayId);
    glBindVertexArray(this->vertexArrayId);
    
    program = new ShaderProgram(shaderName);

}

Grid::~Grid(){
    
    glDeleteVertexArrays(1, &this->vertexArrayId);
    glDeleteBuffers(1, &this->vertexBufferId);

}

Vertex* Grid::createGrid(int size){
    
    Vertex* ptr = allocator.allocate<Vertex>(size * 4);
    gridSize = size * 4;
    
    float increment = 2.0f / (size * 2.0f);
    
    for (int i = 0; i < gridSize; i += 2) {
        
        if(i <= size * 2){
            
            float x = -1.0f + i * increment;
            ptr[i] = Vertex(x, 1.0f, 0.0f, LinearColors::black);
            ptr[i + 1] = Vertex(x, -1.0f, 0.0f, LinearColors::black);
            
        }
        else{
            
            float mul = (i - size * 2);
            float y = -1.0f + mul * increment;
            ptr[i] = Vertex(-1.0f, y, 0.0f, LinearColors::black);
            ptr[i + 1] = Vertex(1.0f, y, 0.0f, LinearColors::black);
            
        }
        
    }
    
    return ptr;
    
}


void Grid::render(float deltaTime){
    
    float ratio;
    int width, height;
    
    glfwGetFramebufferSize(((GLFWEngine*)GLFWEngine::getInstance())->getMainWindow(), &width, &height);
    
    ratio = width / (float)height;
    glViewport(0, 0, width, height);
    
    const float time = GLFWEngine::getInstance()->getTime();
    
    Matrix4 matrix = Matrix4::orthographic(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);
    program->bind();
    
    int mvp_location = glGetUniformLocation(program->getProgram(), "viewMatrix");
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)matrix.data);
    
    glBindVertexArray(this->vertexArrayId);
    glDrawArrays(GL_LINES, 0, gridSize);

    program->unbind();
    glBindVertexArray(0);

}


