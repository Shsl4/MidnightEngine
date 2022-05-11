#pragma once

#include <Object.h>
#include <Memory/Array.h>
#include <Scene/CameraComponent.h>

#include <Memory/WeakPointer.h>

/*!
 * The CameraManager is an object keeping references to every camera in a scene.
 * It allows easy camera access and manipulation.
 */
class ENGINE_API CameraManager : public Object {

public:

    /*!
     *  The CameraManager constructor.
     *
     *  \param[in] owner The scene this CameraManager belongs to.
     */
    explicit CameraManager(Scene *owner);
    
    /*!
     * Remove the default constructor.
     */
    CameraManager() = delete;

    /*!
     * Returns the camera currently used for rendering.
     *
     *  \return The active camera
     */
    FORCEINLINE WeakPointer<CameraComponent> getActiveCamera() const {
        return activeCamera;
    }

    /*!
     * Returns the scene this CameraManager belongs to.
     *
     *  \return The owning scene.
     */
    FORCEINLINE Scene* getScene() const
    {
        return this->scene;
    }

    /*!
     * Function called by the scene when a new CameraComponent is created. Do not call manually.
     *
     * \param[in] camera The camera to register
     */
    void registerCamera(WeakPointer<CameraComponent> camera);

    /*!
     * Function called by the scene when a CameraComponent is destroyed. Do not call manually.
     *
     *  \param[in] camera The camera to unregister
     */
    void unregisterCamera(CameraComponent* camera);
    
    /*!
     * Uses the input camera for rendering.
     *
     *  \param[in] camera The camera to use.
     *  \return Whether the camera is being used.
     */
    bool setActiveCamera(WeakPointer<CameraComponent>& camera);


protected:

    /*!
     * Array containing the cameras in the scene.
     */
    Array<WeakPointer<CameraComponent>> cameras;
    
    /*!
     * The camera currently used for rendering.
     */
    WeakPointer<CameraComponent> activeCamera = nullptr;

    /*!
     * The Scene this CameraManager belongs to.
     */
    Scene* scene;

};

