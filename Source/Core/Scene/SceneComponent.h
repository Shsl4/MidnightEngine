#pragma once

#include <type_traits>

#include <fmt/format.h>

#include <Core/Object.h>
#include <Core/EngineTypes.h>
#include <Memory/Array.h>
#include <Memory/String.h>
#include <Math/Transform.h>

/*!
 * SceneComponents are the lowest class representing something in a scene.
 * SceneComponents can be attached together. Each component can have multiple children, but a unique parent.
 * The root component should then be attached to a parent SceneObject.
 */
class SceneComponent : public Object {

    /*!
     *  Defining Super class
     */
    using Super = Object;

    friend class Scene;

public:

    /*!
     * Start is called when the scene has finished loading and constructing all components.
     * It is called from the parent SceneObject.
     */
    virtual void start();

    /*!
     * Update is called on every engine tick which is NOT tied to rendering thread (and therefore not tied to the frame rate).
     * It is called from the parent SceneObject.
     *
     *  \param[in] deltaTime The engine tick delta time
     */
    virtual void update(float deltaTime);

    /*!
     * Construct is called right after the scene created this component.
     *
     * \param[in] relativeTransform The relative transform of the component.
     */
    virtual void construct(Transform const &relativeTransform);

    /*!
     * Attaches this component to a SceneObject. This component will be automatically detached from its parent if it has any.
     *
     *  \param[in] object The SceneObject to attach to.
     *  \return Whether the operation succeeded.
     */
    bool attachTo(class SceneObject *object);

    /*!
     * Attaches this component to another SceneComponent. This component will be automatically detached from its parent if it has any.
     *
     *  \param[in] other The SceneComponent to attach to.
     *  \return Whether the operation succeeded.
     */
    bool attachTo(SceneComponent *other);

    /*!
     * Detaches this component from its parent if it has any.
     */
    void detachFromComponent();

    /*!
     * Checks whether this component has been correctly registered / created
     *
     *  \return Whether the component is valid
     */
    FORCEINLINE bool isValid() const {
        return registered;
    }

    /*!
     * Gets the component's position relative to the parent component.
     *
     *  \return The component's relative position
     */
    FORCEINLINE Vector3 getRelativePosition() const {
        return Vector3::zero;
    }

    /*!
     * Gets the component's rotation relative to the parent component.
     *
     *  \return The component's relative rotation
     */
    FORCEINLINE Vector3 getRelativeRotation() const {
        return Vector3::zero;
    }

    /*!
     * Gets the current component's scale relative to the parent component.
     *
     *  \return The component's relative scale
     */
    FORCEINLINE Vector3 getRelativeScale() const {
        return Vector3::zero;
    }

    /*!
     * Gets the component's transform relative to the parent component.
     *
     *  \return The component's relative rotation
     */
    FORCEINLINE Transform getRelativeTransform() const {
        return Transform();
    }

    /*!
     * Gets the component's position in world space.
     *
     *  \return The component's world position
     */
    FORCEINLINE Vector3 getWorldPosition() {
        return this->transform.position;
    }

    /*!
     * Gets the component's rotation in world space.
     *
     *  \return The component's world rotation
     */
    FORCEINLINE Vector3 getWorldRotation() {
        return this->transform.rotation;
    }

    /*!
     * Gets the component's scale in world space.
     *
     *  \return The component's world scale
     */
    FORCEINLINE Vector3 getWorldScale() {
        return this->transform.scale;
    }

    /*!
     * Gets the component's transform in world space.
     *
     *  \return The component's world transform
     */
    FORCEINLINE Transform getWorldTransform() const {
        return this->transform;
    }

    /*!
     * Returns the SceneObject this SceneComponent is attached to. (if any)
     *
     * \return The parent object
     */
    FORCEINLINE class SceneObject *getParentObject() {
        return this->parentObject;
    }

    /*!
     * Returns the component this SceneComponent is attached to. (if any)
     *
     * \return The parent component
     */
    FORCEINLINE SceneComponent *getParentComponent() {
        return this->parentComponent;
    }

    /*!
     * Returns the components attached to this SceneComponent.
     *
     * \return The parent component
     */
    FORCEINLINE Array<SceneComponent *> getChildComponents() const {
        return this->childComponents;
    }

    /*!
     * Checks whether this component is not attached to any other.
     *
     *  \return Whether is component is a root component.
     */
    FORCEINLINE bool isRootComponent() const {
        return !parentComponent;
    }


protected:

    /*!
     * The SceneComponent constructor.
     */
    SceneComponent() = default;

    /*!
     * The component world Transform.
     */
    Transform transform;

private:

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
    class SceneObject *parentObject = nullptr;

    /*!
     *  Whether this component was registered by the scene.
     */
    bool registered = false;
    
    /*!
     * The name of the component.
     */
    String name = "";

};
