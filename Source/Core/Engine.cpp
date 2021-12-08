#include <Engine.h>

#include <Rendering/ShaderLoader.h>
#include <Rendering/Vertex.h>
#include <Rendering/RenderData.h>
#include <Math/Matrix4.h>
#include <Math/Matrix3.h>
#include <Input/InputManager.h>

#include <bgfx/imgui/imgui.h>
#include <SDL2/SDL_syswm.h>
#include <Scene/Scene.h>

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

bgfx::ProgramHandle program;
bgfx::VertexBufferHandle triangleBuffer;

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

void MEngine::mouseMotion(int x, int y){
    
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    cameraYaw   -= xoffset;
    cameraPitch += yoffset;

    if(cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if(cameraPitch < -89.0f)
        cameraPitch = -89.0f;
    
    auto pitchRad = Math::toRadians(cameraPitch);
    auto yawRad = Math::toRadians(cameraYaw);

    direction.x = cos(yawRad) * cos(pitchRad);
    direction.y = sin(pitchRad);
    direction.z = sin(yawRad) * cos(pitchRad);
    cameraFront = Vector3::normalize(direction);
    
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

    getLogger()->info("Initialized MidnightEngine! Now rendering using {} on {}", getNiceRendererName(), getNiceGPUName());

    VertexLayout::init();

    triangleBuffer = bgfx::createVertexBuffer(bgfx::makeRef(triangle, sizeof(triangle)), VertexLayout::ms_layout);

    program = ShaderLoader::loadProgram("Default");
    
    cameraPosition = Vector3(0.0f, 0.0f, 3.0f);
    viewTarget = Vector3(0.0f, 0.0f, 0.0f);
    cameraRotation = Vector3::normalize(cameraPosition - viewTarget);
    cameraRight = Vector3::normalize(Vector3::cross(Vector3::Up, cameraRotation));
    cameraUp = Vector3::cross(cameraRotation, cameraRight);
    
    lastX = windowWidth / 2;
    lastY = windowHeight / 2;

    running = true;
    
    while (isRunning()) {
        
        render();
        
    }
        
    return 0;
    
}

void MEngine::update() {

    const int64_t now = bx::getHPCounter();
    const double freq = double(bx::getHPFrequency());
    time = (float)((now - timeOffset) / freq);
    deltaTime = time - lastTime;
    lastTime = time;

    inputManager->update();
    activeScene->updateScene(deltaTime);

    float speed = 2.5f * deltaTime;
    auto a = cameraFront * speed;
    auto w = (a * _wPressed);
    auto x = (a * _sPressed);
    auto b = Vector3::normalize(Vector3::cross(cameraFront, cameraUp)) * ((_dPressed - _aPressed) * speed);
    
    float upDiff = (_spacePressed - _shiftPressed) * speed;
    Vector3 newVec = Vector3(0.0f, upDiff, 0.0f);
    
    cameraPosition += w;
    cameraPosition -= x;
    cameraPosition -= b;
    cameraPosition += newVec;
    
    if(cameraPitch > 89.0f)
        cameraPitch = 89.0f;
    if(cameraPitch < -89.0f)
        cameraPitch = -89.0f;
    
    auto pitchRad = Math::toRadians(cameraPitch);
    auto yawRad = Math::toRadians(cameraYaw);

    direction.x = cos(yawRad) * cos(pitchRad);
    direction.y = sin(pitchRad);
    direction.z = sin(yawRad) * cos(pitchRad);
    cameraFront = Vector3::normalize(direction);
}


void MEngine::render(){
    
    bgfx::setViewRect(0, 0, 0, uint16_t(windowWidth), uint16_t(windowHeight));

    bgfx::touch(0);

    activeScene->renderComponents();
    
    float ratio;
    ratio = windowWidth / (float)windowHeight;

    const float radius = 10.0f;
    const float camX = sin(time) * radius;
    const float camZ = cos(time) * radius;
    
    Matrix4 perspective = Matrix4::perspective(60.0f, ratio, 0.1f, 100.0f);
    Matrix4 viewMatrix = Matrix4::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);

    uint64_t  _state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_MSAA;
    
    bgfx::setState(_state);
    
    bgfx::setViewTransform(0, viewMatrix.data, perspective.data);
    bgfx::setVertexBuffer(0, triangleBuffer);
    bgfx::submit(0, program);

    Matrix4 rotMatrix = Matrix4::identity();
    //rotMatrix.rotateY(sin(time) / 10.0f);
    rotMatrix.rotateX(cos(time) / 10.0f);

    for (size_t j = 0; j < 3; ++j) {
        triangle[j].color.setRed(abs((float)sin(j % 3 + time + 3.14f / 4.0f)));
        triangle[j].color.setGreen(abs((float)cos(j % 3 + time)));
        triangle[j].color.setBlue(abs((float)sin(j % 3 + time - 3.14f)));
        triangle[j].position = rotMatrix * triangle[j].position;
        triangle[j].position.x += sin(time) / 100.0f;
    }

    bgfx::destroy(triangleBuffer);
    triangleBuffer = bgfx::createVertexBuffer(bgfx::makeRef(triangle, sizeof(triangle)), VertexLayout::ms_layout);

    bgfx::frame();
        
}

void MEngine::stop(){
    
    running = false;
    Vector3 v = Vector3(1, 2, 3);
    Matrix3 m1 = Matrix3::fill(1);
    Matrix3 m2 = Matrix3::fill(2);
    Matrix3 m3 = Matrix3::empty();
    v=m2.operator*(v);
    std::cout << v.x << std::endl;
    m3.print();
    
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
