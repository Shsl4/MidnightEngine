#pragma once

#include <Object.h>
#include <Memory/Array.h>
#include <Scene/CameraComponent.h>

/*!
 * The CameraManager is an object keeping references to every camera in a scene.
 * It allows easy camera access and manipulation.
 */
class CameraManager : public Object {

public:

    CameraManager(class Scene *owner);

    /*!
     * Returns the camera currently used for rendering.
     *
     *  @return The active camera
     */
    FORCEINLINE CameraComponent *getActiveCamera() const {
        return activeCamera;
    }

    /*!
     * Returns the scene this CameraManager belongs to.
     *
     *  @return The owning scene.
     */
    FORCEINLINE const class Scene *getScene() {
        return this->scene;
    }

    /*!
     * Function called by the scene when a new CameraComponent is created. Do not call manually.
     *
     * @param[in] camera The camera to register
     */
    void registerCamera(CameraComponent *camera);

    /*!
     * Function called by the scene when a CameraComponent is destroyed. Do not call manually.
     *
     *  @param[in] camera The camera to unregister
     */
    void unregisterCamera(CameraComponent *camera);

    /*!
     * Uses the input camera for rendering.
     *
     *  @param[in] camera The camera to use.
     *  @return Whether the camera is being used.
     */
    bool setActiveCamera(CameraComponent *camera);


protected:

    Array<CameraComponent *> cameras;
    Allocator allocator;
    CameraComponent *activeCamera = nullptr;
    const class Scene *scene;


};

