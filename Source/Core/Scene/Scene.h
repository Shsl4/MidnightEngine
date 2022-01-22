#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/SceneComponent.h>
#include <Scene/CameraComponent.h>
#include <Scene/CameraManager.h>
#include <Memory/Array.h>
#include <Logging/Logger.h>

class Scene : public Object {

public:

    Scene() {

        this->logger = std::make_unique<Logger>("Scene");
        this->cameraManager = std::make_unique<CameraManager>(this);
        logger->info("Constructed scene.");

    }

    /*!
     *  Instantiates a new SceneObject in the scene
     *
     *  @tparam T The SceneObject class to instantiate
     *  @tparam Args A variadic arguments template type
     *  @param transform The world Transform of the SceneObject
     *  @param args The arguments to pass to the SceneObject's constructor
     *  @return The newly created SceneObject
     *
     */
    template<class T, typename ... Args>
    T *createObject(Transform const &transform, Args &&... args) {

        static_assert(std::is_base_of_v<SceneObject, T>, "T should inherit from SceneComponent");

        T *object = allocator.instantiate<T>(args...);
        object->createComponents(this, transform);

        if (!object->rootComponent) {

            allocator.deallocate(object);
            logger->info("Destroying object of class {} as it did not setup a root component at construct time.", object->getClassName());
            return nullptr;

        }

        setupInput(object);

        registeredObjects.append(object);
        return object;

    }

    /*!
     *  Instantiates a new SceneComponent in the scene
     *
     *  @tparam T The SceneComponent class to instantiate
     *  @tparam Args A variadic arguments template type
     *  @param relativeTransform The relative Transform of the SceneComponent
     *  @param args The arguments to pass to the SceneComponent's constructor
     *  @return The newly created SceneComponent
     *
     */
    template<class T, typename ... Args>
    T *createComponent(Transform const &relativeTransform, Args &&... args) {

        static_assert(std::is_base_of_v<SceneComponent, T>, "T should inherit from SceneComponent");

        T *component = allocator.instantiate<T>(args...);

        component->construct(relativeTransform);
        component->registered = true;

        registeredComponents.append(component);

        /// If the created component is a CameraComponent, pass it to the camera manager.
        if (component->template instanceOf<CameraComponent>()) {

            cameraManager->registerCamera(component->template cast<CameraComponent>());

        }

        return component;

    }

    /*!
     * Destroys a SceneComponent from the Scene.
     *
     *  @param component The component to destroy
     *  @return Whether the component was destroyed
     */
    bool destroyComponent(SceneComponent *component) {

        if (!component) {return false;}

        component->detachFromComponent();

        if (component->template instanceOf<CameraComponent>()) {

            cameraManager->unregisterCamera(component->template cast<CameraComponent>());

        }

        return registeredComponents.remove(component);

    }

    /*!
     * Gets this Scene's CameraManager.
     *
     *  @return The Scene CameraManager.
     */
    FORCEINLINE CameraManager *getCameraManager() const {
        return this->cameraManager.get();
    }

private:

    friend class MEngine;

    /*!
     * Renders all renderable components in the scene. This function is called on the render thread.
     */
    void renderComponents() const;

    /*!
     *  Updates every object on the scene.
     *
     *  @param deltaTime The engine delta time
     */
    void updateScene(float deltaTime) const;

    /*!
     * Allows a newly created SceneObject to bind input events.
     *
     *  @param object The target SceneObject
     */
    void setupInput(SceneObject *object);

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

    Allocator allocator = Allocator();

};
