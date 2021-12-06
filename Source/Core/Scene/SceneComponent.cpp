#include <Scene/Component.h>

void SceneComponent::start(){
    
    for (SceneComponent* childComponent : childComponents) {

        childComponent->start();

    }

}

void SceneComponent::update(float deltaTime){

    for (SceneComponent* childComponent : childComponents) {

        childComponent->update(deltaTime);

    }
    
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
