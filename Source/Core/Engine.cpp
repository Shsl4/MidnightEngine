#include <Engine.h>

#include <Math/Matrix4.h>
#include <Math/MathUtils.h>

#include <Scene/CameraComponent.h>
#include <Scene/MeshObject.h>
#include <Scene/FlyingCharacter.h>

#include <Input/InputManager.h>

#include <UI/PerformanceWindow.h>
#include <UI/CharacterInfoWindow.h>

#include <Rendering/Mesh.h>

#include <Memory/String.h>
#include <Memory/UniquePtr.h>

#include <bx/timer.h>
#include <bgfx/bgfx.h>
#include <bgfx/imgui/imgui.h>

Logger Logger::assertLogger = Logger("Assert");

Engine::Engine(PlatformData const &data) : platformData(data) {
    
    // If an engine instance has already been created, abort the program. (Should never happen)
    if(Engine::instance) { abort(); }
    
    Engine::instance = this;

    // Initialize our variables.
    this->logger = UniquePtr<Logger>::make("MidnightEngine");
    this->inputManager = UniquePtr<InputManager>::make();
    this->perfWindow = UniquePtr<PerformanceWindow>::make();
    this->activeScene = UniquePtr<Scene>::make();
    this->resourceLoader = UniquePtr<ResourceLoader>::make();
    this->startTime = bx::getHPCounter();

}

int Engine::init(int argc, const char **argv) {
    
    logger->info("Initializing MidnightEngine...");

    // Initialize BGFX.
    if (!bgfx::init()) {

        // If it fails, print an error message and return.
        logger->fatal("Failed to initialize BGFX!");
        bgfx::shutdown();
        return -1;

    }
    
    bgfx::reset(platformData.renderWidth, platformData.renderHeight, BGFX_RESET_HIDPI);

    // Sets the world "void" to be a grey color.
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);

    resourceLoader->init();
    
    // Add elements in our scene. This is temporary and will be moved elsewhere later.
    activeScene->createObject<MeshObject>(Transform({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 90.0f }, Vector3(5.0f)), "Donut");
    activeScene->createObject<MeshObject>(Transform({5.0f, 0.0f, 0.0f }), "Lamp1");
    activeScene->createObject<MeshObject>(Transform({-5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 90.0f }, Vector3(0.5f)), "Lamp1");
    activeScene->createObject<MeshObject>(Transform({0.0f, 0.0f, -5.0f }), "Cube");
        
    // Create our controllable character.
    auto character = activeScene->createObject<FlyingCharacter>(Transform(Vector3(0.0, 0.0, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)));

    // Create our character info window.
    characterWindow = UniquePtr<CharacterInfoWindow>::make(character);
    
    // Add an input event. If the escape key is pressed, the engine will exit. This will be removed later.
    inputManager->bindEvent(this, KeyBind(SDLK_ESCAPE), EInputEvent::Pressed, &Engine::stop);

    // Initializes our UI system.
    imguiCreate();
    
    // As everything went fine, print an info message.
    logger->info("Initialized MidnightEngine! Now rendering using {} on {}", getNiceRendererName(), getNiceGpuName());
    
    running = true;

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
    if (activeScene.hasValue()) {
        
        // Update the current scene.
        activeScene->updateScene(deltaTime);

    }

}

void Engine::render() {

    // Submit an empty draw call in case nothing is drew to the screen.
    bgfx::touch(0);
    
    // Sets the current viewport dimensions.
    bgfx::setViewRect(0, 0, 0, static_cast<UInt16>(platformData.renderWidth), static_cast<UInt16>(platformData.renderHeight));
    
    // Begin UI drawing.
    imguiBeginFrame(0, 0, 0, 0, static_cast<UInt16>(platformData.renderWidth), static_cast<UInt16>(platformData.renderHeight));
    
    // Render the windows.
    perfWindow->render(nullptr);
    characterWindow->render(nullptr);
    
    // End UI drawing.
    imguiEndFrame();

    // Set our renderer state properties.
    constexpr UInt64 state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A |
            BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

    bgfx::setState(state);

    if (activeScene.hasValue()) {

        // Get the active camera.
        const CameraComponent* camera = activeScene->getCameraManager()->getActiveCamera();

        // Give the view and projection matrices to the vertex shader.
        bgfx::setViewTransform(0, camera->getViewMatrix().data, camera->getProjectionMatrix().data);

        // Render the components in our scene.
        activeScene->renderComponents();

    }

    // Render the frame.
    bgfx::frame();
    
}

void Engine::stop() {

    // Stops the engine.
    running = false;

}

void Engine::cleanup() {

    if (isRunning()) { return; }

    logger->info("Cleaning up...");
    
    // Remove reference to the instance
    Engine::instance = nullptr;
    
    // Release the allocated engine resources.
    this->logger = nullptr;
    this->inputManager = nullptr;
    this->activeScene = nullptr;
    this->resourceLoader = nullptr;
    this->perfWindow = nullptr;
    this->characterWindow = nullptr;

}

String Engine::getNiceRendererName() {

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

String Engine::getNiceGpuName() {
    
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
