#include <Core/Engine.h>
#include <SDL2/SDL.h>
#include <bx/thread.h>
#include <SDL2/SDL_syswm.h>

MEngine* engine = nullptr;

static int32_t threadFunc(bx::Thread* _thread, void* _userData){
    
    engine = new MEngine();
    engine->init(0, nullptr);

}

static void* sdlNativeWindowHandle(SDL_Window* _window)
{
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(_window, &wmi))
    {
        return NULL;
    }

#    if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#        if ENTRY_CONFIG_USE_WAYLAND
    wl_egl_window* win_impl = (wl_egl_window*)SDL_GetWindowData(_window, "wl_egl_window");
    if (!win_impl)
    {
        int width, height;
        SDL_GetWindowSize(_window, &width, &height);
        struct wl_surface* surface = wmi.info.wl.surface;
        if (!surface)
            return nullptr;
        win_impl = wl_egl_window_create(surface, width, height);
        SDL_SetWindowData(_window, "wl_egl_window", win_impl);
    }
    return (void*)(uintptr_t)win_impl;
#        else
    return (void*)wmi.info.x11.window;
#        endif
#    elif BX_PLATFORM_OSX || BX_PLATFORM_IOS
    return wmi.info.cocoa.window;
#    elif BX_PLATFORM_WINDOWS
    return wmi.info.win.window;
#   elif BX_PLATFORM_ANDROID
    return wmi.info.android.window;
#    endif // BX_PLATFORM_

}


int main(int argc, const char** argv) {
    
    bgfx::renderFrame();
    
    SDL_Window* mainWindow = SDL_CreateWindow("MidnightEngine",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280, 720, SDL_WINDOW_SHOWN);

    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(mainWindow, &wmi)) {
        return 1;
    }

    bgfx::PlatformData pd;
#    if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
#        if ENTRY_CONFIG_USE_WAYLAND
    pd.ndt = wmi.info.wl.display;
#        else
    pd.ndt = wmi.info.x11.display;
#        endif
#    else
    pd.ndt = NULL;
#    endif // BX_PLATFORM_
    pd.nwh = sdlNativeWindowHandle(mainWindow);

    pd.context = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;
    bgfx::setPlatformData(pd);
    
    bx::Thread thread;
    thread.init(&threadFunc, nullptr);
    
    while(thread.isRunning()){
        
        bgfx::renderFrame();
        
        if(engine && engine->shouldRender){
            engine->render();
            
        }
        
    }
    
    SDL_DestroyWindow(mainWindow);
    
	return 0;

}
