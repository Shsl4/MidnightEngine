#pragma once

#include <Object.h>
#include <Memory/Array.h>
#include <Scene/CameraComponent.h>

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
    FORCEINLINE CameraComponent *getActiveCamera() const {
        return activeCamera;
    }

    /*!
     * Returns the scene this CameraManager belongs to.
     *
     *  \return The owning scene.
     */
    FORCEINLINE const Scene *getScene() const
    {
        return this->scene;
    }

    /*!
     * Function called by the scene when a new CameraComponent is created. Do not call manually.
     *
     * \param[in] camera The camera to register
     */
    void registerCamera(CameraComponent *camera);

    /*!
     * Function called by the scene when a CameraComponent is destroyed. Do not call manually.
     *
     *  \param[in] camera The camera to unregister
     */
    void unregisterCamera(CameraComponent *camera);
    
    /*!
     * Uses the input camera for rendering.
     *
     *  \param[in] camera The camera to use.
     *  \return Whether the camera is being used.
     */
    bool setActiveCamera(CameraComponent *camera);


protected:

    /*!
     * Array containing the cameras in the scene.
     */
    Array<CameraComponent *> cameras;
    
    /*!
     * The camera currently used for rendering.
     */
    CameraComponent *activeCamera = nullptr;

    /*!
     * The Scene this CameraManager belongs to.
     */
    Scene *scene;

};

