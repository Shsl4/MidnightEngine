#include <Rendering/Mesh.h>
#include <Rendering/ShaderLoader.h>

#include <assimp/mesh.h>

Mesh::Mesh(Array<Vertex> const &vertices, Array<UInt16> const& indexArray, String name) :
    vertexCount(vertices.getSize()), indexCount(indexArray.getSize()), meshName(std::move(name)) {
    
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

Mesh* Mesh::from(const aiMesh* libMesh)
{
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
            texCoords = Vector2(libMesh->mTextureCoords[0][i]);
        }
        
        vertices += Vertex(position, normal, texCoords, LinearColors::white);
        
    }
    
    for(size_t i = 0; i < libMesh->mNumFaces; ++i){
        
        for(size_t j = 0; j < libMesh->mFaces[i].mNumIndices; ++j){
            indices += libMesh->mFaces[i].mIndices[j];
        }
        
    }
    
    indices.reverse();
    
    Mesh *mesh = Allocator<Mesh>().construct(vertices, indices, objectName);
    
    return mesh;
    
}