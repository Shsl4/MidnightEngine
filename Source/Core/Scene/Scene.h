#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/Component.h>
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

        static_assert(std::is_base_of<Component, T>::value, "T should inherit from Component");
        static_assert(!std::is_same<T, Component>::value, "T should not directly be Component");
    
        T* component = allocator.instantiate<T>(args...);
        
        registeredComponents.append(component);
        
        return component;
        
    }
    
    bool destroyComponent(Component* component){
        
        registeredComponents.remove(component);
        
    }
    
    
private:

    friend class MEngine;

    void renderComponents();
    void updateScene(float deltaTime);

    UniquePtr<Logger> logger;

    ManagedArray<SceneObject> registeredObjects;
    ManagedArray<Component> registeredComponents;
    Allocator allocator;
    
};
