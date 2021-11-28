#pragma once

#include <EngineTypes.h>
#include <Object.h>
#include <Math/MathUtils.h>
#include <Memory/Array.h>
#include <Rendering/RenderObject.h>
#include <SDL2/SDL.h>

#include <bx/timer.h>
#include <bx/commandline.h>
#include <bgfx/bgfx_utils.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

class MEngine : public Object
{
public:

    MEngine();
    
    virtual ~MEngine();
    
    virtual int init(int argc, const char** argv);
    virtual void loop();
    virtual void render();

    virtual void stop();
    virtual void restart();
        
    std::string getNiceRendererName();
    std::string getNiceGPUName();

    FORCEINLINE static MEngine* getInstance() { return instance; };
    FORCEINLINE bool isRunning() { return this->running; }
    FORCEINLINE double getDeltaTime() const { return this->deltaTime; }
    FORCEINLINE double getTime() const { return 0.0; }
    FORCEINLINE const class Logger* getLogger() const { return logger.get(); };
    
    FORCEINLINE void selectGPU(uint16_t deviceId, uint16_t vendorId) {
        this->deviceId = deviceId;
        this->vendorId = vendorId;
    }
    
    static inline bgfx::RendererType::Enum renderer = bgfx::RendererType::Metal;


private:

    void onEscape();
    void onLeftMousePressed();
    void onLeftMouseReleased();
    
    void cleanup();
    
    inline static MEngine* instance = nullptr;
    
    float lastTime;
    float deltaTime;
    bool running;
    
    unsigned int deviceId;
    unsigned int vendorId;
    int64_t timeOffset;

    UniquePtr<class InputManager> inputManager;
    UniquePtr<class Logger> logger;
    UniquePtr<class PerformanceWindow> perfWindow;
    
    ManagedArray<RenderObject> objects;

};
