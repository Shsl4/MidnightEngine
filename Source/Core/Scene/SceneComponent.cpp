#include <Scene/SceneComponent.h>
#include <Logging/Logger.h>

void SceneComponent::start(){
    
    Logger::check(registered, "Object of class \"{}\" has been badly constructed! File: {}, Line: {}", this->getClassName());

    for (SceneComponent* childComponent : childComponents) {

        childComponent->start();

    }

}

void SceneComponent::update(float deltaTime){

    for (SceneComponent* childComponent : childComponents) {

        childComponent->update(deltaTime);

    }
    
}

void SceneComponent::construct(Transform transform)
{
    this->transform = transform;
}

bool SceneComponent::attachTo(class SceneObject* object){
    
    return false;

}

bool SceneComponent::attachTo(SceneComponent* other){
    
    return false;

}

bool SceneComponent::detachFromComponent(){
    
    return false;

}
