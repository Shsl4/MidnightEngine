#include <Scene/SceneComponent.h>
#include <Scene/SceneObject.h>
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

void SceneComponent::construct(Transform const & relativeTransform)
{
    this->transform = relativeTransform;
}

bool SceneComponent::attachTo(SceneObject* object){
    
    if(!object || !object->isValid()) { return false; }
    
    bool result = attachTo(object->getRootComponent());
    
    if(result){
        
        this->parentObject = object;
        object->onComponentAttached(this);
        
    }
    
    return result;
    
        
}

bool SceneComponent::attachTo(SceneComponent* other){
    
    if(parentComponent) { detachFromComponent(); }
    
    if(!other || !other->isValid()) { return false; }
    
    if(other == this) { return true; }
    
    other->childComponents.append(this);
    this->parentComponent = other;
    
    return true;

}

void SceneComponent::detachFromComponent(){
    
    if(!parentObject || !parentComponent) { return; }
    
    parentComponent->childComponents.remove(this);
    parentObject->onComponentDetached(this);
    parentComponent = nullptr;
    parentObject = nullptr;

}
