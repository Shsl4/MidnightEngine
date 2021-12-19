#include <Engine.h>

#include <Scene/CameraComponent.h>
#include <Rendering/ShaderLoader.h>
#include <Rendering/Vertex.h>
#include <Math/Matrix4.h>
#include <Math/Matrix3.h>
#include <Input/InputManager.h>
#include <UI/PerformanceWindow.h>

#include <bgfx/imgui/imgui.h>
#include <SDL2/SDL_syswm.h>
#include <Scene/Scene.h>

Logger Logger::assertLogger = Logger("Assert");

struct VertexLayout
{

    static void init()
    {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float, true)
            .end();
    };

    static inline bgfx::VertexLayout ms_layout = {};

};

Vertex triangle[3] =
{

    Vertex(0.0f, 0.69282f - 0.23094f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f),
    Vertex(-0.4f, -0.23094f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f),
    Vertex(0.4f, -0.23094f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f),

};

static Vertex s_cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {-1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, -1.0f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {-1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f,  1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    {-1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
    { 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
};

static const uint16_t s_cubeTriList[] =
{
    0, 1, 2, // 0
    1, 3, 2,
    4, 6, 5, // 2
    5, 6, 7,
    0, 2, 4, // 4
    4, 2, 6,
    1, 5, 3, // 6
    5, 7, 3,
    0, 4, 1, // 8
    4, 5, 1,
    2, 3, 6, // 10
    6, 3, 7,
};


bgfx::ProgramHandle program;
bgfx::VertexBufferHandle triangleBuffer;
bgfx::IndexBufferHandle indexBuffer;

MEngine::MEngine(SDL_Window* mainWindow) : mainWindow(mainWindow) {
    instance = this;

    this->logger = std::make_unique<Logger>("MidnightEngine");
    this->inputManager = std::make_unique<InputManager>();

    SDL_GetWindowSize(mainWindow, &windowWidth, &windowHeight);
    
}

MEngine::~MEngine() {

    cleanup();

}

void MEngine::mouseMotion(int x, int y){
    
    camera->addCameraPitchInput(static_cast<float>(y) / 10.0f);
    camera->addCameraYawInput(static_cast<float>(x) / 10.0f);
    
}

int MEngine::init(int argc, const char** argv){
    
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
    
    bgfx::setViewClear(0
        , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
        , 0x303030ff
        , 1.0f
        , 0
    );

    timeOffset = bx::getHPCounter();

    inputManager->bindEvent(this, KeyBind(SDLK_ESCAPE), EInputEvent::Pressed, &MEngine::stop);

    inputManager->bindEvent(this, KeyBind(SDLK_w), EInputEvent::Pressed, &MEngine::wPressed);
    inputManager->bindEvent(this, KeyBind(SDLK_s), EInputEvent::Pressed, &MEngine::sPressed);
    inputManager->bindEvent(this, KeyBind(SDLK_a), EInputEvent::Pressed, &MEngine::aPressed);
    inputManager->bindEvent(this, KeyBind(SDLK_d), EInputEvent::Pressed, &MEngine::dPressed);
    inputManager->bindEvent(this, KeyBind(SDLK_LSHIFT), EInputEvent::Pressed, &MEngine::shiftPressed);
    inputManager->bindEvent(this, KeyBind(SDLK_SPACE), EInputEvent::Pressed, &MEngine::spacePressed);

    inputManager->bindEvent(this, KeyBind(SDLK_w), EInputEvent::Released, &MEngine::wReleased);
    inputManager->bindEvent(this, KeyBind(SDLK_s), EInputEvent::Released, &MEngine::sReleased);
    inputManager->bindEvent(this, KeyBind(SDLK_a), EInputEvent::Released, &MEngine::aReleased);
    inputManager->bindEvent(this, KeyBind(SDLK_d), EInputEvent::Released, &MEngine::dReleased);
    inputManager->bindEvent(this, KeyBind(SDLK_LSHIFT), EInputEvent::Released, &MEngine::shiftReleased);
    inputManager->bindEvent(this, KeyBind(SDLK_SPACE), EInputEvent::Released, &MEngine::spaceReleased);

    inputManager->bindMouseMovement(this, &MEngine::mouseMotion);

    this->activeScene = std::make_unique<Scene>();


    VertexLayout::init();

    triangleBuffer = createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), VertexLayout::ms_layout);
    indexBuffer = createIndexBuffer(bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList)));
    program = ShaderLoader::loadProgram("Default");
    
    float ratio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    camera = activeScene->createComponent<CameraComponent>(Transform(Vector3(0.0, 0.0, 3.0f), Vector3(-90.0f, 0.0f, 0.0f)), 90.0f, ratio, 500.0f);
    perfWindow = std::make_unique<PerformanceWindow>();

    imguiCreate();

    getLogger()->info("Initialized MidnightEngine! Now rendering using {} on {}", getNiceRendererName(), getNiceGPUName());

    running = true;
    
    while (isRunning()) {
        
        render();
        
    }
    
    return 0;
    
}

void MEngine::update() {

    const int64_t now = bx::getHPCounter();
    const int64_t freq = bx::getHPFrequency();
    time = static_cast<float>(now - timeOffset) / static_cast<float>(freq);
    deltaTime = time - lastTime;
    lastTime = time;

    inputManager->update();

    camera->addMovementInput(camera->getForwardVector(), static_cast<float>(_wPressed - _sPressed), deltaTime);
    camera->addMovementInput(camera->getRightVector(), static_cast<float>(_dPressed - _aPressed), deltaTime);
    camera->addMovementInput(camera->getUpVector(), static_cast<float>(_spacePressed - _shiftPressed), deltaTime);

    activeScene->updateScene(deltaTime);

}


void MEngine::render(){
    
    bgfx::setViewRect(0, 0, 0, static_cast<uint16_t>(windowWidth), static_cast<uint16_t>(windowHeight));

    bgfx::touch(0);

    int mouseX;
    int mouseY;

    SDL_GetMouseState(&mouseX, &mouseY);

    imguiBeginFrame(mouseX, mouseY, 0, 0, static_cast<uint16_t>(windowWidth), static_cast<uint16_t>(windowHeight));

    perfWindow->render(nullptr);

    imguiEndFrame();

    activeScene->renderComponents();

    constexpr uint64_t state = 0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A | 
        BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;
    
    bgfx::setState(state);
    
    bgfx::setViewTransform(0, camera->getViewMatrix().data, camera->getProjectionMatrix().data);

    setIndexBuffer(indexBuffer);
    setVertexBuffer(0, triangleBuffer);
    submit(0, program);

    Matrix4 rotMatrix = Matrix4::identity();
    rotMatrix.rotateX(cos(time) * 3.0f * deltaTime);

    for (size_t j = 0; j < 8; ++j) {
        s_cubeVertices[j].color.setRed(abs(sin(j % 2 + time + 3.14f / 4.0f)));
        s_cubeVertices[j].color.setGreen(abs(cos(j % 2 + time)));
        s_cubeVertices[j].color.setBlue(abs(sin(j % 2 + time - 3.14f)));
        s_cubeVertices[j].position = rotMatrix * s_cubeVertices[j].position;
    }

    destroy(triangleBuffer);
    triangleBuffer = createVertexBuffer(bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices)), VertexLayout::ms_layout);
    
    bgfx::frame();
        
}

void MEngine::stop(){
    
    running = false;
    
}

void MEngine::cleanup(){

    getLogger()->info("Destroying MidnightEngine...");

    instance = nullptr;

    this->logger = nullptr;
    this->inputManager = nullptr;
    this->activeScene = nullptr;
    this->perfWindow = nullptr;

}

std::string MEngine::getNiceRendererName() const {

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

std::string MEngine::getNiceGPUName() const {

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
