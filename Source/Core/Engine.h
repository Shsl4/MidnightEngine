#pragma once

#include <Object.h>
#include <EngineTypes.h>
#include <Math/Matrix4.h>
#include <Math/MathUtils.h>
#include <Memory/Array.h>
#include <Platform/PlatformData.h>

/*!
 *  The main engine class.
 */
class Engine : public Object {
    
public:

    /*!
     * The engine constructor. Initializes engine resources.
     *
     * \param[in] data The platform data.
     */
    explicit Engine(PlatformData const &data);

    /*!
     * The engine destructor. It deallocates all resources used by the class.
     */
    ~Engine() override;

    /*!
     *  The engine's initialization function. It is called after the constructor.
     *
     *  \param[in] argc The console argument count.
     *  \param[in] argv The console arguments.
     *  \return The engine exit code.
     */
    int init(int argc, const char **argv);

    /*!
     *  The engine's loop function. It runs on the main engine thread.
     */
    void update();

    /*!
     *  The engine's render function. It runs on the render thread.
     */
    void render();

    /*!
     * Stops the engine.
     */
    void stop();

    /*!
     * Returns a nicely formatted string containing the current renderer name.
     *
     *  \return The renderer name.
     */
    String getNiceRendererName() const;

    /*!
     * Returns a nicely formatted string containing the current GPU name.
     *
     *  \return The GPU name.
     */
    String getNiceGPUName() const;

    /*!
     * Returns the unique engine instance.
     *
     *  \return The engine instance.
     */
    FORCEINLINE static Engine *getInstance() {
        return instance;
    };

    /*!
     * Returns the platform data.
     *
     *  \return The platform data.
     */
    FORCEINLINE const PlatformData getPlatformData() const {
        return platformData;
    };

    
    /*!
     * Checks whether the engine is running.
     *
     *  \return Whether the engine is running.
     */
    FORCEINLINE bool isRunning() const {
        return this->running;
    }

    /*!
     * Returns the engine main thread delta time.
     *
     * \return The engine delta time.
     */
    FORCEINLINE double getDeltaTime() const {
        return this->deltaTime;
    }

    /*!
     * Returns the time (in seconds) elapsed since the engine was started.
     *
     *  \return The engine time in seconds.
     */
    FORCEINLINE double getTime() const {
        return time;
    }

    /*!
     * Returns the currently loaded engine scene.
     *
     * \return The active scene.
     */
    FORCEINLINE const class Scene *getActiveScene() const {
        return activeScene.get();
    };

    /*!
     * Returns the engine InputManager instance.
     *
     * \return The engine InputManager.
     */
    FORCEINLINE class InputManager *getInputManager() const {
        return inputManager.get();
    };

private:

    /*!
     * Function called on engine destruction. It deallocates all resources used by the class.
     */
    void cleanup();

    /*!
     *  The static engine instance.
     */
    inline static Engine *instance = nullptr;
    
    /*!
     *  The engine main window
     */
    SDL_Window *mainWindow = nullptr;

    /*!
     *  The engine time in seconds
     */
    float time = 0.0f;
    
    /*!
     *  The engine time on last tick
     */
    float lastTime = 0.0f;
    
    /*!
     *  The time elapsed since last tick
     */
    float deltaTime = 0.0f;
    
    /*!
     * The engine run state
     */
    bool running = false;

    /*!
     * The time at which the engine was started.
     */
    Int64 startTime = 0;
    
    /*!
     * Holds info about the window and render resolutions.
     */
    PlatformData platformData;

    /*!
     * The engine InputManager.
     */
    UniquePtr<class InputManager> inputManager;
    
    /*!
     * The engine Logger.
     */
    UniquePtr<class Logger> logger;
    
    /*!
     * The currently loaded Scene.
     */
    UniquePtr<class Scene> activeScene;
    
    /*!
     * The displayed PerformanceWindow.
     *
     * \deprecated Used for debugging purposes, will be removed later.
     */
    UniquePtr<class PerformanceWindow> perfWindow;
    
    /*!
     * The displayed CharacterWindow.
     *
     * \deprecated Used for debugging purposes, will be removed later.
     */
    UniquePtr<class CharacterInfoWindow> characterWindow;
    
    /*!
     * The engine MeshLoader.
     */
    UniquePtr<class MeshLoader> meshLoader;

};