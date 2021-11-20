#pragma once

#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <UI/PerformanceWindow.h>
#include <Math/MathUtils.h>
#include <bx/timer.h>
#include <bx/commandline.h>
#include <bgfx/bgfx_utils.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL2/SDL.h>

class MEngine
{
public:

    MEngine() {
        
        instance = this;
        logger = std::make_unique<Logger>("MidnightEngine");

    }
    
    virtual ~MEngine();
    
    virtual int init(int argc, const char** argv);
    virtual void render();
    virtual void tick();

    virtual void stop();
    virtual void restart();
    
    inline static bgfx::RendererType::Enum renderer = bgfx::RendererType::Vulkan;

    std::string getNiceRendererName();
    std::string getNiceGPUName();

    virtual FORCEINLINE double getDeltaTime() const { return this->deltaTime; }
    virtual FORCEINLINE double getTime() const { return this->time; }

    FORCEINLINE const Logger* getLogger() const { return logger.get(); };
    FORCEINLINE static MEngine* getInstance() { return instance; };
    
    FORCEINLINE void selectGPU(uint16_t deviceId, uint16_t vendorId) {
        this->deviceId = deviceId;
        this->vendorId = vendorId;
    }
    
    bool shouldRun = true;
    bool shouldRender = false;

private:

    void cleanup();
    
    float time;
    float lastTime;
    float deltaTime;
        
    inline static MEngine* instance = nullptr;
    UniquePtr<Logger> logger;
    
    uint32_t m_width;
    uint32_t m_height;
    uint32_t m_debug;
    uint32_t m_reset;
    int64_t timeOffset;

    uint16_t deviceId = BGFX_PCI_ID_NONE;
    uint16_t vendorId = 0;

    bool shouldRestart = false;

    bgfx::VertexBufferHandle triangleBuffer;
    bgfx::ProgramHandle program;
    PerformanceWindow perfWindow = PerformanceWindow();

};
