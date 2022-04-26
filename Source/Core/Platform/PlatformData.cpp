#include <Platform/PlatformData.h>

PlatformData::PlatformData(const int argc, const char **argv, SDL_Window *window, float scaleFactor)
    : argc(argc), argv(argv), mainWindow(window), dpiScale(scaleFactor) {
    
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    
    this->renderWidth = windowWidth * dpiScale;
    this->renderHeight = windowHeight * dpiScale;

}
