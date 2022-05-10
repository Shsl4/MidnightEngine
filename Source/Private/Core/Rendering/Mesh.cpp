#include <Rendering/Mesh.h>
#include <bgfx/bgfx.h>
#include <Rendering/ShaderManager.h>

#define HandleWrapper(name, type)                                \
                                                                 \
    struct name {                                                \
                                                                 \
        explicit name(type const& handle) : handle(handle) {     \
                                                                 \
        }                                                        \
                                                                 \
        ~name(){                                                 \
            bgfx::destroy(handle);                               \
        }                                                        \
                                                                 \
        type handle;                                             \
                                                                 \
    };

HandleWrapper(ProgramHandle, bgfx::ProgramHandle)
HandleWrapper(VertexHandle, bgfx::VertexBufferHandle)
HandleWrapper(IndexHandle, bgfx::IndexBufferHandle)

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

    const auto vertexMemory = bgfx::makeRef(data,  static_cast<Int32>(vertexCount) * sizeof(Vertex));
    const auto indexMemory = bgfx::makeRef(indices, static_cast<Int32>(indexCount) * sizeof(UInt16));
    
    this->vertexBuffer = Allocator<VertexHandle>().construct(createVertexBuffer(vertexMemory, getVertexLayout()));
    this->indexBuffer = Allocator<IndexHandle>().construct(createIndexBuffer(indexMemory));
    this->programHandle = Allocator<ProgramHandle>().construct(ShaderManager::loadProgram("Material"));
    
}

void Mesh::use() const
{
    setIndexBuffer(indexBuffer->handle);
    setVertexBuffer(0, vertexBuffer->handle);
}

void Mesh::submit() const
{
    bgfx::submit(0, programHandle->handle);
}

Mesh::~Mesh() {

    vertexAllocator.release(data);
    indexAllocator.release(indices);
    vertexAllocator.autoDestroy(vertexBuffer);
    vertexAllocator.autoDestroy(indexBuffer);
    vertexAllocator.autoDestroy(programHandle);

}
