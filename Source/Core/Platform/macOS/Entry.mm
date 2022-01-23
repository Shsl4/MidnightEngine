#import <Core/Engine.h>
#import <Platform/macOS/Entry.h>

#import <SDL2/SDL_syswm.h>

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

    SDL_Window *window = SDL_CreateWindow("Main Window", 1280 / 4, 720 / 4, 1280, 720, 0);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    hasTerminated = NO;

    bgfx::renderFrame();

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{

        [self initEngine:window];

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

- (int)initEngine:(SDL_Window *)window {

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);

    if (!SDL_GetWindowWMInfo(window, &wmi)) {
        return 0;
    }

    bgfx::PlatformData data;
    data.ndt = nil;
    data.nwh = wmi.info.cocoa.window;
    data.context = nil;
    data.backBuffer = nil;
    data.backBufferDS = nil;
    bgfx::setPlatformData(data);

    engine = std::make_unique<Engine>(window);
    engine->init(0, nil);

    imguiDestroy();
    bgfx::shutdown();

    hasTerminated = YES;

    return 0;

}

- (void)update {

    bgfx::renderFrame();

    engine->update();

    NSEvent *event = [Entry getNextEvent];

    if (event != nil) {

        [NSApp sendEvent:event];

    }

    [NSApp updateWindows];

}

+ (NSEvent *)getNextEvent {

    return [NSApp nextEventMatchingMask:NSEventMaskAny
                              untilDate:nil
                                 inMode:NSDefaultRunLoopMode
                                dequeue:YES];

}


@end

int main(int argc, const char **argv) {

    //@autoreleasepool{
    [[[Entry alloc] init] entry:argc argv:argv];
    //}

    return 0;

}
