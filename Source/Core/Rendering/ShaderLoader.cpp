#include <Rendering/ShaderLoader.h>
#include <fstream>

Array<UInt8> ShaderLoader::loadFile(String const& path){
    
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

String ShaderLoader::getShaderResourcePath() {
    
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

bgfx::ShaderHandle ShaderLoader::loadShader(String const& name) {
    
    // Format the file path.
    const String filePath = fmt::format("{}{}.bin", getShaderResourcePath(), name);
    
    // Load the file content
    const Array<UInt8> fileData = loadFile(filePath);
    
    // Convert the data to bgfx::Memory.
    const bgfx::Memory* mem = bgfx::copy(fileData.begin(), fileData.getSize());

    // Create our shader and name it.
    const bgfx::ShaderHandle handle = bgfx::createShader(mem);
    setName(handle, name.toCString());

    return handle;

}

bgfx::ProgramHandle ShaderLoader::loadProgram(String const& name) {

    // Format our fragment and vertex shader file names.
    const String vertexShaderName = fmt::format("vs_{}", name);
    const String fragmentShaderName = fmt::format("fs_{}", name);

    // Load the fragment and vertex shaders.
    const bgfx::ShaderHandle vertexShader = loadShader(vertexShaderName.toCString());
    const bgfx::ShaderHandle fragmentShader = loadShader(fragmentShaderName.toCString());

    // Create the shader program from the loaded shaders.
    return bgfx::createProgram(vertexShader, fragmentShader, true);

}

