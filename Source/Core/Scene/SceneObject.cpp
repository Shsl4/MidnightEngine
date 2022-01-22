#include <Scene/SceneObject.h>
#include <Input/InputManager.h>

void SceneObject::start() {
    rootComponent->start();
}

void SceneObject::update(float deltaTime) {
    rootComponent->update(deltaTime);
}

void SceneObject::onComponentAttached(SceneComponent *component) {

    //logger.info("Attached component {} to {}", component->getDescription(), this->getDescription());

}

void SceneObject::setRootComponent(SceneComponent *component) {

    this->rootComponent = component;
    component->attachTo(this);

}

void SceneObject::setupInput(InputManager *manager) {

}

void SceneObject::onComponentDetached(SceneComponent *component) {


}
