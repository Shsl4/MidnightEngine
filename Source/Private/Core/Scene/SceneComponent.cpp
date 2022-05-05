#include <Scene/SceneComponent.h>
#include <Scene/SceneObject.h>
#include <Logging/Logger.h>
#include <Memory/String.h>

void SceneComponent::start() {

    Logger::check(registered, "Object of class \"{}\" has been badly constructed! File: {}, Line: {}", this->getClassName());

    // Call start on every child component
    for (SceneComponent *childComponent: childComponents) {

        childComponent->start();

    }

}

void SceneComponent::update(float deltaTime) {

    // Call update on every child component
    for (SceneComponent *childComponent: childComponents) {

        childComponent->update(deltaTime);

    }

}

void SceneComponent::construct(Transform const &relativeTransform) {
    
    this->transform = relativeTransform;
    
}

bool SceneComponent::attachTo(SceneObject *object) {

    // If the pointer or the object is invalid, return.
    if (!object || !object->isValid()) { return false; }

    // Get the attachment result.
    bool result = attachTo(object->getRootComponent());

    // If the attachment succeeded
    if (result) {

        // Set the parent object and call the attached callback.
        this->parentObject = object;
        object->onComponentAttached(this);

    }

    return result;


}

bool SceneComponent::attachTo(SceneComponent *other) {

    // If the pointer or the component is invalid, return.
    if (!other || !other->isValid()) { return false; }
    
    // You can't attach a component to itself.
    if (other == this) { return false; }
    
    // If this component is already attached to another component, detach it.
    if (parentComponent) { detachFromComponent(); }

    // Add this to the child components of other.
    other->childComponents.append(this);
    
    // Set the new parent component.
    this->parentComponent = other;

    return true;

}

void SceneComponent::detachFromComponent() {

    // If there is no parent object or component
    if (!parentObject || !parentComponent) {
        return;
    }

    // Detach and set the variables to nullptr.
    parentComponent->childComponents.removeFirstOf(this);
    parentObject->onComponentDetached(this);
    parentComponent = nullptr;
    parentObject = nullptr;

}
