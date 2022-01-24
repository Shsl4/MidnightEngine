#pragma once

#include <Core/EngineTypes.h>
#include <SDL2/SDL.h>

struct PlatformData {

    PlatformData(const int argc, const char **argv, SDL_Window *window, float scaleFactor);

    const int argc;
    const char** argv;
    SDL_Window* mainWindow;

    Int32 windowWidth;
    Int32 windowHeight;
    Int32 renderWidth;
    Int32 renderHeight;
    
    float dpiScale;

};
