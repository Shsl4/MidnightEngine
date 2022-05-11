#include <Engine.h>
#include <thread>

#include <Math/Matrix4.h>
#include <Math/MathUtils.h>

#include <Scene/CameraComponent.h>

#include <Input/InputManager.h>

#include <UI/PerformanceWindow.h>

#include <Scene/Scene.h>

#include <Memory/UniquePointer.h>

#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/imgui/imgui.h>

#include <SDL2/SDL_keycode.h>
#include <thread>
#include <iostream>

Logger Logger::assertLogger = Logger("Assert");

Engine::Engine() {
    
    // If an engine instance has already been created, abort the program. (Should never happen)
    if(Engine::instance) { abort(); }
    
    Engine::instance = this;

    // Initialize our variables.
    this->inputManager = UniquePointer<InputManager>::make();
    this->perfWindow = UniquePointer<PerformanceWindow>::make();
    this->resourceLoader = UniquePointer<ResourceLoader>::make();
    this->console = UniquePointer<Console>::make(this);

}

int Engine::init(int argc, const char **argv, PlatformData const& data) {
    
    Console::getLogger()->info("Initializing MidnightEngine...");
    
    // Initialize BGFX.
    if (!bgfx::init()) {

        // If it fails, print an error message and return.
        Console::getLogger()->fatal("Failed to initialize BGFX!");
        bgfx::shutdown();
        return -1;

    }
    
    this->platformData = data;
    this->startTime = bx::getHPCounter();

    bgfx::reset(platformData.renderWidth, platformData.renderHeight, BGFX_RESET_HIDPI);
    
    // Sets the world "void" to be a black color.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0, 1.0f, 0);

    // Initializes our UI system.
    imguiCreate();

    resourceLoader->init();
        
    // Add an input event. If the escape key is pressed, the engine will exit. This will be removed later.
    inputManager->bindEvent(this, KeyBind(SDLK_ESCAPE), EInputEvent::Pressed, &Engine::stop);
    
    // As everything went fine, print an info message.
    Console::getLogger()->info("Initialized MidnightEngine! Now rendering using {} on {}", getNiceRendererName(), getNiceGpuName());
    
    running = true;

    console->init();

    onStart();

    return 0;

}

void Engine::update() {

    if (!isRunning()) { return; }
    
    const Int64 now = bx::getHPCounter();
    const Int64 freq = bx::getHPFrequency();
    
    // Calculate the current frame time, last frame time and delta time.
    time = static_cast<float>(now - startTime) / static_cast<float>(freq);
    deltaTime = time - lastTime;
    lastTime = time;
    
    // Update our input system.
    inputManager->update();
    
    // Only update the scene if it is loaded
    if (activeScene.valid() && activeScene->getState() == Scene::State::Loaded) {
        
        // Update the current scene.
        activeScene->update(deltaTime);

    }

    onUpdate();

}

void Engine::render() {

    for (auto const& task : renderThreadTasks) {
        task();
    }

    renderThreadTasks.clear();

    // Begin UI drawing.
    imguiBeginFrame(0, 0, 0, 0, static_cast<UInt16>(platformData.renderWidth), static_cast<UInt16>(platformData.renderHeight));
    
    // Render the windows.
    perfWindow->render(nullptr);

    // End UI drawing.
    imguiEndFrame();

    // Submit an empty draw call in case nothing is drawn to the screen.
    bgfx::touch(0);
    
    // Sets the current viewport dimensions.
    bgfx::setViewRect(0, 0, 0, static_cast<UInt16>(platformData.renderWidth), static_cast<UInt16>(platformData.renderHeight));
    
    // Set our renderer state properties.
    constexpr UInt64 state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A |
            BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

    bgfx::setState(state);

    if (activeScene.valid() && activeScene->getState() == Scene::State::Loaded) {

        // Get the active camera.
        auto camera = activeScene->getCameraManager()->getActiveCamera();

        // Give the view and projection matrices to the vertex shader.
        bgfx::setViewTransform(0, camera->getViewMatrix().data, camera->getProjectionMatrix().data);

        // Render the components in our scene.
        activeScene->renderComponents();

    }

    // Render the frame.
    bgfx::frame();
    
}

void Engine::schedule(Threads thread, std::function<void()> const& function)
{

    switch (thread)
    {
    case Threads::Main:
        mainThreadTasks.push_back(function);
        break;
    case Threads::Render:
        renderThreadTasks.push_back(function);
        break;
    case Threads::New:
#ifdef __APPLE__
        auto th = std::thread(function);
#else
        auto th = std::jthread(function);
#endif
        th.detach();
        break;
    }
           
}

void Engine::unloadScene() {

    if (activeScene.valid())
    {
        Console::getLogger()->info("Unloading scene {}...", activeScene->getSceneName());
        
        schedule(Threads::Render, [this]()
        {
            
            String name = activeScene->getSceneName();
            activeScene->cleanup();
            activeScene = nullptr;
            Console::getLogger()->info("Unloaded scene {}.", name);
            
        });
    }

}

void Engine::stop() {

    // Stops the engine.
    running = false;

}

void Engine::cleanup() {

    if (isRunning()) { return; }

    Console::getLogger()->info("Cleaning up...");
    
    if(activeScene.valid())
    {
        String name = activeScene->getSceneName();
        Console::getLogger()->info("Unloading scene {}...", name);
        activeScene->cleanup();
        activeScene = nullptr;
        Console::getLogger()->info("Unloaded scene {}.", name);
    }
    
    // Release the allocated engine resources.
    inputManager.release();
    activeScene = nullptr;
    resourceLoader.release();
    perfWindow.release();

    imguiDestroy();

    bgfx::shutdown();

    // Remove reference to the instance
    Engine::instance = nullptr;

    Console::getLogger()->info("Exited MidnightEngine.");
    
}

const char* Engine::getNiceRendererName() {

    // Return a nice string with the renderer name.
    switch (bgfx::getRendererType()) {

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

const char* Engine::getNiceGpuName() {
    
    // Return a nice string with the GPU vendor.
    switch (const bgfx::Caps *caps = bgfx::getCaps(); caps->vendorId) {
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
