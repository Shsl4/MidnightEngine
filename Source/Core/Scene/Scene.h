#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/SceneComponent.h>
#include <Scene/CameraComponent.h>
#include <Scene/CameraManager.h>
#include <Memory/Array.h>
#include <Memory/UniquePtr.h>
#include <Logging/Logger.h>

/*!
 * A scene is an object that represents the world. It holds SceneComponents and SceneObjects, and manages their lifetimes.
 */
class ENGINE_API Scene : public Object {

public:

    /*!
     * The scene constructor. Initializes the private variables.
     */
    Scene();
    
    /*!
     *  Instantiates a new SceneObject in the scene
     *
     *  \tparam T The SceneObject class to instantiate
     *  \tparam Args A variadic template type
     *  \param[in] transform The world Transform of the SceneObject
     *  \param[in] args The arguments to pass to the SceneObject's constructor
     *  \return The newly created SceneObject
     *
     */
    template<class T, typename ... Args>
    T *createObject(Transform const &transform, Args &&... args) {
        
        Allocator<T> allocator;

        // Check if the class we are trying to instantiate is a SceneObject.
        static_assert(std::is_base_of_v<SceneObject, T>, "T should inherit from SceneComponent");

        // Instantiate the SceneObject
        T *object = allocator.construct(args...);
        
        // Create the components
        object->createComponents(this, transform);

        // If the SceneObject did not setup a root component, destroy it.
        if (!object->rootComponent) {

            allocator.destroy(object);
            
            logger->debug("Destroying object of class {} as it did not setup a root component at construct time.", object->getClassName());
            
            return nullptr;

        }

        // Setup the input on the created SceneObject
        setupInput(object);

        // Register the SceneObject
        registeredObjects.append(object);
        return object;

    }

    /*!
     *  Instantiates a new SceneComponent in the scene
     *
     *  \tparam T The SceneComponent class to instantiate
     *  \tparam Args A variadic template type
     *  \param[in] relativeTransform The relative Transform of the SceneComponent
     *  \param[in] args The arguments to pass to the SceneComponent's constructor
     *  \return The newly created SceneComponent
     *
     */
    template<class T, typename ... Args>
    T *createComponent(Transform const &relativeTransform, Args &&... args) {

        // Check if the class we are trying to instantiate is a SceneComponent.
        static_assert(std::is_base_of_v<SceneComponent, T>, "T should inherit from SceneComponent");
        
        Allocator<T> allocator;

        // Instantiate the SceneComponent
        T *component = allocator.construct(args...);
        component->scene = this;
        component->construct(relativeTransform);
        
        // Mark it as registered (bad, will probably be changed later)
        component->registered = true;

        // If the created component is a CameraComponent, it is passed to the camera manager.
        if (component->template inherits<CameraComponent>()) {

            cameraManager->registerCamera(component->template cast<CameraComponent>());

        }

        // Register the SceneComponent
        registeredComponents.append(component);
        return component;

    }

    /*!
     * Destroys a SceneComponent from the Scene.
     *
     *  \param[in] component The component to destroy
     *  \return Whether the component was destroyed
     */
    bool destroyComponent(SceneComponent *component);

    /*!
     * Gets this Scene's CameraManager.
     *
     *  \return The Scene CameraManager.
     */
    FORCEINLINE CameraManager *getCameraManager() const {
        return this->cameraManager.get();
    }

protected:

    friend class Engine;

    /*!
     * Renders all renderable components in the scene. This function is called on the render thread.
     */
    virtual void renderComponents() const;

    /*!
     *  Updates every object on the scene.
     *
     *  \param[in] deltaTime The engine delta time
     */
    virtual void updateScene(float deltaTime) const;

    /*!
     * Allows a newly created SceneObject to bind input events.
     *
     *  \param[in] object The target SceneObject
     */
    virtual void setupInput(SceneObject *object);

    /*!
     * An automatically managed array storing references to all SceneObjects in the scene.
     * The SceneObjects are automatically released when the scene is destroyed.
     */
    AutoReleaseArray<SceneObject *> registeredObjects = AutoReleaseArray<SceneObject *>(50);

    /*!
     * An automatically managed array storing references to all SceneComponents in the scene.
     * The SceneComponents are automatically released when the scene is destroyed.
     */
    AutoReleaseArray<SceneComponent *> registeredComponents = AutoReleaseArray<SceneComponent *>(50);

    /*!
     * The scene's CameraManager.
     */
    UniquePtr<CameraManager> cameraManager;

    /*!
     * The scene's Logger.
     */
    UniquePtr<Logger> logger;

};
