#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/SceneComponent.h>
#include <Memory/Array.h>
#include <Logging/Logger.h>

class Scene : public Object {
    
public:
    
    Scene() {
        
        this->logger = std::make_unique<Logger>("Scene");
        logger->info("Constructed scene.");

    }
    
    template<class T, typename ... Args>
    T* createComponent(Transform const & relativeTransform, Args&&... args){

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

    void renderComponents() const;
    void updateScene(float deltaTime) const;

    UniquePtr<Logger> logger;

    ManagedArray<SceneObject> registeredObjects = ManagedArray<SceneObject>(50);
    ManagedArray<SceneComponent> registeredComponents = ManagedArray<SceneComponent>(50);

    Allocator allocator = Allocator();
    
};
