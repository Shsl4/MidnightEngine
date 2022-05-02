#pragma once

#include <Memory/Array.h>
#include <Memory/String.h>
#include <Rendering/Vertex.h>

/*!
 * A structure representing a mesh.
 */
struct ENGINE_API Mesh
{
    virtual ~Mesh();

    Mesh(Array<Vertex> const& vertices, Array<UInt16> const& indexArray, String name, String path);

    void use() const;

    void submit() const;
    
    UInt16 programHandle;
    UInt16 vertexBuffer;
    UInt16 indexBuffer;

    size_t vertexCount = 0;
    size_t indexCount = 0;

    Vertex* data = nullptr;
    UInt16* indices = nullptr;

    String meshName;
    String filePath;

    inline static Allocator<Vertex> vertexAllocator = Allocator<Vertex>();
    inline static Allocator<UInt16> indexAllocator = Allocator<UInt16>();

};
