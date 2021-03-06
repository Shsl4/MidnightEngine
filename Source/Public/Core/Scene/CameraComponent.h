#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>
#include <Math/Matrix4.h>

class ENGINE_API CameraComponent : public SceneComponent {

private:

    using Super = SceneComponent;

public:

    /*!
     * The default CameraComponent constructor. Initializes the camera with a
     * 90 degrees field of view, 16/9 aspect ratio and 150 unit render distance.
     */
    CameraComponent();

    void start() override;

    void update(float deltaTime) override;
 
    /*!
     * Sets the field of view (in degrees) of the camera.
     *
     *  \param[in] fov The new field of view
     */
    void setFieldOfView(float fov);

    /*!
     * Sets aspect ratio of the camera.
     *
     *  \param[in] ratio The new aspect ratio
     */
    void setAspectRatio(float ratio);

    /*!
     * Sets the render distance of the camera in units
     *
     *  \param[in] distance The maximal render distance
     */
    void setRenderDistance(float distance);

    /*!
     * Adds yaw (horizontal) input to the camera.
     *
     *  \param[in] yaw The rotation to add to the camera (in degrees)
     */
    void addCameraYawInput(float yaw);

    /*!
     * Adds pitch (vertical) input to the camera.
     *
     *  \param[in] pitch The rotation to add to the camera (in degrees)
     */
    void addCameraPitchInput(float pitch);

    /*!
     *  \deprecated
     *  Translates the camera in space. This shouldn't be implemented here and will be removed later.
     *
     *  \param[in] direction The direction to translate to
     *  \param[in] scale The translation power
     *  \param[in] deltaTime The time between the previous tick
     */
    void addMovementInput(Vector3 direction, float scale, float deltaTime);

    /*!
     * Returns the field of view of the camera
     *
     * \return The field of view of the CameraComponent
     */
    FORCEINLINE float getFieldOfView() const {
        return this->fieldOfView;
    }

    /*!
     * Returns the aspect ratio of the camera
     *
     * \return The aspect ratio of the CameraComponent
     */
    FORCEINLINE float getAspectRatio() const {
        return this->aspectRatio;
    }

    /*!
     * Returns the render distance of the camera
     *
     * \return The render distance of the CmaeraComponent
     */
    FORCEINLINE float getRenderDistance() const {
        return this->renderDistance;
    }

    /*!
     * Returns a copy of the camera's view matrix.
     *
     * \return The camera view matrix
     */
    FORCEINLINE Matrix4 getViewMatrix() const {
        return this->viewMatrix;
    }

    /*!
     * Returns a copy of the camera's projection matrix.
     *
     * \return The camera projection matrix
     */
    FORCEINLINE Matrix4 getProjectionMatrix() const {
        return this->projectionMatrix;
    }

    /*!
     * Returns a copy of the camera's right vector.
     *
     * \return The camera right vector
     */
    NODISCARD Vector3 getRightVector() const override;

    /*!
     * Returns a copy of the camera's up vector.
     *
     * \return The camera up vector
     */
    NODISCARD Vector3 getUpVector() const override;

    /*!
     * Returns a copy of the camera's forward vector.
     *
     * \return The camera forward vector
     */
    NODISCARD Vector3 getForwardVector() const override;

    void setWorldPosition(const Vector3 &position) override;

    void setWorldRotation(const Vector3 &rotation) override;

private:

    /*!
     * Recalculates the camera view and projection matrices.
     */
    void updateMatrices();

    /*!
     * Recalculates the camera view matrix.
     */
    void updateViewMatrix();

    /*!
     * Recalculates the camera projection matrix.
     */
    void updateProjectionMatrix();

    /*!
     * The View Matrix of the camera.
     */
    Matrix4 viewMatrix;

    /*!
     * The Projection Matrix of the camera.
     */
    Matrix4 projectionMatrix;

    /*!
     * The camera right vector.
     */
    Vector3 rightVector;

    /*!
     * The camera up vector.
     */
    Vector3 upVector = Vector3::up;

    /*!
     * The camera forward vector.
     */
    Vector3 forwardVector;

    /*!
     * The active field of view of the camera.
     */
    float fieldOfView = 90.0f;

    /*!
     * The active aspect ratio of the camera.
     */
    float aspectRatio = 16.0f / 9.0f;

    /*!
     * The active render distance of the camera.
     */
    float renderDistance = FLT_MAX;

    /*!
     * The active camera speed.
     */
    float speed = 2.5f;

};

