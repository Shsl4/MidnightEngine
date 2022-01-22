#include <Rendering/ShaderLoader.h>

const bgfx::Memory *ShaderLoader::loadMem(bx::FileReaderI *_reader, const char *_filePath) {
    if (bx::open(_reader, _filePath)) {
        auto size = (UInt32) bx::getSize(_reader);
        const bgfx::Memory *mem = bgfx::alloc(size + 1);
        bx::read(_reader, mem->data, size, bx::ErrorAssert{});
        bx::close(_reader);
        mem->data[mem->size - 1] = '\0';
        return mem;
    }

    logger.error("Failed to load file {}!", _filePath);

    return nullptr;

}

bgfx::ShaderHandle ShaderLoader::loadShader(const char *_name) {

    char filePath[512];

    const char *shaderPath = "";

    switch (bgfx::getRendererType()) {
        case bgfx::RendererType::Noop:
        case bgfx::RendererType::Direct3D9:
            shaderPath = "Resources/Shaders/DX9/";
            break;
        case bgfx::RendererType::Direct3D11:
        case bgfx::RendererType::Direct3D12:
            shaderPath = "Resources/Shaders/DX11/";
            break;
        case bgfx::RendererType::Agc:
        case bgfx::RendererType::Gnm:
            shaderPath = "Resources/Shaders/PSSL/";
            break;
        case bgfx::RendererType::Metal:
            shaderPath = "Resources/Shaders/Metal/";
            break;
        case bgfx::RendererType::Nvn:
            shaderPath = "Resources/Shaders/Nvn/";
            break;
        case bgfx::RendererType::OpenGL:
            shaderPath = "Resources/Shaders/GLSL/";
            break;
        case bgfx::RendererType::OpenGLES:
            shaderPath = "Resources/Shaders/ESSL/";
            break;
        case bgfx::RendererType::Vulkan:
            shaderPath = "Resources/Shaders/SPIRV/";
            break;
        case bgfx::RendererType::WebGPU:
            shaderPath = "Resources/Shaders/SPIRV/";
            break;
        default:
            BX_ASSERT(false, "You should not be here!");
            break;
    }

    bx::strCopy(filePath, BX_COUNTOF(filePath), shaderPath);
    bx::strCat(filePath, BX_COUNTOF(filePath), _name);
    bx::strCat(filePath, BX_COUNTOF(filePath), ".bin");

    std::string path = fmt::format("{}/{}", std::filesystem::current_path().string(), filePath);

    bgfx::ShaderHandle handle = bgfx::createShader(loadMem(fileReader, filePath));
    bgfx::setName(handle, _name);

    return handle;

}

bgfx::ProgramHandle ShaderLoader::loadProgram(const char *programeName) {

    std::string vertexShaderName = fmt::format("vs_{}", programeName);
    std::string fragmentShaderName = fmt::format("fs_{}", programeName);

    bgfx::ShaderHandle vertexShader = loadShader(vertexShaderName.c_str());
    bgfx::ShaderHandle fragmentShader = loadShader(fragmentShaderName.c_str());

    return bgfx::createProgram(vertexShader, fragmentShader, true);

}

