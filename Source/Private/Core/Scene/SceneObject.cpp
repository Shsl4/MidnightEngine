#include <Scene/SceneObject.h>
#include <Input/InputManager.h>

#include "Scene/Scene.h"

void SceneObject::start() {
    
    // Call start on the root component
    rootComponent->start();
    
}

void SceneObject::update(float deltaTime) {
    
    // Call update on the root component
    rootComponent->update(deltaTime);
    
}

void SceneObject::setRootComponent(SceneComponent *component) {

    component->attachTo(this);

}

void SceneObject::onComponentAttached(SceneComponent *component) {

}

void SceneObject::onComponentDetached(SceneComponent *component) {
    
}

bool SceneObject::attachTo(SceneObject* object, AttachmentProperties properties) const
{
    return this->rootComponent->attachTo(object, properties);
}

void SceneObject::setupInput(InputManager *manager) {

}

void SceneObject::addWorldPosition(Vector3 const& position) const
{
    this->rootComponent->addWorldPosition(position);
}

void SceneObject::addWorldRotation(Vector3 const& rotation) const
{
    this->rootComponent->addWorldRotation(rotation);
}

void SceneObject::addWorldScale(Vector3 const& scale) const
{
    this->rootComponent->addWorldScale(scale);
}

void SceneObject::addWorldTransform(Transform const& transformToAdd) const
{
    this->rootComponent->addWorldTransform(transformToAdd);
}

void SceneObject::setWorldPosition(Vector3 const& position) const
{
    this->rootComponent->setWorldPosition(position);
}

void SceneObject::setWorldRotation(Vector3 const& rotation) const
{
    this->rootComponent->setWorldRotation(rotation);
}

void SceneObject::setWorldScale(Vector3 const& scale) const
{
    this->rootComponent->setWorldScale(scale);
}

void SceneObject::setWorldTransform(Transform const& transformToSet) const
{
    this->rootComponent->setWorldTransform(transformToSet);
}
