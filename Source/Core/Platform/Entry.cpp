#include <Platform/Entry.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bgfx/imgui/imgui.h>

#include <thread>

#ifndef __APPLE__

#if defined(_WIN64) && defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif

int Entry::entry([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {

    SDL_Window* window = SDL_CreateWindow("Main Window", 1280 / 4, 720 / 4, 1280, 720, 0);
    SDL_SetRelativeMouseMode(SDL_TRUE);

#ifdef MIDNIGHT_FULLSCREEN
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
    
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version)

    if (!SDL_GetWindowWMInfo(window, &wmi))
    {
        return 1;
    }

    bgfx::PlatformData platformData;

#ifdef _WIN64
    platformData.nwh = wmi.info.win.window;
#else
    platformData.nwh = reinterpret_cast<void*>(wmi.info.x11.window);
    platformData.ndt = wmi.info.x11.display;
#endif

    bgfx::setPlatformData(platformData);
    
    PlatformData data = PlatformData(argc, argv, window, 1.0f);
    
    hasTerminated = false;

    bgfx::renderFrame();

    // Creates our engine thread
    auto thread = std::jthread(&Entry::initEngine, this, data);

    // Wait for engine creation before looping.
    while (!engine || !engine->isRunning()) {
        bgfx::renderFrame();
    }

    while (!hasTerminated) {
        this->update();
    }

    SDL_DestroyWindow(window);

    thread.detach();
        
    return 0;

}

int Entry::initEngine(PlatformData data) {

    engine = std::make_unique<Engine>(data);
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

#endif
