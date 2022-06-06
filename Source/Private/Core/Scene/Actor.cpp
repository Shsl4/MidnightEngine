#include <Scene/Actor.h>
#include <Input/InputManager.h>

#include <Scene/Scene.h>
#include <Core/Engine.h>

void Actor::start() {

    // Call start on the root component
    rootComponent->start();

}

void Actor::update(float deltaTime) {

    // Call update on the root component
    rootComponent->update(deltaTime);

}

void Actor::setRootComponent(SceneComponent* component) {
    
    component->parentActor = this;
    rootComponent = component;

}

void Actor::onComponentAttached(SceneComponent* component) {}

void Actor::onComponentDetached(SceneComponent* component) { }

bool Actor::attachTo(Actor* object, AttachmentProperties properties) const {
    return this->rootComponent->attachTo(object, properties);
}

Scene* Actor::getScene() const {
    return Engine::getInstance()->getActiveScene().valid() ? Engine::getInstance()->getActiveScene().raw() : nullptr;
}

void Actor::registerManagers(Component* component) {
    
    if (component->inherits<CameraComponent>()) {
        getScene()->getCameraManager()->registerCamera(component->cast<CameraComponent>());
    }
    
    if (component->inherits<LightComponent>()) {
        getScene()->lights += component->cast<LightComponent>();
    }
    
}

void Actor::setupInput(InputManager* manager) {}

void Actor::addWorldPosition(Vector3 const& position) const { this->rootComponent->addWorldPosition(position); }

void Actor::addWorldRotation(Vector3 const& rotation) const { this->rootComponent->addWorldRotation(rotation); }

void Actor::addWorldScale(Vector3 const& scale) const { this->rootComponent->addWorldScale(scale); }

void Actor::addWorldTransform(Transform const& transformToAdd) const {
    this->rootComponent->addWorldTransform(transformToAdd);
}

void Actor::setWorldPosition(Vector3 const& position) const { this->rootComponent->setWorldPosition(position); }

void Actor::setWorldRotation(Vector3 const& rotation) const { this->rootComponent->setWorldRotation(rotation); }

void Actor::setWorldScale(Vector3 const& scale) const { this->rootComponent->setWorldScale(scale); }

void Actor::setWorldTransform(Transform const& transformToSet) const {
    this->rootComponent->setWorldTransform(transformToSet);
}
