//
//  Platform_macOS.m
//  BGFXDemo
//
//  Created by Pierre on 14/11/2021.
//

#import "Bridge.h"
#import <bgfx/bgfx.h>
#import <bgfx/platform.h>
#import <bx/thread.h>
#import <Core/BGFXEngine.h>

@implementation Bridge

+ (void) initEngine:(NSWindow*)window{
    
    bgfx::PlatformData data;
    data.ndt = nil;
    data.nwh = window;
    data.context = nil;
    data.backBuffer = nil;
    data.backBufferDS = nil;
    bgfx::setPlatformData(data);
    
    BGFXEngine engine;
    engine.init(0, nil);
        
    while (NSApp.isRunning) {
        
        engine.loop();
        
    }
    
    
}

+ (void) renderFrame{
    
    bgfx::renderFrame();
    
}

@end
