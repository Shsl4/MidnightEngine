#include <Engine.h>

#include <Scene/CameraComponent.h>
#include <Scene/MeshObject.h>
#include <Scene/FlyingCharacter.h>
#include <Rendering/ShaderLoader.h>
#include <Input/InputManager.h>
#include <UI/PerformanceWindow.h>
#include <UI/CharacterInfoWindow.h>
#include <Rendering/Mesh.h>

#include <bgfx/imgui/imgui.h>

Logger Logger::assertLogger = Logger("Assert");

MEngine::MEngine(SDL_Window *mainWindow) : mainWindow(mainWindow) {
    instance = this;

    this->logger = std::make_unique<Logger>("MidnightEngine");
    this->inputManager = std::make_unique<InputManager>();
    this->perfWindow = std::make_unique<PerformanceWindow>();
    this->meshLoader = std::make_unique<MeshLoader>();
    SDL_GetWindowSize(mainWindow, &windowWidth, &windowHeight);

}

MEngine::~MEngine() {

    cleanup();

}

int MEngine::init(int argc, const char **argv) {

    getLogger()->info("Initializing MidnightEngine...");

    bgfx::Init init;

#ifdef VSYNC

    init.resolution.reset = BGFX_RESET_VSYNC;

#else

    init.resolution.reset = 0;

#endif

    if (!bgfx::init(init)) {

        getLogger()->fatal("Failed to initialize BGFX!");
        bgfx::shutdown();
        return -1;

    }

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0
    );

    timeOffset = bx::getHPCounter();

    this->activeScene = std::make_unique<Scene>();

    inputManager->bindEvent(this, KeyBind(SDLK_ESCAPE), EInputEvent::Pressed, &MEngine::stop);

    activeScene->createObject<MeshObject>(Transform(Vector3(0.0f, 0.0f, 5.0f)), "Monkey.obj");
    activeScene->createObject<MeshObject>(Transform(Vector3(0.0f, 0.0f, -5.0f)), "Cube.obj");
    activeScene->createObject<MeshObject>(Transform(Vector3(5.0f, 0.0f, 0.0f)), "Lantern_Sphere.obj");
    activeScene->createObject<MeshObject>(Transform(Vector3(-5.0f, 0.0f, 0.0f)), "Lamp1.obj");
    auto character = activeScene->createObject<FlyingCharacter>(Transform(Vector3(0.0, 0.0, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)));

    characterWindow = std::make_unique<CharacterInfoWindow>(character);

    imguiCreate();

    getLogger()->info("Initialized MidnightEngine! Now rendering using {} on {}", getNiceRendererName(), getNiceGPUName());

    running = true;

    while (isRunning()) {

        render();

    }

    return 0;

}

void MEngine::update() {

    const Int64 now = bx::getHPCounter();
    const Int64 freq = bx::getHPFrequency();
    time = static_cast<float>(now - timeOffset) / static_cast<float>(freq);
    deltaTime = time - lastTime;
    lastTime = time;

    inputManager->update();
    activeScene->updateScene(deltaTime);

}

void MEngine::render() {

    bgfx::setViewRect(0, 0, 0, static_cast<UInt16>(windowWidth), static_cast<UInt16>(windowHeight));
    bgfx::touch(0);

    imguiBeginFrame(0, 0, 0, 0, static_cast<UInt16>(windowWidth), static_cast<UInt16>(windowHeight));
    perfWindow->render(nullptr);
    characterWindow->render(nullptr);
    imguiEndFrame();

    constexpr UInt64 state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A |
            BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;

    bgfx::setState(state);

    CameraComponent *camera = activeScene->getCameraManager()->getActiveCamera();

    bgfx::setViewTransform(0, camera->getViewMatrix().data, camera->getProjectionMatrix().data);

    activeScene->renderComponents();

    bgfx::frame();

}

void MEngine::stop() {

    running = false;

}

void MEngine::cleanup() {

    getLogger()->info("Destroying MidnightEngine...");

    instance = nullptr;

    this->logger = nullptr;
    this->inputManager = nullptr;
    this->activeScene = nullptr;
    this->perfWindow = nullptr;
    this->characterWindow = nullptr;
    this->meshLoader = nullptr;

}

std::string MEngine::getNiceRendererName() const {

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

std::string MEngine::getNiceGPUName() const {

    const bgfx::Caps *caps = bgfx::getCaps();

    switch (caps->vendorId) {
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
