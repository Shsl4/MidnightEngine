#pragma once

#include <Math/Transform.h>
#include <Scene/SceneComponent.h>
#include <Object.h>

/*!
 * A SceneObject represents an entity that holds SceneComponents.
 */
class ENGINE_API SceneObject : public Object {

public:

    /*!
     * The SceneObject constructor. It takes no parameters by default.
     */
    SceneObject() = default;

    /*!
     * Start is called when the scene has finished loading and constructing all components and objects.
     * It is called directly from the scene.
     */
    virtual void start();
    
    /*!
     * Update is called on every engine tick which is NOT tied to rendering thread (and therefore not tied to the frame rate).
     * It is called directly from the scene.
     *
     *  \param[in] deltaTime The engine tick delta time
     */
    virtual void update(float deltaTime);

    /*!
     *  Function called when a SceneComponent is attached on this SceneObject.
     *
     *  \param[in] component The component newly attached
     */
    virtual void onComponentAttached(class SceneComponent *component);

    /*!
     *  Function called when a SceneComponent is detached from this SceneObject.
     *
     *  \param[in] component The component newly detached
     */
    virtual void onComponentDetached(class SceneComponent *component);

    /*!
    *  \brief Attaches this component to a SceneObject. This component will be automatically detached from its parent if it has any.
    *
    *  \param[in] object The SceneObject to attach to.
    *  \return Whether the operation succeeded.
    */
    bool attachTo(SceneObject* object, AttachmentProperties properties = AttachmentProperties::noScale) const;

    /*!
     * Function called by the scene when the object is created. SceneObjects must override this method to
     * create their SceneComponents.
     *
     * \param[in] scene The scene this object was created in.
     * \param[in] transform The SceneObject world transform.
     */
    virtual void createComponents(class Scene *scene, Transform transform) = 0;

    void addWorldPosition(Vector3 const& position) const;

    void addWorldRotation(Vector3 const& rotation) const;

    void addWorldScale(Vector3 const& scale) const;

    void addWorldTransform(Transform const& transformToAdd) const;

    void setWorldPosition(Vector3 const& position) const;

    void setWorldRotation(Vector3 const& rotation) const;

    void setWorldScale(Vector3 const& scale) const;

    void setWorldTransform(Transform const& transformToSet) const;


    /*!
    *  \brief Gets the component's position relative to the parent component.
    *
    *  \return The component's relative position
    */
    FORCEINLINE Vector3 getRelativePosition() const {
        return this->rootComponent->getRelativePosition();
    }

    /*!
     *  \brief Gets the component's rotation relative to the parent component.
     *
     *  \return The component's relative rotation
     */
    FORCEINLINE Vector3 getRelativeRotation() const {
        return this->rootComponent->getRelativeRotation();
    }

    /*!
     *  \brief Gets the current component's scale relative to the parent component.
     *
     *  \return The component's relative scale
     */
    FORCEINLINE Vector3 getRelativeScale() const {
        return this->rootComponent->getRelativeScale();
    }

    /*!
     *  \brief Gets the component's transform relative to the parent component.
     *
     *  \return The component's relative rotation
     */
    FORCEINLINE Transform getRelativeTransform() const {
        return this->rootComponent->getRelativeTransform();
    }

    FORCEINLINE Vector3 getForwardVector() const {
        return this->rootComponent->getForwardVector();
    }

    FORCEINLINE Vector3 getRightVector() const {
        return this->rootComponent->getRightVector();
    }

    FORCEINLINE Vector3 getUpVector() const {
        return this->rootComponent->getUpVector();
    }

    /*!
    *  \brief Gets the component's position in world space.
    *
    *  \return The component's world position
    */
    FORCEINLINE Vector3 getWorldPosition() const
    {
        return this->rootComponent->getWorldPosition();
    }

    /*!
     *  \brief Gets the component's rotation in world space.
     *
     *  \return The component's world rotation
     */
    FORCEINLINE Vector3 getWorldRotation() const
    {
        return this->rootComponent->getWorldRotation();
    }

    /*!
     *  \brief Gets the component's scale in world space.
     *
     *  \return The component's world scale
     */
    FORCEINLINE Vector3 getWorldScale() const
    {
        return this->rootComponent->getWorldScale();
    }

    /*!
     * Returns whether this object is valid.
     *
     * \return Whether this object is valid.
     */
    FORCEINLINE bool isValid() const {
        return true;
    }

    /*!
     * Returns this SceneObject root component
     *
     * \return This object's root component
     */
    FORCEINLINE SceneComponent *getRootComponent() const {
        return this->rootComponent;
    }

    FORCEINLINE class Scene *getScene() const {
        return this->rootComponent->getScene();
    }

protected:

    /*!
     * Called after the SceneObject is created. It allows input binding.
     *
     *  \param[in] manager The engine InputManager
     */
    virtual void setupInput(class InputManager *manager);

    /*!
     * Sets the input component as this object's root component
     *
     *  \param[in] component The new root component
     */
    void setRootComponent(class SceneComponent *component);

private:

    friend class Scene;
    friend class SceneComponent;

    /*!
     * This object's root component
     */
    SceneComponent *rootComponent = nullptr;

};
