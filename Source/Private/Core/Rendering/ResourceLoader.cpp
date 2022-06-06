#include <Rendering/ResourceLoader.h>

#include <filesystem>
#include <assimp/scene.h>

#include <assimp/postprocess.h>

#include <Rendering/ShaderManager.h>
#include <assimp/Importer.hpp>
#include <Console/Console.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <Rendering/Model.h>
#include <Rendering/Uniforms.h>

#include <Rendering/ShaderPrograms.h>

#include "bimg/decode.h"
#include "bx/file.h"
#include "Utilities/ArrayUtils.h"

ResourceLoader::~ResourceLoader() {
    
    Uniforms::destroyUniforms();
    ShaderPrograms::destroyPrograms();

}

void ResourceLoader::init() {

    Uniforms::makeUniforms();
    ShaderPrograms::makePrograms();
    
    std::filesystem::recursive_directory_iterator iterator;
    
    try {
        iterator = std::filesystem::recursive_directory_iterator("./Resources/Textures");
    }
    catch (std::exception const& e) {
        raisef("Error loading textures: {}", e.what());
    }

    // For each file in our model resource directory
    for (const auto & entry : iterator)
    {
        if (entry.is_directory()) { continue; }

        // Load it
        loadTexture(entry.path().string());
        
    }
        
    try {
        iterator = std::filesystem::recursive_directory_iterator("./Resources/Models");
    }
    catch (std::exception const& e) {
        raisef("Error loading models: {}", e.what());
    }

    // For each file in our model resource directory
    for (const auto & entry : iterator)
    {
        // Get the file name and its extension
        std::filesystem::path file = entry.path().filename();
        String extension = file.extension().string().c_str();

        // If the file is a 3D model file
        if (extension == ".obj" || extension == ".fbx")
        {
            // Load it
            loadModel(file.string().c_str());
        }
        
    }
    
}

Array<UInt8> ResourceLoader::loadFile(String const& path){
    
    // Create an array with a default size of 512.
    auto array = Array<UInt8>(512);
    
    // Create our file stream in binary mode.
    std::ifstream fStream(path.toCString(), std::ios::binary);

    // If the file does not exist or is inaccessible, return.
    if (!fStream.is_open()) { return array; }
    
    // Store the file contents in the array.
    while(fStream.good()){
        array += fStream.get();
    }
    
    // Return the array.
    return array;
    
}

WeakPointer<Model> ResourceLoader::getModel(String const& name) const
{

    // For every loaded model
    for (auto const& model : loadedModels)
    {
        // If the model name matches
        if (model->modelName == name)
        {
            // Return the model
            return model.weak();
        }
        
    }

    // Print an error message
    Console::getLogger()->error("Tried to get mesh named {} which does not exist.", name);

    return nullptr;
    
}

WeakPointer<Texture> ResourceLoader::getTexture(String const& name) const {

    
    // For every loaded texture
    for (auto const& texture : loadedTextures) {
        
        // If the texture name matches
        if (texture->textureName == name)
        {
            // Return the texture
            return texture.weak();
        }
        
    }

    // Print an error message
    Console::getLogger()->error("Tried to get texture named {} which does not exist.", name);

    return missingTexture;
    
}

void ResourceLoader::loadTexture(String const& file)
{

    // Get the file name by removing the file extension
    String fileName = file.substring(0, ArrayUtils::lastIndexOf(file, '.').getValueElse(file.getSize()));
    ArrayUtils::replaceAll(fileName, '\\', '/');
    fileName = fileName.substring(ArrayUtils::lastIndexOf(fileName, '/').getValueElse(0) + 1, fileName.getSize());
        
    Int32 width = 0;
    Int32 height = 0;
    Int32 channelCount = 0;

    auto* data = stbi_load(file.toCString(), &width, &height, &channelCount, 4);

    if (data == nullptr)
    {
        Console::getLogger()->error("Failed to load texture. Reason: {}. File: {}", stbi_failure_reason(), file);
        return;
    }

    // Create the mesh and store it in our array. Meshes are automatically released when the engine stops.
    const auto texture = SharedPointer<Texture>::make(fileName, width, height, data);

    if(fileName == "Missing"){
        missingTexture = texture;
    }
    
    loadedTextures += texture;
    
    stbi_image_free(data);

    // Log a success message.
    Console::getLogger()->success("Successfully loaded texture {}.", fileName);
    
}


Vector3 barycentric(Vector3 p, Vector3 a, Vector3 b, Vector3 c) {
                
    Vector3 v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = Vector3::dot(v0, v0);
    float d01 = Vector3::dot(v0, v1);
    float d11 = Vector3::dot(v1, v1);
    float d20 = Vector3::dot(v2, v0);
    float d21 = Vector3::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return { u, v, w };
    
}


void ResourceLoader::loadModel(String const& file)
{

    // Get the file name by removing the file extension
    String fileName = file.substring(0, ArrayUtils::lastIndexOf(file, '.').getValueElse(file.getSize()));

    // Create our resource path
    String path = "Resources/Models/";
    path.append(file);

    // Create a new importer.
    Assimp::Importer importer;
    
    // Load the 3D model.
    const aiScene* scene = importer.ReadFile(path.toCString(), aiProcessPreset_TargetRealtime_Fast |
                                                               aiProcess_Triangulate);
    // If the model could not be loaded, print an error message.
    if (scene == nullptr)
    {
        Console::getLogger()->error("Failed to load model. The path {} is invalid.", path);
        return;
    }

    Array<SharedPointer<Mesh>> meshes;
    auto textures = Array<SharedPointer<Texture>>(scene->mNumMeshes);
    
    for (size_t i = 0; i < scene->mNumMeshes; ++i) {

        const auto libMesh = scene->mMeshes[i];
        auto vertices = Array<Vertex>(libMesh->mNumVertices);
        auto indices = Array<UInt16>(10000);
        
        for(size_t j = 0; j < libMesh->mNumVertices; ++j) {
            
            const auto pos = libMesh->mVertices[j];
            
            Vector3 position = Vector3(pos.x, pos.y, pos.z);
            Vector3 normal = Vector3::zero;
            Vector2 texCoords = Vector2::zero;

            if (libMesh->HasNormals()) {

                const auto norm = libMesh->mNormals[j];
                normal = Vector3(norm.x, norm.y, norm.z);
                
            }
        
            if (libMesh->HasTextureCoords(0)) {
                
                const auto coords = libMesh->mTextureCoords[0][j];
                texCoords = Vector2(coords.x, coords.y);
                
            }
        
            vertices += Vertex(position, normal, texCoords, LinearColors::white, Vector3::zero);
            
        }

        const Int32 numFaces = static_cast<Int32>(libMesh->mNumFaces) - 1;
    
        for(Int32 j = numFaces; j >= 0 ; --j){

            const Int32 numIndices = static_cast<Int32>(libMesh->mFaces[j].mNumIndices) - 1;
            
            for(Int32 k = numIndices; k >= 0 ; --k){
                indices += libMesh->mFaces[j].mIndices[k];
            }
            
        }
        
        for (size_t f = 0; f < libMesh->mNumFaces; ++f) {
            
            const auto face = libMesh->mFaces[f];
            const auto numIndices = face.mNumIndices;
    
            assert(numIndices == 3);

            // \todo Fix barycentric coordinates
            auto& v1 = vertices[face.mIndices[0]];
            auto& v2 = vertices[face.mIndices[f % 2 == 0 ? 1 : 2]];
            auto& v3 = vertices[face.mIndices[f % 2 == 0 ? 2 : 1]];

            v1.barycentric = barycentric(v1.position, v1.position, v2.position, v3.position);
            v2.barycentric = barycentric(v2.position, v1.position, v2.position, v3.position);
            v3.barycentric = barycentric(v3.position, v1.position, v2.position, v3.position);
            
        }
        
        textures += nullptr;
        
        meshes += SharedPointer<Mesh>::make(vertices, indices, fileName, path);

    }    
    
    loadedModels += SharedPointer<Model>::make(meshes, textures, fileName);
    
    // Log a success message.
    Console::getLogger()->success("Successfully loaded model {}. Combined {} components.", fileName, scene->mNumMeshes);
    
}