#import <Memory/Allocator.h>
#import <Memory/AutoReleasePointer.h>
#import <Core/Engine.h>
#import <Platform/Entry.h>

#import <SDL2/SDL.h>
#import <SDL2/SDL_syswm.h>

#import <bgfx/bgfx.h>
#import <bgfx/platform.h>
#import <bgfx/imgui/imgui.h>

#import <Logging/Logger.h>

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

int Entry::entry([[maybe_unused]] int argc, [[maybe_unused]] const char** argv, std::function<Engine*()> const& provider) {

    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp finishLaunching];

    SDL_Window *window = SDL_CreateWindow("Main Window", 0, 0, 0, 0, 1);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);

    if (!SDL_GetWindowWMInfo(window, &wmi)) {
        return;
    }
    
    bgfx::PlatformData platformData;
    platformData.nwh = wmi.info.cocoa.window;
    bgfx::setPlatformData(platformData);
    
    CGFloat scaleFactor = wmi.info.cocoa.window.screen.backingScaleFactor;
    
    PlatformData data = PlatformData(argc, argv, window, scaleFactor);
    
    hasTerminated = NO;

    bgfx::renderFrame();
    
    engine = provider();

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{

         initEngine(data);

    });

    while (!engine || !engine->isRunning()) {
        bgfx::renderFrame();
    }

    while (!hasTerminated) {

        mainThread();

    }

    SDL_DestroyWindow(window);

    return 0;


}

int Entry::initEngine(PlatformData data) {

    int value = engine->init(0, nil, data);
    
    if (value != 0) { return value; }

    while (engine->isRunning())
    {
        engine->render();
    }

    engine->cleanup();

    hasTerminated = YES;

    return 0;

}

void Entry::mainThread() const {
    
    engine->update();
    bgfx::renderFrame();

}
