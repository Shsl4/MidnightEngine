#include <Rendering/Mesh.h>
#include <Rendering/ResourceLoader.h>

Mesh::Mesh(Array<Vertex> const &vertices, Array<UInt16> const& indexArray, String name, String path) :
    vertexCount(vertices.getSize()), indexCount(indexArray.getSize()), meshName(std::move(name)), filePath(std::move(path)) {
    
    data = vertexAllocator.alloc(vertexCount);
    indices = indexAllocator.alloc(indexCount);
    
    Memory::move(vertices.begin(), vertices.end(), data);
    Memory::move(indexArray.begin(), indexArray.end(), indices);

    this->vertexBuffer = createVertexBuffer(bgfx::makeRef(data,  static_cast<Int32>(vertexCount) * sizeof(Vertex)), getVertexLayout());
    this->indexBuffer = createIndexBuffer(bgfx::makeRef(indices, static_cast<Int32>(indexCount) * sizeof(UInt16)));
    this->programHandle = ResourceLoader::loadProgram("Advanced");
    
}

Mesh::~Mesh() {
    
    destroy(vertexBuffer);
    destroy(indexBuffer);
    destroy(programHandle);
    vertexAllocator.release(data);
    indexAllocator.release(indices);
    
}