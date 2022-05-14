#include <Rendering/ShaderManager.h>

#include <Rendering/ResourceLoader.h>
#include <Logging/Logger.h>
#include <fmt/format.h>

bgfx::ShaderHandle ShaderManager::loadShader(String const& name) {
    
    // Format the file path.
    const String filePath = fmt::format("{}{}.bin", getShaderResourcePath(), name);
    
    // Load the file content
    const Array<UInt8> fileData = ResourceLoader::loadFile(filePath);

    expect(!fileData.isEmpty(), "Tried to load an invalid shader file: {}", filePath);
    
    // Convert the data to bgfx::Memory.
    const bgfx::Memory* mem = bgfx::copy(fileData.begin(), static_cast<UInt32>(fileData.getSize()));

    // Create our shader and name it.
    const bgfx::ShaderHandle handle = createShader(mem);

    setName(handle, name.toCString());

    return handle;

}

bgfx::ProgramHandle ShaderManager::loadProgram(String const& name) {

    // Format our fragment and vertex shader file names.
    const String vertexShaderName = fmt::format("vs_{}", name);
    const String fragmentShaderName = fmt::format("fs_{}", name);

    // Load the fragment and vertex shaders.
    const bgfx::ShaderHandle vertexShader = loadShader(vertexShaderName);
    const bgfx::ShaderHandle fragmentShader = loadShader(fragmentShaderName);

    // Create the shader program from the loaded shaders.
    return createProgram(vertexShader, fragmentShader, true);

}

String ShaderManager::getShaderResourcePath() {
    
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
