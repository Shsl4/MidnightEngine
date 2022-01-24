#include <Rendering/ShaderLoader.h>
#include <fstream>

Array<UInt8> ShaderLoader::loadFile(std::string const& path){
    
    // Create an array with a default size of 512.
    Array<uint8_t> array = Array<uint8_t>(512);
    
    // Create our file stream in binary mode.
    std::ifstream fStream(path, std::ios::binary);

    // If the file does not exist or is inaccessible, return.
    if (!fStream.is_open()) { return array; }
    
    // Store the file contents in the array.
    while(fStream.good()){
        array += fStream.get();
    }
    
    // Return the array.
    return array;
    
}

std::string ShaderLoader::getShaderResourcePath() {
    
    // Switch on the renderer type.
    switch (bgfx::getRendererType()) {
            
        case bgfx::RendererType::Noop:
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
             return "Resources/Shaders/SPIRV/";
            
        case bgfx::RendererType::WebGPU:
            return "Resources/Shaders/SPIRV/";
            
        default:
            return "";
    }
    
}

bgfx::ShaderHandle ShaderLoader::loadShader(std::string const& name) {
    
    // Format the file path.
    std::string filePath = fmt::format("{}{}.bin", getShaderResourcePath(), name);
    
    // Load the file content
    const Array<UInt8> fileData = loadFile(filePath);
    
    // Convert the data to bgfx::Memory.
    const bgfx::Memory* mem = bgfx::copy(fileData.begin(), fileData.getSize());

    // Create our shader and name it.
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name.c_str());

    return handle;

}

bgfx::ProgramHandle ShaderLoader::loadProgram(std::string const& name) {

    // Format our fragment and vertex shader file names.
    std::string vertexShaderName = fmt::format("vs_{}", name);
    std::string fragmentShaderName = fmt::format("fs_{}", name);

    // Load the fragment and vertex shaders.
    bgfx::ShaderHandle vertexShader = loadShader(vertexShaderName.c_str());
    bgfx::ShaderHandle fragmentShader = loadShader(fragmentShaderName.c_str());

    // Create the shader program from the loaded shaders.
    return bgfx::createProgram(vertexShader, fragmentShader, true);

}

