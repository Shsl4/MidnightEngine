#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/SceneComponent.h>
#include <Scene/CameraComponent.h>
#include <Scene/CameraManager.h>
#include <Memory/Array.h>
#include <Logging/Logger.h>

class Scene : public Object {
    
public:
    
    Scene() {
        
        this->logger = std::make_unique<Logger>("Scene");
        this->cameraManager = std::make_unique<CameraManager>();
        logger->info("Constructed scene.");

    }
    
    template<class T, typename ... Args>
    T* createObject(Transform const& defaultTransform, Args&&... args){
        
        static_assert(std::is_base_of_v<SceneObject, T>, "T should inherit from SceneComponent");
        
        T* object = allocator.instantiate<T>(args...);
        object->createComponents(this, defaultTransform);
        
        if(!object->rootComponent) {
            
            allocator.deallocate(object);
            logger->info("Destroying object of class {} as it did not setup a root component at construct time.", object->getClassName());
            return nullptr;
            
        }

        setupInput(object);
        
        registeredObjects.append(object);
        return object;
        
    }
    
    template<class T, typename ... Args>
    T* createComponent(Transform const& relativeTransform, Args&&... args){

        static_assert(std::is_base_of_v<SceneComponent, T>, "T should inherit from SceneComponent");
    
        T* component = allocator.instantiate<T>(args...);

        component->construct(relativeTransform);
        component->registered = true;
        
        registeredComponents.append(component);

        if(component->template instanceOf<CameraComponent>()){
            
            cameraManager->registerCamera(component->template cast<CameraComponent>());
            
        }
        
        return component;
        
    }
    
    bool destroyComponent(SceneComponent* component){
        
        component->detachFromComponent();
        
        if(component->template instanceOf<CameraComponent>()){
            
            cameraManager->unregisterCamera(component->template cast<CameraComponent>());
            
        }
        registeredComponents.remove(component);
        
    }
    
    FORCEINLINE CameraManager* getCameraManager() const { return this->cameraManager.get(); }
    
    
private:

    friend class MEngine;

    void renderComponents() const;
    void updateScene(float deltaTime) const;
    void setupInput(SceneObject* object);
    
    UniquePtr<Logger> logger;
    UniquePtr<CameraManager> cameraManager;
    
    Array<SceneObject*> registeredObjects = Array<SceneObject*>(50);
    Array<SceneComponent*> registeredComponents = Array<SceneComponent*>(50);

    Allocator allocator = Allocator();
    
};
