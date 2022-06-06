#include "PhysicsModelActor.h"

PhysicsModelActor::PhysicsModelActor() {
    
    this->physicsComponent = createComponent<PhysicsComponent>("PhysicsComp");
    this->model = createComponent<ModelComponent>("Model");
    this->setRootComponent(physicsComponent);
    this->model->attachTo(physicsComponent);
    
}

void PhysicsModelActor::setModel(String const& name) const {
    this->model->setModel(name);
}

void PhysicsModelActor::setTexture(size_t index, String const& name) const {
    model->setTexture(index, name);
}

void PhysicsModelActor::setShader(size_t index, bgfx::ProgramHandle handle) const {
    model->setShader(index, handle);
}

void PhysicsModelActor::setMaterial(size_t index, Material const& material) const {
    model->setMaterial(index, material);
}

Material& PhysicsModelActor::getMaterial(size_t index) const {
    return model->getMaterial(index);
}