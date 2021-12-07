#ifdef _WIN64

#include <Platform/Windows/Entry.h>
#include <bgfx/bgfx.h>
#include <thread>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int Entry::entry(int argc, const char** argv) {
    
    SDL_Window* window = SDL_CreateWindow("Main Window", 1280 / 4, 720 / 4, 1280, 720, 0);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    hasTerminated = false;

    bgfx::renderFrame();

    std::thread thread = std::thread(&Entry::initEngine, this, window);

    // Wait for engine creation before looping.
    while (!engine || !engine->isRunning()) {
        bgfx::renderFrame();
    }

    while (!hasTerminated) {
        this->update();
    }

    thread.join();

    SDL_DestroyWindow(window);
    
    return 0;

}

int Entry::initEngine(SDL_Window* window) {
    
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);

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

    engine = std::make_unique<MEngine>(window);
    engine->init(0, nullptr);

    bgfx::shutdown();

    hasTerminated = true;

    return 0;

}

void Entry::update() {
    
    bgfx::renderFrame();
    engine->update();

}

int main(int argc, const char** argv) {

    // Enables memory leak check on Windows
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    std::unique_ptr<Entry> entry = std::make_unique<Entry>();

    entry->entry(argc, argv);

    return 0;

}

#endif
