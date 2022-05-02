#include <Rendering/Mesh.h>
#include <bgfx/bgfx.h>
#include <Rendering/ResourceLoader.h>

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

Mesh::Mesh(Array<Vertex> const &vertices, Array<UInt16> const& indexArray, String name, String path) :
    vertexCount(vertices.getSize()), indexCount(indexArray.getSize()), meshName(std::move(name)), filePath(std::move(path)) {

    data = vertexAllocator.alloc(vertexCount);
    indices = indexAllocator.alloc(indexCount);

    Memory::move(vertices.begin(), vertices.end(), data);
    Memory::move(indexArray.begin(), indexArray.end(), indices);

    this->vertexBuffer = createVertexBuffer(bgfx::makeRef(data,  static_cast<Int32>(vertexCount) * sizeof(Vertex)), getVertexLayout()).idx;
    this->indexBuffer = createIndexBuffer(bgfx::makeRef(indices, static_cast<Int32>(indexCount) * sizeof(UInt16))).idx;
    this->programHandle = ResourceLoader::loadProgram("Advanced");
    
}

void Mesh::use() const
{
    setIndexBuffer(bgfx::IndexBufferHandle(indexBuffer));
    setVertexBuffer(0, bgfx::VertexBufferHandle(vertexBuffer));    
}

void Mesh::submit() const
{
    bgfx::submit(0, bgfx::ProgramHandle(programHandle));
}

Mesh::~Mesh() {

    destroy(bgfx::VertexBufferHandle(vertexBuffer));
    destroy(bgfx::IndexBufferHandle(indexBuffer));
    destroy(bgfx::ProgramHandle(programHandle));
    vertexAllocator.release(data);
    indexAllocator.release(indices);

}
