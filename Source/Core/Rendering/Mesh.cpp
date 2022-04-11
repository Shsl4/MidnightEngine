#include <Rendering/Mesh.h>
#include <Memory/String.h>
#include <Memory/Memory.h>
#include <sstream>
#include <fstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(Array<Vertex> const &vertices, Array<UInt16> const& indexArray, MeshType type, String const &name) :
vertexCount(vertices.getSize()), indexCount(indexArray.getSize()), meshType(type), meshName(name) {
    
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

Mesh *MeshLoader::loadMesh(String const &file) {
    
    // Preallocate the arrays with large sizes to avoid many reallocations later.
    String path = "Resources/Models/";
    path.append(file);
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toCString(), aiProcessPreset_TargetRealtime_Fast);
    aiMesh* libMesh = scene->mMeshes[0];
    
    String objectName = libMesh->mName.C_Str();
    
    auto vertices = Array<Vertex>(libMesh->mNumVertices);
    auto indices = Array<UInt16>(10000);

    for(size_t i = 0; i < libMesh->mNumVertices; ++i){
        
        Vector3 position = libMesh->mVertices[i];
        Vector3 normal = Vector3::zero;
        Vector2 texCoords = Vector2::zero;

        if (libMesh->HasNormals()) {
            normal = libMesh->mNormals[i];
        }
        
        if (libMesh->HasTextureCoords(0)) {
            texCoords = libMesh->mTextureCoords[0][i];
        }
        
        vertices += Vertex(position, normal, texCoords, LinearColors::white);
        
    }
    
    for(size_t i = 0; i < libMesh->mNumFaces; ++i){
        
        for(size_t j = 0; j < libMesh->mFaces[i].mNumIndices; ++j){
            indices += libMesh->mFaces[i].mIndices[j];
        }
        
    }
    
    indices.reverse();
    
    Mesh *mesh = instance->allocator.construct(vertices, indices, MeshType::OBJ, objectName);
    
    instance->loadedMeshes += mesh;
    
    return mesh;
    
}
