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

class MEngine : public Object {
public:

    explicit MEngine(SDL_Window *mainWindow);

    ~MEngine() override;

    virtual int init(int argc, const char **argv);

    virtual void update();

    virtual void render();

    virtual void stop();

    std::string getNiceRendererName() const;

    std::string getNiceGPUName() const;

    FORCEINLINE static MEngine *getInstance() {
        return instance;
    };

    FORCEINLINE bool isRunning() const {
        return this->running;
    }

    FORCEINLINE double getDeltaTime() const {
        return this->deltaTime;
    }

    FORCEINLINE double getTime() const {
        return time;
    }

    FORCEINLINE const class Logger *getLogger() const {
        return logger.get();
    };

    FORCEINLINE const class Scene *getActiveScene() const {
        return activeScene.get();
    };

    FORCEINLINE class InputManager *getInputManager() const {
        return inputManager.get();
    };

private:

    void cleanup();

    inline static MEngine *instance = nullptr;
    SDL_Window *mainWindow = nullptr;

    float time = 0.0f;
    float lastTime = 0.0f;
    float deltaTime = 0.0f;
    bool running = false;

    Int64 timeOffset = 0;
    Int32 windowWidth = 0;
    Int32 windowHeight = 0;

    UniquePtr<class InputManager> inputManager;
    UniquePtr<class Logger> logger;
    UniquePtr<class Scene> activeScene;
    UniquePtr<class PerformanceWindow> perfWindow;
    UniquePtr<class CharacterInfoWindow> characterWindow;
    UniquePtr<class MeshLoader> meshLoader;

};
