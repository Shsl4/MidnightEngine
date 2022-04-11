#include <Rendering/Mesh.h>
#include <Memory/String.h>
#include <Memory/Memory.h>
#include <sstream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(Array<Vertex> const &vertices, Array<UInt16> const& indexArray, MeshType type, String const &name) :
        vertexCount(vertices.getSize()), indexCount(indexArray.getCapacity()), meshType(type), meshName(name) {

    data = vertexAllocator.alloc(vertexCount);
    indices = indexAllocator.alloc(indexCount);


    Memory::move(vertices.begin(), vertices.end(), data);
    Memory::move(indexArray.begin(), indexArray.end(), indices);

    vertexBuffer = createVertexBuffer(bgfx::makeRef(data, (Int32)vertexCount * sizeof(Vertex)), Mesh::getVertexLayout());
    indexBuffer = bgfx::createIndexBuffer(bgfx::makeRef(indices, (Int32)indexCount * sizeof(UInt16)));
    programHandle = ShaderLoader::loadProgram("Advanced");

}

Mesh::~Mesh() {

    bgfx::destroy(vertexBuffer);
    bgfx::destroy(indexBuffer);
    bgfx::destroy(programHandle);
    vertexAllocator.release(data);
    indexAllocator.release(indices);

}

Mesh *MeshLoader::loadOBJ(String const &file) {

    // Preallocate the arrays with large sizes to avoid many reallocations later.
    String path = "Resources/Models/";
    path.append(file);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toCString(), aiProcessPreset_TargetRealtime_Fast);//aiProcessPreset_TargetRealtime_Fast has the configs you'll need

    aiMesh* mesh = scene->mMeshes[0];

    auto vertexPositions = Array<Vector3>(10000);
    auto vertexNormals = Array<Vector3>(10000);
    auto vertexTexCoords = Array<Vector2>(5000);

    unsigned int elementCount = mesh->mNumFaces * 3;
    std::string objectName = "";

    if (mesh->HasPositions()) {
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            vertexPositions += Vector3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        }
    }


    if (mesh->HasTextureCoords(0)) {
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            vertexTexCoords += Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
    }


    if (mesh->HasNormals()) {
        for (int i = 0; i < mesh->mNumVertices; ++i) {
            vertexNormals += Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }
    }

    auto indices = Array<UInt16>(10000);
    size_t count = 0;

    if (mesh->HasFaces()) {
        for (int i = 0; i < mesh->mNumFaces; ++i) {
            indices[i * 3] = mesh->mFaces[i].mIndices[0];
            indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
            indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
            count++;
        }
    }

    auto vertices = Array<Vertex>(count);
    size_t capacity = vertices.getCapacity();

    for (size_t i = 0; i < capacity; ++i) {

        auto a = vertexPositions[indices[i * 3]];

        auto b = vertexNormals[indices[i * 3 + 1]];

        auto vtx = Vertex(a,
                b,
                Vector2::zero,
                LinearColors::white);

        vertices += vtx;

    }
    vertices.resize(count);
    Mesh *mesh1 = instance->allocator.construct(vertices, indices, MeshType::OBJ, objectName);

    instance->loadedMeshes += mesh1;

    return mesh1;

}
