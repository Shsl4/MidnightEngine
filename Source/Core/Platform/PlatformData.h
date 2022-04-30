#pragma once

#include <Core/EngineMacros.h>
#include <Core/EngineTypes.h>
#include <SDL2/SDL.h>

struct ENGINE_API PlatformData {

    PlatformData(int argc, const char **argv, SDL_Window *window, float scaleFactor);

    const int argc = 0;
    const char** argv = nullptr;
    SDL_Window* mainWindow = nullptr;

    Int32 windowWidth = 0;
    Int32 windowHeight = 0;
    Int32 renderWidth = 0;
    Int32 renderHeight = 0;
    
    float dpiScale = 1.0f;

};
