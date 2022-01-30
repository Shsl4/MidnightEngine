#import <Memory/Allocator.h>
#import <Core/Engine.h>
#import <Platform/macOS/Entry.h>

#import <SDL2/SDL_syswm.h>

#import <bgfx/bgfx.h>
#import <bgfx/platform.h>
#import <bgfx/imgui/imgui.h>

@implementation Entry {

    BOOL hasTerminated;
    std::unique_ptr<Engine> engine;

}

- (int)entry:(int)argc argv:(const char *_Nonnull *_Nonnull)argv {

    [NSApplication sharedApplication];
    [NSApp setDelegate:self];
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

    /// \todo Implement a correct multi-threaded paradigm
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{

        [self initEngine:data];

    });

    while (!engine || !engine->isRunning()) {
        bgfx::renderFrame();
    }

    while (!hasTerminated) {

        [self update];

    }

    SDL_DestroyWindow(window);

    return 0;

}

- (int)initEngine:(PlatformData)data {

    engine = std::make_unique<Engine>(data);
    engine->init(0, nil);

    engine = nullptr;
    imguiDestroy();
    bgfx::shutdown();

    hasTerminated = YES;

    return 0;

}

- (void)update {

    bgfx::renderFrame();

    if (!engine) { return; }
    
    engine->update();

}

@end

int main(int argc, const char **argv) {
    
    Entry* entry = [[Entry alloc] init];
    [entry entry:argc argv:argv];
    [entry release];
        
    return 0;

}
