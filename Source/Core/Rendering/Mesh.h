#pragma once

#include <Math/Vector2.h>
#include <Rendering/Vertex.h>
#include <Rendering/ShaderLoader.h>
#include <Memory/Array.h>
#include <bgfx/bgfx.h>
#include <vector>

#include <assimp/scene.h>
#include <assimp/mesh.h>

enum class MeshType {

    OBJ,
    FBX

};

/*!
 * A structure representing a mesh. 
 */
struct Mesh {

    Mesh(Array<Vertex> const &vertices, Array<UInt16> const& indexArray, MeshType type, String const &name);
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

    Vertex *data = nullptr;
    UInt16 *indices = nullptr;

    MeshType meshType;
    String meshName;

    inline static Allocator<Vertex> vertexAllocator = Allocator<Vertex>();
    inline static Allocator<UInt16> indexAllocator = Allocator<UInt16>();

};

/*!
 *  A utility class allowing to load OBJ models.
 *
 *  \todo Make the class nonstatic, make it load all models in the resource folder
 *  on startup and make them available through public functions.
 */
class MeshLoader {

public:

    MeshLoader() {
        Logger::check(!MeshLoader::instance, "This shouldn't be built twice.");
        MeshLoader::instance = this;
    }

    static Mesh *loadMesh(String const &file);

private:

    static inline MeshLoader *instance = nullptr;

    AutoReleaseArray<Mesh *> loadedMeshes;
    Allocator<Mesh> allocator = Allocator<Mesh>();

};
