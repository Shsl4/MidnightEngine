#pragma once

#include <Object.h>
#include <fmt/format.h>
#include <EngineTypes.h>
#include <Math/Transform.h>
#include <Memory/Array.h>
#include <type_traits>

class SceneComponent : public Object {
    
    typedef Object Super;
    friend class Scene;
    
public:
    
    virtual void start();
    virtual void update(float deltaTime);
    virtual void construct(Transform transform);
    
    bool attachTo(class SceneObject* object);
    bool attachTo(SceneComponent* other);
    bool detachFromComponent();
    
    FORCEINLINE Vector3 getRelativePosition() { return this->transform.position; }
    FORCEINLINE Vector3 getRelativeRotation() { return this->transform.rotation; }
    FORCEINLINE Vector3 getRelativeScale() { return this->transform.scale; }

    FORCEINLINE Vector3 getWorldPosition() { return this->transform.position; }
    FORCEINLINE Vector3 getWorldRotation() { return this->transform.rotation; }
    FORCEINLINE Vector3 getWorldScale() { return this->transform.scale; }

    FORCEINLINE Transform getRelativeTransform() { return this->transform; }
    FORCEINLINE Transform getWorldTransform() { return this->transform; }

    FORCEINLINE class SceneObject* getParentObject() { return this->parentObject; }
    FORCEINLINE SceneComponent* getParentComponent() { return this->parentComponent; }

    FORCEINLINE Array<SceneComponent> getChildComponents() { return this->childComponents; }

    FORCEINLINE bool isRootComponent() { return !parentComponent; }


protected:
    
    SceneComponent() : registered(false), parentComponent(nullptr), parentObject(nullptr) {
     
    }
    
protected:
    
    Transform transform;

private:
    
    class SceneObject* parentObject;
    SceneComponent* parentComponent;
    Array<SceneComponent> childComponents;
    
    bool registered;
    std::string name;
    
};