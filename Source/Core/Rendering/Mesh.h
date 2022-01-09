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
    
    Mesh(Array<Vertex> const& vertices, size_t numIndices, MeshType type, std::string const& name);

    virtual ~Mesh();
    
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
    uint16_t* indices = nullptr;
    
    MeshType meshType;
    std::string meshName;

    inline static Allocator allocator = Allocator();

};

class MeshLoader{

public:
    
    MeshLoader() {
        Logger::check(!MeshLoader::instance, "This shouldn't be built twice.");
        MeshLoader::instance = this;
    }

    static Mesh* loadOBJ(std::string const& file);

private:

    static inline MeshLoader* instance = nullptr;
    
    AutoReleaseArray<Mesh*> loadedMeshes;
    Allocator allocator = Allocator();

};
