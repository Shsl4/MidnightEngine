#include <Platform/PlatformData.h>
#include <SDL2/SDL.h>

PlatformData::PlatformData(const int argc, const char **argv, void *window, float scaleFactor)
    : argc(argc), argv(argv), mainWindow(window), dpiScale(scaleFactor) {
    
    SDL_GetWindowSize(static_cast<SDL_Window*>(window), &windowWidth, &windowHeight);
    
    this->renderWidth = static_cast<Int32>(static_cast<float>(windowWidth) * dpiScale);
    this->renderHeight = static_cast<Int32>(static_cast<float>(windowHeight) * dpiScale);

}
