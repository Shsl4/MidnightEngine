#include <Engine.h>
#include <Rendering/ShaderLoader.h>
#include <Rendering/Vertex.h>
#include <Rendering/Triangle.h>
#include <Rendering/RenderData.h>
#include <Input/InputManager.h>
#include <UI/PerformanceWindow.h>

#include <bgfx/imgui/imgui.h>
#include <SDL2/SDL_syswm.h>
#include <Scene/Scene.h>

MEngine::MEngine(){
    
    MEngine::instance = this;
    this->logger = std::make_unique<Logger>("MidnightEngine");
    this->inputManager = std::make_unique<InputManager>();
    this->perfWindow = std::make_unique<PerformanceWindow>();
    
}

MEngine::~MEngine() {

    getLogger()->debug("Destroying MidnightEngine...");
    cleanup();

}

int MEngine::init(int argc, const char** argv){
    
    getLogger()->info("Initializing MidnightEngine...");

    if (!bgfx::init()) {

        getLogger()->fatal("Failed to initialize BGFX!");
        bgfx::shutdown();
        return -1;

    }
    
    running = true;

    const bgfx::Caps* caps = bgfx::getCaps();

    for (uint8_t ii = 0; ii < caps->numGPUs; ++ii)
    {
        const bgfx::Caps::GPU& gpu = caps->gpu[ii];
        if (caps->vendorId == gpu.vendorId
            && caps->deviceId == gpu.deviceId)
        {

            vendorId = caps->gpu[ii].vendorId;
            deviceId = caps->gpu[ii].deviceId;
            break;
        }

    }

    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewClear(0
        , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
        , 0x303030ff
        , 1.0f
        , 0
    );
    
    inputManager->bindEvent(this, KeyBind(SDLK_ESCAPE), EInputEvent::IE_PRESSED, &MEngine::onEscape);

    inputManager->bindEvent(this, KeyBind(SDL_BUTTON_LEFT), EInputEvent::IE_PRESSED, &MEngine::onLeftMousePressed);
    
    inputManager->bindEvent(this, KeyBind(SDL_BUTTON_LEFT), EInputEvent::IE_RELEASED, &MEngine::onLeftMouseReleased);
    
    Triangle* tri = new Triangle("Default");
    
    objects.append(tri);
    
    imguiCreate();

    getLogger()->info("Initialized MidnightEngine! Now rendering using {} on {} (Device ID: {})", getNiceRendererName(), getNiceGPUName(), deviceId);
    
    timeOffset = bx::getHPCounter();
    
    logger->info("Desc: {}", getDescription());
    
    while (isRunning()) {
        
        render();
        
    }
        
    return 0;
    
}

bool left = false, right = false, middle = false;

void MEngine::loop(){
    
    inputManager->update();
    
}

void MEngine::onEscape(){
    
    running = false;
    
}

void MEngine::onLeftMousePressed(){
    
    logger->debug("Pressed!");
    
}


void MEngine::onLeftMouseReleased(){
    
    logger->debug("Released!");
    
}


void MEngine::render(){
    
    uint32_t m_width = 1280;
    uint32_t m_height = 720;
    
    imguiBeginFrame(inputManager->getMouseX(),
                    inputManager->getMouseY(),
                    (inputManager->leftMousePressed() ? IMGUI_MBUT_LEFT : 0) |
                    (inputManager->rightMousePressed() ? IMGUI_MBUT_RIGHT : 0) |
                    (inputManager->middleMousePressed() ? IMGUI_MBUT_MIDDLE : 0),
                    0,
                    uint16_t(m_width),
                    uint16_t(m_height)
        );

    perfWindow->render(nullptr);
    
    imguiEndFrame();
    
    const int64_t now = bx::getHPCounter();
    const double freq = double(bx::getHPFrequency());
    float time = (float)((now - timeOffset)/freq);
    
    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

    bgfx::touch(0);
    
    RenderData data = RenderData(time, deltaTime, m_width, m_height);
    
    for (int i = 0; i < objects.getCount(); ++i) {
        
        objects[i]->render(data);
        
    }
    
    bgfx::frame();
        
}

void MEngine::restart(){
    
    getLogger()->info("Restarting engine...");
    
}

void MEngine::stop(){
    
    running = false;
    
}

void MEngine::cleanup(){
    
    imguiDestroy();
    bgfx::shutdown();

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
