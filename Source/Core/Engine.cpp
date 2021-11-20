#include <Core/Engine.h>
#include <Core/Rendering/ShaderLoader.h>
#include <Core/Rendering/Vertex.h>
#include <bgfx/imgui/imgui.h>
#include <SDL2/SDL_syswm.h>

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

Args::Args(int _argc, const char* const* _argv)
    : m_type(bgfx::RendererType::Count)
    , m_pciId(BGFX_PCI_ID_NONE)
{
    bx::CommandLine cmdLine(_argc, (const char**)_argv);

    if (cmdLine.hasArg("gl"))
    {
        m_type = bgfx::RendererType::OpenGL;
    }
    else if (cmdLine.hasArg("vk"))
    {
        m_type = bgfx::RendererType::Vulkan;
    }
    else if (cmdLine.hasArg("noop"))
    {
        m_type = bgfx::RendererType::Noop;
    }
    else if (BX_ENABLED(BX_PLATFORM_WINDOWS | BX_PLATFORM_WINRT | BX_PLATFORM_XBOXONE))
    {
        if (cmdLine.hasArg("d3d9"))
        {
            m_type = bgfx::RendererType::Direct3D9;
        }
        else if (cmdLine.hasArg("d3d11"))
        {
            m_type = bgfx::RendererType::Direct3D11;
        }
        else if (cmdLine.hasArg("d3d12"))
        {
            m_type = bgfx::RendererType::Direct3D12;
        }
    }
    else if (BX_ENABLED(BX_PLATFORM_OSX))
    {
        if (cmdLine.hasArg("mtl"))
        {
            m_type = bgfx::RendererType::Metal;
        }
    }

    if (cmdLine.hasArg("amd"))
    {
        m_pciId = BGFX_PCI_ID_AMD;
    }
    else if (cmdLine.hasArg("nvidia"))
    {
        m_pciId = BGFX_PCI_ID_NVIDIA;
    }
    else if (cmdLine.hasArg("intel"))
    {
        m_pciId = BGFX_PCI_ID_INTEL;
    }
    else if (cmdLine.hasArg("sw"))
    {
        m_pciId = BGFX_PCI_ID_SOFTWARE_RASTERIZER;
    }
}

static void* sdlNativeWindowHandle(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi))
    {
        return NULL;
    }

#    if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#        if ENTRY_CONFIG_USE_WAYLAND
    wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
    if (!win_impl)
    {
        int width, height;
        SDL_GetWindowSize(_window, &width, &height);
        struct wl_surface* surface = wmi.info.wl.surface;
        if (!surface)
            return nullptr;
        win_impl = wl_egl_window_create(surface, width, height);
        SDL_SetWindowData(_window, "wl_egl_window", win_impl);
    }
    return (void*)(uintptr_t)win_impl;
#        else
    return (void*)wmi.info.x11.window;
#        endif
#    elif BX_PLATFORM_OSX || BX_PLATFORM_IOS
    return wmi.info.cocoa.window;
#    elif BX_PLATFORM_WINDOWS
    return wmi.info.win.window;
#   elif BX_PLATFORM_ANDROID
    return wmi.info.android.window;
#    endif // BX_PLATFORM_

}

MEngine::~MEngine() {

    getLogger()->debug("Destroying MidnightEngine...");
    
    cleanup();

}

int MEngine::init(int argc, const char** argv){
    
    getLogger()->info("Initializing MidnightEngine...");
    
    Args args(argc, argv);

    m_width = 1280;
    m_height = 720;
    m_debug = BGFX_DEBUG_TEXT;

#ifdef VSYNC

    m_reset = BGFX_RESET_VSYNC;

#else

    m_reset = 0;

#endif

    bgfx::Init init;

    init.type = renderer;
    init.vendorId = vendorId;
    init.deviceId = deviceId;
    init.resolution.width = m_width;
    init.resolution.height = m_height;
    init.resolution.reset = m_reset;

    if (!bgfx::init(init)) {

        getLogger()->fatal("Failed to initialize BGFX!");
        bgfx::shutdown();
        return -1;

    }

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

    this->renderer = caps->rendererType;

    bgfx::setDebug(m_debug);

    bgfx::setViewClear(0
        , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
        , 0x303030ff
        , 1.0f
        , 0
    );

    VertexLayout::init();

    triangleBuffer = bgfx::createVertexBuffer(bgfx::makeRef(triangle, sizeof(triangle)), VertexLayout::ms_layout);

    program = ShaderLoader::loadProgram("Default");

    timeOffset = bx::getHPCounter();

    imguiCreate();

    getLogger()->info("Initialized MidnightEngine! Now rendering using {} on {} (Device ID: {})", getNiceRendererName(), getNiceGPUName(), deviceId);

    shouldRender = true;
    
    while (shouldRun) {

        tick();

    }
    
    if (shouldRestart) {
        cleanup();
        shouldRun = true;
        shouldRestart = false;
        this->init(argc, argv);
    }

    return 0;
    
}

bool left = false, right = false, middle = false;
int mouseX = 0, mouseY = 0;

void MEngine::tick(){
    
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {


        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                shouldRun = false;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:

            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                left = true;
            case SDL_BUTTON_MIDDLE:
                middle = true;
            case SDL_BUTTON_RIGHT:
                right = true;
            default:
                break;
            }

            break;
        case SDL_MOUSEBUTTONUP:

            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                left = false;
            case SDL_BUTTON_MIDDLE:
                middle = false;
            case SDL_BUTTON_RIGHT:
                right = false;
            default:
                break;
            }

            break;
        case SDL_QUIT:
            shouldRun = false;
            break;
        default:
            break;
        }

    }
    
    SDL_GetMouseState(&mouseX, &mouseY);
    
}

void MEngine::render(){
    
    imguiBeginFrame(mouseX
        , mouseY
        , (left ? IMGUI_MBUT_LEFT : 0)
        | (right ? IMGUI_MBUT_RIGHT : 0)
        | (middle ? IMGUI_MBUT_MIDDLE : 0)
        , 0
        , uint16_t(m_width)
        , uint16_t(m_height)
    );

    perfWindow.render(nullptr);

    imguiEndFrame();

    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

    bgfx::touch(0);

    float time = (float)((bx::getHPCounter() - timeOffset) / double(bx::getHPFrequency()));

    float ratio;
    ratio = m_width / (float)m_height;
    Matrix4 rotZ = Matrix4::identity();
    rotZ.rotateZ(time);
    Matrix4 orthographic = Matrix4::orthographic(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f) * rotZ;
    Matrix4 id = Matrix4::identity();

    bgfx::setViewTransform(0, id.data, orthographic.data);

    bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));

    bgfx::setTransform(id.data);
    bgfx::setVertexBuffer(0, triangleBuffer);
    bgfx::submit(0, program);

    for (size_t j = 0; j < 3; ++j) {
        triangle[j].color.setRed(abs((float)sin(j % 3 + time + 3.14f / 4.0f)));
        triangle[j].color.setGreen(abs((float)cos(j % 3 + time)));
        triangle[j].color.setBlue(abs((float)sin(j % 3 + time - 3.14f)));
    }

    bgfx::frame();

    bgfx::destroy(triangleBuffer);
    triangleBuffer = bgfx::createVertexBuffer(bgfx::makeRef(triangle, sizeof(triangle)), VertexLayout::ms_layout);

    return;
    
    
}

void MEngine::restart(){
    
    getLogger()->info("Restarting engine...");

    shouldRun = false;
    shouldRestart = true;
    
}

void MEngine::stop(){
    
    shouldRun = false;
    
}

void MEngine::cleanup(){
    
    imguiDestroy();

    bgfx::destroy(triangleBuffer);
    bgfx::destroy(program);

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
