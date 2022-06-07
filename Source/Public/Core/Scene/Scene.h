#pragma once

#include <Object.h>
#include <Scene/Actor.h>
#include <Scene/SceneComponent.h>
#include <Scene/CameraComponent.h>
#include <Scene/CameraManager.h>
#include <Console/Console.h>
#include <Memory/Array.h>
#include <Memory/UniquePointer.h>
#include <Logging/Logger.h>

#include "PxScene.h"
#include "Physics/PhysicsManager.h"

struct ENGINE_API Color {

    static void setByte(UInt32& bytes, UInt8 byte, int pos) {
        bytes &= ~(static_cast<UInt32>(0xff) << (8 * pos));
        bytes |= (static_cast<UInt32>(byte) << (8 * pos));
    }

    Color(UInt8 red, UInt8 green, UInt8 blue);

    UInt32 value;

};

/*!
 *  \brief A scene is an object that represents the world. It holds Actors, and manages their lifetime.
 */
class ENGINE_API Scene : public Object {

public:
    
    enum class State {
        
        Unloaded,
        Loading,
        Loaded,
        Unloading
        
    };

    /*!
     * The scene constructor. Initializes the private variables.
     */
    Scene(PhysicsManager* manager);

    /*!
    * Loads the scene. It sets up and constructs every object in the scene.
    */
    void load();

    virtual void start() = 0;

    /*!
    * Called when the scene is destroyed. It destroys every object in the scene.
    */
    virtual void cleanup();

    void listObjects() const;

    void listComponents() const;

    NODISCARD FORCEINLINE State getState() const { return this->state; }

    NODISCARD FORCEINLINE virtual String getSceneName() const = 0;

    void setWorldColor(UInt32 color) const;

    NODISCARD Actor* getObjectByIndex(size_t index) const;

    /*!
     *  Instantiates a new Actor in the scene
     *
     *  \tparam T The Actor class to instantiate
     *  \param[in] transform The world Transform of the Actor
     *  \return The newly created Actor
     */
    template <class T>
    T* createActor(Transform const& transform = Transform::zero()) {

        // Check if the class we are trying to instantiate is a Actor.
        static_assert(std::is_base_of_v<Actor, T>, "T should inherit from Actor");

        // Instantiate the Actor
        SharedPointer<T> actor = SharedPointer<T>::make();
        
        // If the Actor did not setup a root component, create a default one.
        if (!actor->rootComponent) {
            SceneComponent* comp = actor->template createComponent<SceneComponent>("DefaultRoot");
            comp->attachTo(actor.raw());
        }

        actor->setWorldTransform(transform);

        // Setup the input on the created Actor
        setupInput(actor.raw());

        // Register the Actor
        registeredActors += actor;

        return actor.raw();

    }

    template<class T>
    T* getFirstComponentOf() const {

        for (auto& actor : registeredActors) {
            
            if (actor->getFirstComponentOf<T>()) {
                
                return actor->getFirstComponentOf<T>();
                
            }
            
        }

        return nullptr;

    }

    /*!
    * Destroys a Actor from the Scene.
    *
    *  \param[in] object The object to destroy
    *  \return Whether the object was destroyed
    */
    virtual bool destroyActor(Actor* object);

    /*!
     * Gets this Scene's CameraManager.
     *
     *  \return The Scene CameraManager.
     */
    FORCEINLINE CameraManager* getCameraManager() const { return this->cameraManager.raw(); }

    Array<class LightComponent*> lights = Array<class LightComponent*>();
    
    FORCEINLINE physx::PxScene* getPhysicsScene() const { return this->physicsScene; }

protected:
    
    friend class Engine;

    /*!
     *  \brief Renders all renderable components in the scene. This function is called on the render thread.
     */
    virtual void renderComponents() const;

    /*!
     *  \brief Updates every object on the scene.
     *
     *  \param[in] deltaTime The engine delta time
     */
    virtual void update(float deltaTime);

    /*!
     *  \brief Allows a newly created Actor to bind input events.
     *
     *  \param[in] object The target Actor
     */
    virtual void setupInput(Actor* object);

    void updatePhysics(float deltaTime) const;

    void waitForPhysics() const;

    /*!
     *  \brief An automatically managed array storing references to all Actors in the scene.
     *  The Actors are automatically released when the scene is destroyed.
     */
    Array<SharedPointer<Actor>> registeredActors = Array<SharedPointer<Actor>>(50);

    Array<Actor*> pendingDestroy = Array<Actor*>(50);

    /*!
     * The scene's CameraManager.
     */
    SharedPointer<CameraManager> cameraManager = nullptr;

    /*!
     * The scene's state.
     */
    State state = State::Unloaded;

    physx::PxScene* physicsScene;
    
};
