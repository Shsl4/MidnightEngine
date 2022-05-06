#include <Scene/SceneObject.h>
#include <Input/InputManager.h>

void SceneObject::start() {
    
    // Call start on the root component
    rootComponent->start();
    
}

void SceneObject::update(float deltaTime) {
    
    // Call update on the root component
    rootComponent->update(deltaTime);
    
}

void SceneObject::setRootComponent(SceneComponent *component) {

    this->rootComponent = component;
    component->attachTo(this);

}

void SceneObject::onComponentAttached(SceneComponent *component) {

}

void SceneObject::onComponentDetached(SceneComponent *component) {

}

bool SceneObject::attachTo(SceneObject* object, AttachmentProperties properties)
{
    return this->rootComponent->attachTo(object, properties);
}

void SceneObject::setupInput(InputManager *manager) {

}

void SceneObject::addWorldPosition(Vector3 const& position)
{
    this->rootComponent->addWorldPosition(position);
}

void SceneObject::addWorldRotation(Vector3 const& rotation)
{
    this->rootComponent->addWorldRotation(rotation);
}

void SceneObject::addWorldScale(Vector3 const& scale)
{
    this->rootComponent->addWorldScale(scale);
}

void SceneObject::addWorldTransform(Transform const& transformToAdd)
{
    this->rootComponent->addWorldTransform(transformToAdd);
}

void SceneObject::setWorldPosition(Vector3 const& position)
{
    this->rootComponent->setWorldPosition(position);
}

void SceneObject::setWorldRotation(Vector3 const& rotation)
{
    this->rootComponent->setWorldRotation(rotation);
}

void SceneObject::setWorldScale(Vector3 const& scale)
{
    this->rootComponent->setWorldScale(scale);
}

void SceneObject::setWorldTransform(Transform const& transformToSet)
{
    this->rootComponent->setWorldTransform(transformToSet);
}
