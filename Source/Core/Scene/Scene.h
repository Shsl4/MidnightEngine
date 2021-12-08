#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/SceneComponent.h>
#include <Memory/Array.h>
#include <Logging/Logger.h>

class Scene : public Object {
    
public:
    
    Scene() : registeredComponents(15), registeredObjects(15) {
        
        this->logger = std::make_unique<Logger>("Scene");
        logger->info("Constructed scene.");

    }
    
    template<class T, typename ... Args>
    T* createComponent(Transform relativeTransform, Args&&... args){

        static_assert(std::is_base_of<SceneComponent, T>::value, "T should inherit from SceneComponent");
    
        T* component = allocator.instantiate<T>(args...);

        component->construct(relativeTransform);
        component->registered = true;
        
        registeredComponents.append(component);
        
        return component;
        
    }
    
    bool destroyComponent(SceneComponent* component){
        
        registeredComponents.remove(component);
        
    }
    
    
private:

    friend class MEngine;

    void renderComponents();
    void updateScene(float deltaTime);

    UniquePtr<Logger> logger;

    ManagedArray<SceneObject> registeredObjects;
    ManagedArray<SceneComponent> registeredComponents;

    Allocator allocator;
    
};
