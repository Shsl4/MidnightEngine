#pragma once

#include <Object.h>
#include <Scene/SceneObject.h>
#include <Scene/Component.h>
#include <Memory/Array.h>

class Scene : public Object {
    
public:
    
    Scene(){
        
        
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
    
    ManagedArray<SceneObject> registeredObjects;
    ManagedArray<Component> registeredComponents;
    Allocator allocator;
    
};
