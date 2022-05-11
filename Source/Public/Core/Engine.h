#pragma once

#include <Object.h>

#include <Memory/Array.h>
#include <Memory/UniquePointer.h>

#include <Input/InputManager.h>
#include <UI/PerformanceWindow.h>

#include <Platform/PlatformData.h>
#include <Rendering/ResourceLoader.h>

#include <Scene/Scene.h>
#include <Console/Console.h>

/*!
 *  The main engine class.
 */
class ENGINE_API Engine : public Object
{
public:
 
    enum class Threads
    {
        Main,
        Render,
        New
    };

    /*!
     * The engine constructor. Initializes engine resources.
     *
     */
    Engine();

    /*!
     * The engine destructor. It deallocates all resources used by the class.
     */
    ~Engine() override = default;

    /*!
     * Stops the engine.
     */
    void stop();

    /*!
     * Returns a nicely formatted string containing the current renderer name.
     *
     *  \return The renderer name.
     */
    static const char* getNiceRendererName();

    /*!
     * Returns a nicely formatted string containing the current GPU name.
     *
     *  \return The GPU name.
     */
    static const char* getNiceGpuName();

    /*!
     * Returns the unique engine instance.
     *
     *  \return The engine instance.
     */
    FORCEINLINE static Engine* getInstance()
    {
        return instance;
    }

    /*!
     * Returns the platform data.
     *
     *  \return The platform data.
     */
    FORCEINLINE PlatformData getPlatformData() const
    {
        return platformData;
    }

    /*!
     * Checks whether the engine is running.
     *
     *  \return Whether the engine is running.
     */
    FORCEINLINE bool isRunning() const
    {
        return this->running;
    }

    /*!
     * Returns the engine main thread delta time.
     *
     * \return The engine delta time.
     */
    FORCEINLINE float getDeltaTime() const
    {
        return this->deltaTime;
    }

    /*!
     * Returns the time (in seconds) elapsed since the engine was started.
     *
     *  \return The engine time in seconds.
     */
    FORCEINLINE float getTime() const
    {
        return time;
    }

    /*!
     * Returns the currently loaded engine scene.
     *
     * \return The active scene.
     */
    FORCEINLINE WeakPointer<Scene> getActiveScene() const
    {
        return activeScene.weak();
    }

    /*!
     * Returns the engine InputManager instance.
     *
     * \return The engine InputManager.
     */
    FORCEINLINE InputManager* getInputManager() const
    {
        return inputManager.raw();
    }

    /*!
     * Returns the engine InputManager instance.
     *
     * \return The engine InputManager.
     */
    FORCEINLINE const ResourceLoader* getResourceLoader() const
    {
        return resourceLoader.raw();
    }

    FORCEINLINE const Console* getConsole() const
    {
        return console.raw();
    }

protected:

    void schedule(Threads thread, std::function<void()> const& function);

    template <class SceneClass>
    void loadScene()
    {
        static_assert(std::is_convertible_v<SceneClass*, Scene*>, "SceneClass must publicly inherit Scene");

        unloadScene();
        
        schedule(Threads::Render, [this]()
        {
            activeScene = SharedPointer<SceneClass>::make();
            Console::getLogger()->info("Loading scene of type {}...", activeScene->getClassName());
            activeScene->load();
            Console::getLogger()->success("Successfully loaded scene {}", activeScene->getSceneName());
        });
     
    }

    void unloadScene();

    virtual void onStart() = 0;

    virtual void onUpdate() = 0;

private:
    
    friend class Entry;
    friend class Console;

    /*!
     *  The engine's initialization function. It is called after the constructor.
     *
     *  \param[in] argc The console argument count.
     *  \param[in] argv The console arguments.
     *  \return The engine exit code.
     */
    int init(int argc, const char** argv, PlatformData const& data);

    /*!
     *  The engine's loop function. It runs on the main engine thread.
     */
    void update();

    /*!
     *  The engine's render function. It runs on the render thread.
     */
    void render();

    /*!
    * Function called on engine destruction. It deallocates all resources used by the class.
    */
    void cleanup();

    /*!
     *  The static engine instance.
     */
    inline static Engine* instance = nullptr;

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
    UniquePointer<InputManager> inputManager;
 
    /*!
     * The currently loaded Scene.
     */
    SharedPointer<Scene> activeScene;

    /*!
    * The engine resource loader.
    */
    UniquePointer<ResourceLoader> resourceLoader;
    
    UniquePointer<Console> console;

    UniquePointer<PerformanceWindow> perfWindow;

    std::vector<std::function<void()>> renderThreadTasks;
    std::vector<std::function<void()>> mainThreadTasks;
 
};
