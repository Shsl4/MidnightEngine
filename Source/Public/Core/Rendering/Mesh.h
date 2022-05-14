#pragma once

#include <Memory/Array.h>
#include <Memory/String.h>
#include <Memory/UniquePointer.h>
#include <Rendering/Vertex.h>

#include <Rendering/Material.h>
#include <bgfx/bgfx.h>

#include "Texture.h"
#include "Memory/SharedPointer.h"

/*!
 * A structure representing a mesh.
 */
struct ENGINE_API Mesh {
    
    Mesh(Array<Vertex> const& vertices, Array<UInt16> const& indexArray, SharedPointer<Texture> meshTexture,
        String name, String path);

    ~Mesh();
    
    void render(UInt16 viewId, Material const& material, bgfx::ProgramHandle program) const;
    
    struct VertexHandle* vertexBuffer;
    struct IndexHandle* indexBuffer;

    size_t vertexCount = 0;
    size_t indexCount = 0;

    Vertex* data = nullptr;
    UInt16* indices = nullptr;

    String meshName;
    String filePath;

    SharedPointer<Texture> texture;
    
    inline static Allocator<Vertex> vertexAllocator = Allocator<Vertex>();
    inline static Allocator<UInt16> indexAllocator = Allocator<UInt16>();

};
