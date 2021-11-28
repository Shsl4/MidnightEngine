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
    MEngine* engine;
    
}

- (int) entry:(int)argc argv:(const char* _Nonnull * _Nonnull)argv{

    [NSApplication sharedApplication];
    NSApp.delegate = self;
    [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps: YES];
    [NSApp finishLaunching];
    
    SDL_Window* window = SDL_CreateWindow("Objc Window", 0, 0, 1280, 720, SDL_WINDOW_RESIZABLE);
    
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
        
    if (!SDL_GetWindowWMInfo(window, &wmi))
    {
        return 0;
    }
    
    hasTerminated = NO;
    
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
       
        [self initEngine: wmi.info.cocoa.window];
        
    });
    
    bgfx::renderFrame();
    
    // Wait for engine creation before looping.
    while(!engine){ }
    
    while(!hasTerminated){
        
        [self loop];
        
    }
    
    
}

- (int) initEngine:(NSWindow*)window {
        
    bgfx::PlatformData data;
    data.ndt = nil;
    data.nwh = window;
    data.context = nil;
    data.backBuffer = nil;
    data.backBufferDS = nil;
    bgfx::setPlatformData(data);
    
    engine = new MEngine();
    engine->init(0, nil);
    
    hasTerminated = YES;
    
}

- (void) loop{

    bgfx::renderFrame();
    
    engine->loop();

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
