//
//  Main.m
//  MidnightEngine
//
//  Created by Pierre on 27/11/2021.
//

#import "Entry.h"
#import <SDL2/SDL.h>
#import <SDL2/SDL_syswm.h>
#import <bgfx/bgfx.h>
#import <bgfx/platform.h>
#import <bx/thread.h>
#import <Core/Engine.h>

@implementation Entry{
    
    BOOL hasTerminated;
    std::unique_ptr<MEngine> engine;
    
}

- (int) entry:(int)argc argv:(const char* _Nonnull * _Nonnull)argv{

    [NSApplication sharedApplication];
    NSApp.delegate = self;
    [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps: YES];
    [NSApp finishLaunching];
    
    SDL_Window* window = SDL_CreateWindow("Main Window", 1280 / 4, 720 / 4, 1280, 720, 0);

    hasTerminated = NO;

    bgfx::renderFrame();

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
       
        [self initEngine: window];
        
    });
    
    while (!engine || !engine->isRunning()) {
        bgfx::renderFrame();
    }
    
    while(!hasTerminated){
        
        [self update];
        
    }
    
    SDL_DestroyWindow(window);

    return 0;
    
}

- (int) initEngine:(SDL_Window*)window {
    
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
        
    if (!SDL_GetWindowWMInfo(window, &wmi))
    {
        return 0;
    }       

    bgfx::PlatformData data;
    data.ndt = nil;
    data.nwh = wmi.info.cocoa.window;
    data.context = nil;
    data.backBuffer = nil;
    data.backBufferDS = nil;
    bgfx::setPlatformData(data);
    
    engine = std::make_unique<MEngine>(window);
    engine->init(0, nil);
    
    bgfx::shutdown();
    
    hasTerminated = YES;

    return 0;
    
}

- (void) update{

    bgfx::renderFrame();
    
    engine->update();

    NSEvent* event = [Entry getNextEvent];
    
    if(event != nil){
        
        [NSApp sendEvent:event];
        
    }
    
    [NSApp updateWindows];
    
}

+ (NSEvent*) getNextEvent{
    
    return [NSApp nextEventMatchingMask: NSEventMaskAny
                              untilDate: nil
                                 inMode: NSDefaultRunLoopMode
                                dequeue:YES];
    
}


@end

int main(int argc, const char** argv){

    @autoreleasepool {
        [[[Entry alloc] init] entry:argc argv:argv];
    }
    
    return 0;
    
}
