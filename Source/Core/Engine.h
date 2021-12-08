#pragma once

#include <EngineTypes.h>
#include <Object.h>
#include <Math/Matrix4.h>
#include <Math/MathUtils.h>
#include <Memory/Array.h>
#include <SDL2/SDL.h>

#include <bx/timer.h>
#include <bx/commandline.h>
#include <bgfx/bgfx_utils.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

class MEngine : public Object
{
public:

    MEngine(SDL_Window* mainWindow);
    
    virtual ~MEngine();
    
    virtual int init(int argc, const char** argv);
    virtual void update();
    virtual void render();

    virtual void stop();
        
    std::string getNiceRendererName();
    std::string getNiceGPUName();

    FORCEINLINE static MEngine* getInstance() { return instance; };
    FORCEINLINE bool isRunning() { return this->running; }
    FORCEINLINE double getDeltaTime() const { return this->deltaTime; }
    FORCEINLINE double getTime() const { return time; }
    FORCEINLINE const class Logger* getLogger() const { return logger.get(); };
    FORCEINLINE const class Scene* getActiveScene() const { return activeScene.get(); };

    static inline bgfx::RendererType::Enum renderer = bgfx::RendererType::Metal;
  
    void mouseMotion(int x, int y);

#define inputBoilerplate(name) void name##Pressed() {  _##name##Pressed = true; } \
                               void name##Released() {  _##name##Pressed = false; } \
                               bool _##name##Pressed = false;

    inputBoilerplate(w)
    inputBoilerplate(a)
    inputBoilerplate(s)
    inputBoilerplate(d)
    inputBoilerplate(space)
    inputBoilerplate(shift)

private:

    class CameraComponent* camera;

    void cleanup();
    
    inline static MEngine* instance = nullptr;
    
    float time;
    float lastTime;
    float deltaTime;
    bool running;
    
    int64_t timeOffset;

    int windowWidth;
    int windowHeight;

    UniquePtr<class InputManager> inputManager;
    UniquePtr<class Logger> logger;
    UniquePtr<class Scene> activeScene;
    UniquePtr<class PerformanceWindow> perfWindow;
    

    SDL_Window* mainWindow;

};