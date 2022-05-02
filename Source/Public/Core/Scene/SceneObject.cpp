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

void SceneObject::setupInput(InputManager *manager) {

}
