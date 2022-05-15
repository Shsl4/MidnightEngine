#import <Memory/Allocator.h>
#import <Memory/UniquePointer.h>
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

    const auto console = UniquePointer<Console>::make(engine);
    
    console->init();
    
    try {
        
        if (engine->init(0, nullptr, data)) { return 1; }
                
        while (engine->isRunning())
        {
            engine->render();
        }
        
    }
    catch (Exception const& e) {

        Console::getLogger()->fatal(e.what());
        Console::getLogger()->fatal("Stack Trace: ");

        const Array<StackFrame> stackFrames = e.getStackTrace();
        size_t frameIndex = 0;
        
        for (size_t i = 2; i < stackFrames.getSize(); ++i) {
            
            StackFrame& frame = stackFrames[i];

            if(frame.lineNumber == -1) {

                Console::getLogger()->fatal("[{}] {} <{}> | ({})",
                    frameIndex,
                    frame.functionName,
                    frame.fileName,
                    frame.libraryName);
                
            }
            else {

                Console::getLogger()->fatal("[{}] {} in file {} at line {} | ({})",
                    frameIndex,
                    frame.functionName,
                    frame.fileName,
                    frame.lineNumber,
                    frame.libraryName);
                
            }
                        
            ++frameIndex;
            
        }

        engine->cleanup();
        
        hasTerminated = YES;

        return 2;
        
    }

    Console::getLogger()->info("Cleaning up...");

    engine->cleanup();
    
    hasTerminated = YES;
    
    Console::getLogger()->info("Exited MidnightEngine.");

    return 0;

}

void Entry::mainThread() const {
    
    engine->update();
    bgfx::renderFrame();

}
