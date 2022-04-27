#include "ResourceLoader.h"

#include <filesystem>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

ResourceLoader::ResourceLoader()
{
    for (const auto & entry : std::filesystem::directory_iterator("Resources/Models"))
    {
        std::filesystem::path file = entry.path().filename();
        String extension = file.extension().string();

        if (extension == ".obj")
        {
            loadMesh(file.string());
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

String ResourceLoader::getShaderResourcePath() {
    
    // Switch on the renderer type.
    switch (bgfx::getRendererType()) {
            
        case bgfx::RendererType::Direct3D9:
            return "Resources/Shaders/DX9/";
            
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:
            return "Resources/Shaders/DX11/";
            
        case bgfx::RendererType::Agc:
        case bgfx::RendererType::Gnm:
            return "Resources/Shaders/PSSL/";
            
        case bgfx::RendererType::Metal:
            return "Resources/Shaders/Metal/";
            
        case bgfx::RendererType::Nvn:
            return "Resources/Shaders/Nvn/";
            
        case bgfx::RendererType::OpenGL:
            return "Resources/Shaders/GLSL/";
            
        case bgfx::RendererType::OpenGLES:
            return "Resources/Shaders/ESSL/";
            
        case bgfx::RendererType::Vulkan:
        case bgfx::RendererType::WebGPU:
            return "Resources/Shaders/SPIRV/";
        
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Count:
            return "";
        
    }

    return "";
    
}

bgfx::ShaderHandle ResourceLoader::loadShader(String const& name) {
    
    // Format the file path.
    const String filePath = fmt::format("{}{}.bin", getShaderResourcePath(), name);
    
    // Load the file content
    const Array<UInt8> fileData = loadFile(filePath);
    
    // Convert the data to bgfx::Memory.
    const bgfx::Memory* mem = bgfx::copy(fileData.begin(), fileData.getSize());

    // Create our shader and name it.
    const bgfx::ShaderHandle handle = bgfx::createShader(mem);
    //setName(handle, name.toCString());

    return handle;

}

bgfx::ProgramHandle ResourceLoader::loadProgram(String const& name) {

    // Format our fragment and vertex shader file names.
    const String vertexShaderName = fmt::format("vs_{}", name);
    const String fragmentShaderName = fmt::format("fs_{}", name);

    // Load the fragment and vertex shaders.
    const bgfx::ShaderHandle vertexShader = loadShader(vertexShaderName);
    const bgfx::ShaderHandle fragmentShader = loadShader(fragmentShaderName);

    // Create the shader program from the loaded shaders.
    return bgfx::createProgram(vertexShader, fragmentShader, true);

}

Mesh* ResourceLoader::getMesh(String const& name) const
{

    for (auto const& mesh : loadedMeshes)
    {
        if (mesh->meshName == name)
        {
            return mesh;
        }
        
    }

    logger.error("Tried to get mesh named {} which does not exist.", name);

    return nullptr;
    
}

bool ResourceLoader::loadMesh(String const& file)
{

    String fileName = file.substring(0, file.lastIndexOf('.').getValueElse(file.getSize()));
    String path = "Resources/Models/";
    path.append(file);
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.toCString(), aiProcessPreset_TargetRealtime_Fast |
        aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);

    if (scene == nullptr)
    {
        logger.error("Failed to load model. The path {} is invalid.", path);
        return false;
    }

    auto totalVertices = Array<Vertex>(10000);
    auto totalIndices = Array<UInt16>(10000);
    
    for (size_t i = 0; i < scene->mNumMeshes; ++i)
    {
        const auto libMesh = scene->mMeshes[i];
        auto vertices = Array<Vertex>(libMesh->mNumVertices);
        auto indices = Array<UInt16>(10000);

        for(size_t j = 0; j < libMesh->mNumVertices; ++j){
        
            Vector3 position = libMesh->mVertices[j];
            Vector3 normal = Vector3::zero;
            Vector2 texCoords = Vector2::zero;

            if (libMesh->HasNormals()) {
                normal = libMesh->mNormals[j];
            }
        
            if (libMesh->HasTextureCoords(0)) {
                texCoords = Vector2(libMesh->mTextureCoords[0][j]);
            }
        
            vertices += Vertex(position, normal, texCoords, LinearColors::white);
        
        }

        const Int32 numFaces = static_cast<Int32>(libMesh->mNumFaces) - 1;
    
        for(Int32 j = numFaces; j >= 0 ; --j){

            const Int32 numIndices = static_cast<Int32>(libMesh->mFaces[j].mNumIndices) - 1;
            
            for(Int32 k = numIndices; k >= 0 ; --k){
                indices += libMesh->mFaces[j].mIndices[k] + totalVertices.getSize();
            }
            
        }

        totalVertices += vertices;
        totalIndices += indices;

    }
    
    loadedMeshes += Allocator<Mesh>().construct(totalVertices, totalIndices, fileName, path);

    logger.success("Successfully loaded mesh {}. Combined {} components.", fileName, scene->mNumMeshes);

    return true;
    
}

