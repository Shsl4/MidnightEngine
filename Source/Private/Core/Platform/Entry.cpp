#include <Platform/Entry.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <Core/StackTrace.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bgfx/imgui/imgui.h>

#include <thread>

#include <Input/InputManager.h>


#ifndef __APPLE__

#if defined(_WIN64) && defined(_DEBUG)

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#endif

#define MIDNIGHT_FULLSCREEN

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

    const auto console = UniquePointer<Console>::make(engine);
    
    console->init();
    
    try {
        
        if (engine->init(0, nullptr, data)) { return 1; }
        
        while (engine->isRunning())
        {
            engine->render();
        }
        
    }
    catch (Exception const& e) {

        Console::getLogger()->fatal(e.what());
        Console::getLogger()->fatal("Stack Trace: ");

        const Array<StackFrame> stackFrames = e.getStackTrace();
        size_t frameIndex = 0;
        
        for (size_t i = 2; i < stackFrames.getSize(); ++i, ++frameIndex) {
            Console::getLogger()->fatal("[{}] {}", frameIndex, stackFrames[i].format());
        }

        engine->cleanup();
        
        hasTerminated = true;

        return 2;
        
    }

    Console::getLogger()->info("Cleaning up...");

    engine->cleanup();
    
    hasTerminated = true;
    
    Console::getLogger()->info("Exited MidnightEngine.");

    return 0;

}

void Entry::mainThread() const {

    engine->update();
    bgfx::renderFrame();

}

#endif
