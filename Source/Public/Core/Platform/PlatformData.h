#pragma once

#include <Core/EngineMacros.h>
#include <Core/EngineTypes.h>

struct ENGINE_API PlatformData {

    PlatformData() = default;

    PlatformData(int argc, const char **argv, void *window, float scaleFactor);
    
    int argc = 0;
    const char** argv = nullptr;
    void* mainWindow = nullptr;

    Int32 windowWidth = 0;
    Int32 windowHeight = 0;
    Int32 renderWidth = 0;
    Int32 renderHeight = 0;
    
    float dpiScale = 1.0f;

};
