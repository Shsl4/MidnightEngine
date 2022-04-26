#pragma once

#include <bgfx/bgfx.h>

#include <Memory/Array.h>
#include <Memory/String.h>
#include <Rendering/Vertex.h>

/*!
 * A structure representing a mesh.
 */
struct Mesh
{
    static Mesh* from(const struct aiMesh* libMesh);
    virtual ~Mesh();

    Mesh(Array<Vertex> const& vertices, Array<UInt16> const& indexArray, String name);

    static bgfx::VertexLayout getVertexLayout()
    {
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
    UInt16* indices = nullptr;

    String meshName;

    inline static Allocator<Vertex> vertexAllocator = Allocator<Vertex>();
    inline static Allocator<UInt16> indexAllocator = Allocator<UInt16>();
};
