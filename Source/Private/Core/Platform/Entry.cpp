#include <Platform/Entry.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bgfx/imgui/imgui.h>

#include <thread>

#include <Input/InputManager.h>

#include <source_location>


#ifndef __APPLE__

#if defined(_WIN64) && defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif

int Entry::entry([[maybe_unused]] int argc, [[maybe_unused]] const char** argv, std::function<Engine*()> const& provider) {

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

    engine = provider();

    // Creates our engine thread
    auto thread = std::jthread(&Entry::initEngine, this, data);

    // Wait for engine creation before looping.
    while (!engine || !engine->isRunning()) {
        bgfx::renderFrame();
    }

    while (!hasTerminated) {
        this->mainThread();
    }

    SDL_DestroyWindow(window);

    thread.detach();
        
    return 0;

}

int Entry::initEngine(PlatformData data) {

    int value = engine->init(0, nullptr, data);
    
    if (value != 0) { return value; }

    try {
            
        while (engine->isRunning())
        {
            engine->render();
        }
        
    }
    catch (Exception const& e) {
        Console::getLogger()->fatal(fmt::format("{} in file: {}, in function: {}, line: {}", e.what(),
             e.location.file_name(), e.location.function_name(),  e.location.line()).c_str());
        engine->stop();
    }
    
    engine->cleanup();

    hasTerminated = true;

    return 0;

}

void Entry::mainThread() const {

    engine->update();
    bgfx::renderFrame();

}

#endif
