#include <Scene/SceneComponent.h>
#include <Scene/SceneObject.h>
#include <Logging/Logger.h>
#include <Math/MathUtils.h>
#include <Memory/String.h>

#include "Engine.h"

Logger logger = Logger("SceneComponent");

const AttachmentProperties AttachmentProperties::all = { true, true, true };
const AttachmentProperties AttachmentProperties::locationOnly = { true, false, false };
const AttachmentProperties AttachmentProperties::rotationOnly = { false, true, false };
const AttachmentProperties AttachmentProperties::scaleOnly = { false, false, true };
const AttachmentProperties AttachmentProperties::noLocation = { false, true, true };
const AttachmentProperties AttachmentProperties::noRotation = { true, false, true };
const AttachmentProperties AttachmentProperties::noScale = { true, true, false };

SceneComponent::SceneComponent()
{
}

void SceneComponent::start() {

    Logger::check(registered, "Object of class \"{}\" has been badly constructed! File: {}, Line: {}", this->getClassName());

    // Call start on every child component
    for (SceneComponent *childComponent: childComponents) {

        childComponent->start();

    }

}

void SceneComponent::update(const float deltaTime) {

    // Call update on every child component
    for (SceneComponent *childComponent: childComponents) {

        childComponent->update(deltaTime);

    }

}

void SceneComponent::construct(Transform const &relativeTransform) {
    
    this->transform = relativeTransform;
    
}

bool SceneComponent::attachTo(SceneObject *object, AttachmentProperties properties) {

    // If the pointer or the object is invalid, return.
    if (!object || !object->isValid()) { return false; }

    // Get the attachment result.
    const bool result = attachTo(object->getRootComponent(), properties);

    // If the attachment succeeded
    if (result) {

        // Set the parent object and call the attached callback.
        this->parentObject = object;
        object->onComponentAttached(this);

    }

    return result;
    
}

bool SceneComponent::attachTo(SceneComponent *other, AttachmentProperties properties) {

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

    this->attachmentProperties = properties;

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

Vector3 SceneComponent::getRelativePosition() const
{
    if(parentComponent)
    {
        return transform.position - parentComponent->transform.position;
    }
    
    return transform.position;
}

Vector3 SceneComponent::getRelativeRotation() const
{
    if(parentComponent)
    {
        return transform.rotation - parentComponent->transform.rotation;
    }
    
    return transform.rotation;
}

Vector3 SceneComponent::getRelativeScale() const
{
    if (parentComponent)
    {
        return transform.scale / parentComponent->transform.scale;
    }
    
    return transform.scale;
}

Vector3 SceneComponent::getForwardVector() const
{
    return Vector3::normalize(Matrix4::fromRotation(transform.rotation) * Vector3(1.0f, 0.0f, 0.0f)); 
}

Vector3 SceneComponent::getRightVector() const
{
    return Vector3::normalize(Matrix4::fromRotation(transform.rotation) * Vector3(0.0f, 0.0f, 1.0f));
}

Vector3 SceneComponent::getUpVector() const
{
    return Vector3::normalize(Matrix4::fromRotation(transform.rotation) * Vector3(0.0f, 1.0f, 0.0f));
}

void SceneComponent::addWorldPosition(Vector3 const& position) {

    this->transform.position += position;

    for (const auto &child : childComponents) {
        if (child->attachmentProperties.followsPosition()) {
            child->addWorldPosition(position);
        }
    }
    
}

void SceneComponent::addWorldRotation(Vector3 const& rotation) {
    
    this->transform.rotation += rotation;

    for (const auto &child : childComponents) {

        if (child->attachmentProperties.followsPosition()) {

            const auto pos = getWorldPosition();
            const auto childPos = child->getWorldPosition();
            const auto f = getForwardVector();
            const float distance = Vector3::distance(pos, childPos);

            child->setWorldPosition((f * distance) + pos);

        }
        
        if (child->attachmentProperties.followsRotation()) {
            child->addWorldRotation(rotation);
        }

    }
    
}

void SceneComponent::addWorldScale(Vector3 const& scale) {
    this->transform.scale += scale;

    for (const auto &child : childComponents) {
        if (child->attachmentProperties.followsScale()) {
            child->addWorldScale(scale);
        }
    }
    
}

void SceneComponent::addWorldTransform(Transform const& transformToAdd) {
    addWorldPosition(transformToAdd.position);
    addWorldRotation(transformToAdd.rotation);
    addWorldScale(transformToAdd.scale);
}

void SceneComponent::setWorldPosition(Vector3 const& position) {

    for (const auto& child : childComponents) {
        if (child->attachmentProperties.followsPosition()) {
            child->setWorldPosition(child->getWorldPosition() - transform.position + position);
        }
    }

    this->transform.position = position;

}

void SceneComponent::setWorldRotation(Vector3 const& rotation) {

    this->transform.rotation = rotation;

    for (const auto& child : childComponents) {

        if (child->attachmentProperties.followsPosition()) {

            const auto pos = getWorldPosition();
            const auto childPos = child->getWorldPosition();
            const auto f = getForwardVector();
            const float distance = Vector3::distance(pos, childPos);

            child->setWorldPosition((f * distance) + pos);

        }

        if (child->attachmentProperties.followsRotation()) {
            child->setWorldRotation(rotation);
        }

    }

}

void SceneComponent::setWorldScale(Vector3 const& scale) {

    Vector3 ratio = scale / transform.scale;

    this->transform.scale = scale;

    for (const auto& child : childComponents) {

        if (child->attachmentProperties.followsScale()) {
            child->setWorldScale(child->getWorldScale() * ratio);
        }

    }

}

void SceneComponent::setWorldTransform(Transform const& transformToSet) {
    setWorldPosition(transformToSet.position);
    setWorldRotation(transformToSet.rotation);
    setWorldScale(transformToSet.scale);
}
