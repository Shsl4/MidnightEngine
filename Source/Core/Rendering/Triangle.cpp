//
//  Triangle.cpp
//  MidnightEngine
//
//  Created by Pierre on 24/11/2021.
//

#include "Triangle.h"
#include <bx/timer.h>
#include <Rendering/Vertex.h>
#include <Rendering/ShaderLoader.h>

Vertex triangle[3] =
{

    Vertex(0.0f, 0.69282f - 0.23094f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f),
    Vertex(-0.4f, -0.23094f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f),
    Vertex(0.4f, -0.23094f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),

};

Triangle::Triangle(const char* shaderName){

    vertexBufferId = bgfx::createVertexBuffer(bgfx::makeRef(triangle, sizeof(triangle)), getVertexLayout());

    programId = ShaderLoader::loadProgram("Default");
    
}

bgfx::VertexLayout Triangle::getVertexLayout(){
    
    static bgfx::VertexLayout ms_layout;
    
    ms_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
        .end();
    
    return ms_layout;
    
}

void Triangle::render(RenderData data){
    
    float ratio;
    ratio = data.width / (float)data.height;
    Matrix4 rotZ = Matrix4::identity();
    rotZ.rotateZ(data.time);
    Matrix4 orthographic = Matrix4::orthographic(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f) * rotZ;
    Matrix4 id = Matrix4::identity();

    bgfx::setViewTransform(0, id.data, orthographic.data);

    bgfx::setTransform(id.data);
    bgfx::setVertexBuffer(0, vertexBufferId);
    bgfx::submit(0, programId);

    for (size_t j = 0; j < 3; ++j) {
        triangle[j].color.setRed(abs((float)sin(j % 3 + data.time + 3.14f / 4.0f)));
        triangle[j].color.setGreen(abs((float)cos(j % 3 + data.time)));
        triangle[j].color.setBlue(abs((float)sin(j % 3 + data.time - 3.14f)));
    }

    bgfx::destroy(vertexBufferId);
    vertexBufferId = bgfx::createVertexBuffer(bgfx::makeRef(triangle, sizeof(triangle)), getVertexLayout());
    
}
