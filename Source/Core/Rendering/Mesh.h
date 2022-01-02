#pragma once

#include <Math/Vector2.h>
#include <Rendering/Vertex.h>
#include <Rendering/ShaderLoader.h>
#include <Memory/Array.h>
#include <bgfx/bgfx.h>
#include <vector>

enum class MeshType{
    
    OBJ,
    FBX
    
};

struct Mesh{
    
    Mesh(Array<Vertex> const& vertices, unsigned short numIndices, MeshType type, std::string const& name){
        
        vertexCount = vertices.getSize();
        indexCount = numIndices;
        meshType = type;
        meshName = name;
        
        data = allocator.allocate<Vertex>(vertexCount);
        indices = allocator.allocate<unsigned short>(indexCount);

        for(int i = 0; i < indexCount; ++i){
            
            indices[i] = indexCount - 1 - i;
            
        }
        
        memcpy(data, vertices.getRawData(), vertexCount * sizeof(Vertex));

        vertexBuffer = createVertexBuffer(bgfx::makeRef(data, vertexCount * sizeof(Vertex)), Mesh::getVertexLayout());
        
        indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(indices, indexCount * sizeof(unsigned short)));
        
        programHandle = ShaderLoader::loadProgram("Advanced");
        
    }
    
    static bgfx::VertexLayout getVertexLayout() {
        
        bgfx::VertexLayout layout;

        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
            .end();
        
        return layout;
        
    }
    
    bgfx::ProgramHandle programHandle;
    bgfx::VertexBufferHandle vertexBuffer;
    bgfx::IndexBufferHandle indexBuffer;
    
    size_t vertexCount = 0;
    size_t indexCount = 0;
    
    Vertex* data = nullptr;
    unsigned short* indices = nullptr;
    
    MeshType meshType;
    std::string meshName;

private:
    
    constexpr static const Allocator allocator = Allocator();
    
};

class MeshLoader{

public:
    
    static Mesh* loadOBJ(std::string const& file);

private:
    
    inline static Array<Mesh*> loadedMeshes;
    
};
