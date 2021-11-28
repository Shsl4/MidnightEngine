#include <Engine.h>

#include <Rendering/ShaderLoader.h>
#include <Rendering/Vertex.h>
#include <Rendering/RenderData.h>
#include <Input/InputManager.h>

#include <bgfx/imgui/imgui.h>
#include <SDL2/SDL_syswm.h>
#include <Scene/Scene.h>

MEngine::MEngine(SDL_Window* mainWindow){
    
    MEngine::instance = this;

    this->logger = std::make_unique<Logger>("MidnightEngine");
    this->inputManager = std::make_unique<InputManager>();

    this->running = false;
    this->mainWindow = mainWindow;

    SDL_GetWindowSize(mainWindow, &windowWidth, &windowHeight);
    
}

MEngine::~MEngine() {

    cleanup();

}

int MEngine::init(int argc, const char** argv){
    
    getLogger()->info("Initializing MidnightEngine...");

    if (!bgfx::init()) {

        getLogger()->fatal("Failed to initialize BGFX!");
        bgfx::shutdown();
        return -1;

    }
    
    bgfx::setViewClear(0
        , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
        , 0x303030ff
        , 1.0f
        , 0
    );

    timeOffset = bx::getHPCounter();

    logger->info("Desc: {}", getDescription());

    inputManager->bindEvent(this, KeyBind(SDLK_ESCAPE), EInputEvent::Pressed, &MEngine::stop);

    this->activeScene = std::make_unique<Scene>();

    getLogger()->info("Initialized MidnightEngine! Now rendering using {} on {}", getNiceRendererName(), getNiceGPUName());

    running = true;
    
    while (isRunning()) {
        
        render();
        
    }
        
    return 0;
    
}

void MEngine::update(){
    
    const int64_t now = bx::getHPCounter();
    const double freq = double(bx::getHPFrequency());
    time = (float)((now - timeOffset) / freq);
    deltaTime = time - lastTime;
    lastTime = time;

    inputManager->update();
    activeScene->updateScene(deltaTime);

}

void MEngine::render(){
    
    bgfx::setViewRect(0, 0, 0, uint16_t(windowWidth), uint16_t(windowHeight));

    bgfx::touch(0);

    activeScene->renderComponents();

    bgfx::frame();
        
}

void MEngine::stop(){
    
    running = false;
    
}

void MEngine::cleanup(){

    getLogger()->info("Destroying MidnightEngine...");

    MEngine::instance = nullptr;

    this->logger = nullptr;
    this->inputManager = nullptr;
    this->activeScene = nullptr;

}

std::string MEngine::getNiceRendererName() {

    switch (renderer) {
    case bgfx::RendererType::Direct3D9:
        return "DirectX 9";
    case bgfx::RendererType::Direct3D11:
        return "DirectX 11";
    case bgfx::RendererType::Direct3D12:
        return "DirectX 12";
    case bgfx::RendererType::Vulkan:
        return "Vulkan";
    case bgfx::RendererType::OpenGL:
        return "OpenGL";
    case bgfx::RendererType::OpenGLES:
        return "OpenGLES";
    case bgfx::RendererType::Metal:
        return "Metal";
    case bgfx::RendererType::WebGPU:
        return "WebGPU";
    default:
        return "Unknown";
    }

}

std::string MEngine::getNiceGPUName() {

    const bgfx::Caps* caps = bgfx::getCaps();

    int current = 0;

    for (uint8_t ii = 0; ii < caps->numGPUs; ++ii)
    {
        const bgfx::Caps::GPU& gpu = caps->gpu[ii];
        if (caps->vendorId == gpu.vendorId
            && caps->deviceId == gpu.deviceId)
        {
            current = ii;
            break;
        }
    }

    switch (caps->gpu[current].vendorId) {
    case BGFX_PCI_ID_AMD:
        return "AMD GPU";
    case BGFX_PCI_ID_INTEL:
        return "Intel GPU";
    case BGFX_PCI_ID_NVIDIA:
        return "NVidia GPU";
    default:
        return "Default GPU";
    }

}
