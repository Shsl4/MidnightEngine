//
//  Triangle.cpp
//  SDLDemo
//
//  Created by Pierre on 15/11/2021.
//

#include <GLFWEngine.h>
#include <Math/Matrix4.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Triangle::Triangle(const char* shaderName){
    
    glGenBuffers(1, &vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    glGenVertexArrays(1, &this->vertexArrayId);
    glBindVertexArray(this->vertexArrayId);
    
    program = allocator.instantiate<ShaderProgram>("Default");
    
}

Triangle::~Triangle(){
    
    glDeleteVertexArrays(1, &this->vertexArrayId);
    glDeleteBuffers(1, &this->vertexBufferId);
    allocator.deallocate(program);

}

void Triangle::render(float deltaTime){
    
    float ratio;
    int width, height;
    
    glfwGetFramebufferSize(((GLFWEngine*)GLFWEngine::getInstance())->getMainWindow(), &width, &height);
    ratio = width / (float)height;
    glViewport(0, 0, width, height);
    
    const float time = GLFWEngine::getInstance()->getTime();

    for (size_t j = 0; j < 3; ++j) {
        triangle[j].color.setRed(abs((float)sin(j % 3 + time + 3.14f / 4.0f)));
        triangle[j].color.setGreen(abs((float)cos(j % 3 + time)));
        triangle[j].color.setBlue(abs((float)sin(j % 3 + time - 3.14f)));
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
    program->bind();

    Matrix4 id = Matrix4::identity();
    id.rotateZ(time);
    id.rotateY(time);
    id.rotateX(time);
    Matrix4 matrix = Matrix4::orthographic(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f) * id;
    
    int mvp_location = glGetUniformLocation(program->getProgram(), "viewMatrix");
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)matrix.data);
    
    glBindVertexArray(this->vertexArrayId);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    program->unbind();
    glBindVertexArray(0);

}
