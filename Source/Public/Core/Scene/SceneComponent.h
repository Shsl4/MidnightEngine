#pragma once

#include <Core/Object.h>
#include <Memory/Array.h>
#include <Memory/String.h>
#include <Math/Transform.h>

#include <Logging/Logger.h>
#include <Memory/UniquePointer.h>

struct ENGINE_API AttachmentProperties {

public:

    static const AttachmentProperties all;
    static const AttachmentProperties locationOnly;
    static const AttachmentProperties rotationOnly;
    static const AttachmentProperties scaleOnly;
    static const AttachmentProperties noLocation;
    static const AttachmentProperties noRotation;
    static const AttachmentProperties noScale;

    FORCEINLINE bool followsPosition() const {
        return this->followPosition;
    }

    FORCEINLINE bool followsRotation() const {
        return this->followRotation;
    }

    FORCEINLINE bool followsScale() const {
        return this->followScale;
    }

    AttachmentProperties() = default;

private:

    AttachmentProperties(bool position, bool rotation, bool scale) : followPosition(position),
        followRotation(rotation), followScale(scale) {

    }

    bool operator==(AttachmentProperties const& other) const noexcept {

        return (followPosition == other.followPosition) &&
            (followRotation == other.followRotation) && 
            (followScale == other.followScale);

    }

    bool followPosition = true;
    bool followRotation = true;
    bool followScale = false;

};

/*!
 *  \brief SceneComponents are the lowest class representing something in a scene.
 *  SceneComponents can be attached together. Each component can have multiple children, but a unique parent.
 *  The root component should then be attached to a parent SceneObject.
 */
class ENGINE_API SceneComponent : public Object {

    /*!
     *  Defining Super class
     */
    using Super = Object;

    friend class Scene;

public:

    /*!
     *  \brief Start is called when the scene has finished loading and constructing all components.
     *  It is called from the parent SceneObject.
     */
    virtual void start();

    /*!
     *  \brief Update is called on every engine tick which is NOT tied to rendering thread (and therefore not tied to the frame rate).
     *  It is called from the parent SceneObject.
     *
     *  \param[in] deltaTime The engine tick delta time
     */
    virtual void update(float deltaTime);

    /*!
     * \brief Construct is called right after the scene created this component.
     *
     * \param[in] relativeTransform The relative transform of the component.
     */
    virtual void construct(Transform const &relativeTransform);

    /*!
     *  \brief Attaches this component to a SceneObject. This component will be automatically detached from its parent if it has any.
     *
     *  \param[in] object The SceneObject to attach to.
     *  \return Whether the operation succeeded.
     */
    bool attachTo(class SceneObject *object, AttachmentProperties properties = AttachmentProperties::noScale);

    /*!
     *  \brief Attaches this component to another SceneComponent. This component will be automatically detached from its parent if it has any.
     *
     *  \param[in] other The SceneComponent to attach to.
     *  \return Whether the operation succeeded.
     */
    bool attachTo(SceneComponent *other, AttachmentProperties properties = AttachmentProperties::noScale);

    /*!
     *  \brief Detaches this component from its parent if it has any.
     */
    void detachFromComponent();
 
    /*!
     *  \brief Gets the component's position relative to the parent component.
     *
     *  \return The component's relative position
     */
    NODISCARD Vector3 getRelativePosition() const;

    /*!
     *  \brief Gets the component's rotation relative to the parent component.
     *
     *  \return The component's relative rotation
     */
    NODISCARD Vector3 getRelativeRotation() const;

    /*!
     *  \brief Gets the current component's scale relative to the parent component.
     *
     *  \return The component's relative scale
     */
    NODISCARD Vector3 getRelativeScale() const;

    /*!
     *  \brief Gets the component's transform relative to the parent component.
     *
     *  \return The component's relative rotation
     */
    NODISCARD Transform getRelativeTransform() const {
        return { getRelativePosition(), getRelativeRotation(), getRelativeScale() };
    }

    NODISCARD virtual Vector3 getForwardVector() const;

    NODISCARD virtual Vector3 getRightVector() const;

    NODISCARD virtual Vector3 getUpVector() const;
 
    void addWorldPosition(Vector3 const& position);

    void addWorldRotation(Vector3 const& rotation);

    void addWorldScale(Vector3 const& scale);

    void addWorldTransform(Transform const& transformToAdd);

    void setWorldPosition(Vector3 const& position);

    void setWorldRotation(Vector3 const& rotation);

    void setWorldScale(Vector3 const& scale);

    void setWorldTransform(Transform const& transformToSet);
 
    /*!
     *  \brief Checks whether this component has been correctly registered / created
     *
     *  \return Whether the component is valid
     */
    FORCEINLINE bool isValid() const {
        return registered;
    }

    /*!
     *  \brief Gets the component's position in world space.
     *
     *  \return The component's world position
     */
    FORCEINLINE Vector3 getWorldPosition() const
    {
        return this->transform.position;
    }

    /*!
     *  \brief Gets the component's rotation in world space.
     *
     *  \return The component's world rotation
     */
    FORCEINLINE Vector3 getWorldRotation() const
    {
        return this->transform.rotation;
    }

    /*!
     *  \brief Gets the component's scale in world space.
     *
     *  \return The component's world scale
     */
    FORCEINLINE Vector3 getWorldScale() const
    {
        return this->transform.scale;
    }

    /*!
     *  \brief Gets the component's transform in world space.
     *
     *  \return The component's world transform
     */
    FORCEINLINE Transform getWorldTransform() const {
        return this->transform;
    }

    /*!
     *  \brief Returns the SceneObject this SceneComponent is attached to. (if any)
     *
     *  \return The parent object
     */
    FORCEINLINE SceneObject *getParentObject() const
    {
        return this->parentObject;
    }

    /*!
     *  \brief Returns the component this SceneComponent is attached to. (if any)
     *
     *  \return The parent component
     */
    FORCEINLINE SceneComponent *getParentComponent() const
    {
        return this->parentComponent;
    }

    /*!
     *  \brief Returns the components attached to this SceneComponent.
     *
     *  \return The parent component
     */
    FORCEINLINE Array<SceneComponent *> getChildComponents() const {
        return this->childComponents;
    }

    /*!
     *  \brief Checks whether this component is not attached to any other.
     *
     *  \return Whether is component is a root component.
     */
    NODISCARD bool isRootComponent() const;

    FORCEINLINE class Scene *getScene() const {
        return this->scene;
    }

protected:

    /*!
     * The SceneComponent constructor.
     */
    SceneComponent() = default;

private:
 
    /*!
     * The component world Transform.
     */
    Transform transform;
 
    /*!
     * The SceneComponents directly attached to this component.
     */
    Array<SceneComponent *> childComponents;
    
    /*!
     * The SceneComponent this component is attached to.
     */
    SceneComponent *parentComponent = nullptr;
    
    /*!
     * The SceneObject this SceneComponent is attached to.
     */
    SceneObject *parentObject = nullptr;

    /*!
     *  Whether this component was registered by the scene.
     */
    bool registered = false;
    
    /*!
     * The name of the component.
     */
    String name = "";

    AttachmentProperties attachmentProperties;

    Scene* scene = nullptr;
    
};
