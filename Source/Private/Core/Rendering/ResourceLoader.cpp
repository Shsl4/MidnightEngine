#include <Rendering/ResourceLoader.h>

#include <filesystem>
#include <assimp/scene.h>

#include <assimp/postprocess.h>

#include <Rendering/ShaderManager.h>
#include <assimp/Importer.hpp>
#include <Console/Console.h>

#include <stb_image.h>

#include <Rendering/Model.h>
#include <Rendering/Uniforms.h>

#include <Rendering/ShaderPrograms.h>

ResourceLoader::~ResourceLoader() {
    
    Uniforms::destroyUniforms();
    ShaderPrograms::destroyPrograms();

}

void ResourceLoader::init() {

    Uniforms::makeUniforms();
    ShaderPrograms::makePrograms();
    
    std::filesystem::directory_iterator iterator;

    try {
        iterator = std::filesystem::directory_iterator("./Resources/Models");
    }
    catch (std::exception const&) {
        Console::getLogger()->fatal("Could not find the Resources folder.");
        return;
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

    try {
        iterator = std::filesystem::directory_iterator("./Resources/Textures");
    }
    catch (std::exception const&) {
        Console::getLogger()->fatal("Could not find the Resources folder.");
        return;
    }

    // For each file in our model resource directory
    for (const auto & entry : iterator)
    {
        // Get the file name and its extension
        std::filesystem::path file = entry.path().filename();

        // Load it
        loadTexture(file.string());
        
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

    // For every loaded mesh
    for (auto const& model : loadedModels)
    {
        // If the mesh name matches
        if (model->modelName == name)
        {
            // Return the mesh
            return model.weak();
        }
        
    }

    // Print an error message
    Console::getLogger()->error("Tried to get mesh named {} which does not exist.", name);

    return nullptr;
    
}

WeakPointer<Texture> ResourceLoader::getTexture(String const& name) const {

    
    // For every loaded mesh
    for (auto const& texture : loadedTextures)
    {
        // If the mesh name matches
        if (texture->textureName == name)
        {
            // Return the mesh
            return texture.weak();
        }
        
    }

    // Print an error message
    Console::getLogger()->error("Tried to get texture named {} which does not exist.", name);

    return nullptr;
    
}

void ResourceLoader::loadTexture(String const& file)
{

    // Get the file name by removing the file extension
    String fileName = file.substring(0, file.lastIndexOf('.').getValueElse(file.getSize()));

    // Create our resource path
    String path = "./Resources/Textures/";
    path.append(file);

    Int32 width = 0;
    Int32 height = 0;
    Int32 channelCount = 0;
    
    auto* data = stbi_load(path.toCString(), &width, &height, &channelCount, 4);

    if (data == nullptr)
    {
        Console::getLogger()->error("Failed to load texture. Reason: {}. File: {}", stbi_failure_reason(), file);
        return;
    }

    // Create the mesh and store it in our array. Meshes are automatically released when the engine stops.
    loadedTextures += SharedPointer<Texture>::make(fileName, width, height, data);
    
    stbi_image_free(data);

    // Log a success message.
    Console::getLogger()->success("Successfully loaded texture {}.", fileName);
    
}

void ResourceLoader::loadModel(String const& file)
{

    // Get the file name by removing the file extension
    String fileName = file.substring(0, file.lastIndexOf('.').getValueElse(file.getSize()));

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
        
            vertices += Vertex(position, normal, texCoords, LinearColors::white);
        
        }

        const Int32 numFaces = static_cast<Int32>(libMesh->mNumFaces) - 1;
    
        for(Int32 j = numFaces; j >= 0 ; --j){

            const Int32 numIndices = static_cast<Int32>(libMesh->mFaces[j].mNumIndices) - 1;
            
            for(Int32 k = numIndices; k >= 0 ; --k){
                indices += libMesh->mFaces[j].mIndices[k];
            }
            
        }

        // Create the mesh and store it in our array.
        meshes += SharedPointer<Mesh>::make(vertices, indices, SharedPointer<Texture>(), fileName, path);
        
    }

    loadedModels += SharedPointer<Model>::make(meshes, fileName, Material());
    
    // Log a success message.
    Console::getLogger()->success("Successfully loaded model {}. Combined {} components.", fileName, scene->mNumMeshes);
    
}
