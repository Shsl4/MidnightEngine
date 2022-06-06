#include "PhysicsModelActor.h"

#include "Console/Console.h"

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
    
    if(!model) {
        Console::getLogger()->error("Tried to set a texture on a PhysicsModelActor without a model");
        return;
    }
        
    model->setTexture(index, name);
}

void PhysicsModelActor::setShader(size_t index, bgfx::ProgramHandle handle) const {

    if(!model) {
        Console::getLogger()->error("Tried to set a shader on a PhysicsModelActor without a model");
        return;
    }
    
    model->setShader(index, handle);
}

void PhysicsModelActor::setMaterial(size_t index, Material const& material) const {

    if(!model) {
        Console::getLogger()->error("Tried to set a material on a PhysicsModelActor without a model");
        return;
    }
    
    model->setMaterial(index, material);
}

Material& PhysicsModelActor::getMaterial(size_t index) const {

    expect(model, "Tried to get a material on a PhysicsModelActor without a model");
    
    return model->getMaterial(index);
}