#include <Platform/PlatformData.h>
#include <SDL2/SDL_syswm.h>



PlatformData::PlatformData(const int argc, const char **argv, SDL_Window *window, float scaleFactor)
    : argc(argc), argv(argv), mainWindow(mainWindow), dpiScale(scaleFactor) {
    
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    
    this->renderWidth = windowWidth * dpiScale;
    this->renderHeight = windowHeight * dpiScale;


}
