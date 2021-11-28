#pragma once

#include <Object.h>
#include <fmt/format.h>
#include <EngineTypes.h>
#include <Math/Transform.h>
#include <Memory/Array.h>
#include <type_traits>

class Component : public Object {
    
    typedef Object Super;
    
public:
    
    virtual void start();
    virtual void tick();
    
    bool attachTo(class SceneObject* object);
    bool attachTo(Component* other);
    bool detachFromComponent();
    
    FORCEINLINE Vector3 getPosition() { return this->transform.position; }
    FORCEINLINE Vector3 getScale() { return this->transform.scale; }
    FORCEINLINE Vector3 getRotation() { return this->transform.rotation; }
    FORCEINLINE Transform getTransform() { return this->transform; }

    FORCEINLINE class SceneObject* getParentObject() { return this->parentObject; }
    FORCEINLINE Component* getParentComponent() { return this->parentComponent; }
    FORCEINLINE ManagedArray<Component> getChildComponents() { return this->childComponents; }
    FORCEINLINE bool isRootComponent() { return parentComponent; }


protected:
    
    Component(){
     
    }
    
private:
    
    Transform transform;
    
    class SceneObject* parentObject;
    
    Component* parentComponent;
    
    ManagedArray<Component> childComponents;
    
    bool registered;
    
    std::string name;
    
};

class MyComponent : public Component{
    
    typedef Component Super;
    
public:
    
    
    MyComponent() : Super(){
        
    }
    
};
