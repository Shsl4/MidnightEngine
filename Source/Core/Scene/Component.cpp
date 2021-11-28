#include <Scene/Component.h>

void Component::start(){
    
    for (Component* childComponent : childComponents) {

        childComponent->start();

    }

}

void Component::update(float deltaTime){

    for (Component* childComponent : childComponents) {

        childComponent->update(deltaTime);

    }
    
}

bool Component::attachTo(class SceneObject* object){
    
    return false;

}

bool Component::attachTo(Component* other){
    
    return false;

}

bool Component::detachFromComponent(){
    
    return false;

}
