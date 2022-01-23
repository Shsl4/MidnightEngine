#ifdef _WIN64

#include <Platform/Windows/Entry.h>
#include <bgfx/bgfx.h>
#include <bgfx/imgui/imgui.h>

#include <thread>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif

int Entry::entry([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
    
    SDL_Window* window = SDL_CreateWindow("Main Window", 1280 / 4, 720 / 4, 1280, 720, 0);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    hasTerminated = false;

    bgfx::renderFrame();

    // Creates our engine thread
    auto jthread = std::jthread(&Entry::initEngine, this, window);

    // Wait for engine creation before looping.
    while (!engine || !engine->isRunning()) {
        bgfx::renderFrame();
    }

    while (!hasTerminated) {
        this->update();
    }

    SDL_DestroyWindow(window);
        
    return 0;

}

int Entry::initEngine(SDL_Window* window) {
    
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version)

    if (!SDL_GetWindowWMInfo(window, &wmi))
    {
        return 0;
    }

    bgfx::PlatformData data;
    data.ndt = nullptr;
    data.nwh = wmi.info.win.window;
    data.context = nullptr;
    data.backBuffer = nullptr;
    data.backBufferDS = nullptr;
    bgfx::setPlatformData(data);

    engine = std::make_unique<Engine>(window);
    engine->init(0, nullptr);

    engine = nullptr;
    imguiDestroy();
    bgfx::shutdown();

    hasTerminated = true;

    return 0;

}

void Entry::update() const {
    
    bgfx::renderFrame();

    if (!engine) { return; }

    engine->update();

}

int main(int argc, const char** argv) {

#ifdef DEBUG_LEAKS

    // Enables memory leak check on Windows
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    auto entry = std::make_unique<Entry>();

    entry->entry(argc, argv);

    return 0;

}

#endif
