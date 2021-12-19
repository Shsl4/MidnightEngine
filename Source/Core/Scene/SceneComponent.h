#pragma once

#include <Object.h>
#include <fmt/format.h>
#include <EngineTypes.h>
#include <Math/Transform.h>
#include <Memory/Array.h>
#include <type_traits>

class SceneComponent : public Object {
    
    using Super = Object;
    friend class Scene;
    
public:
    
    virtual void start();
    virtual void update(float deltaTime);
    virtual void construct(Transform const& relativeTransform);
    
    bool attachTo(class SceneObject* object);
    bool attachTo(SceneComponent* other);
    bool detachFromComponent();
    
    FORCEINLINE Vector3 getRelativePosition() const { return this->transform.position; }
    FORCEINLINE Vector3 getRelativeRotation() const { return this->transform.rotation; }
    FORCEINLINE Vector3 getRelativeScale() const { return this->transform.scale; }

    FORCEINLINE Vector3 getWorldPosition() { return this->transform.position; }
    FORCEINLINE Vector3 getWorldRotation() { return this->transform.rotation; }
    FORCEINLINE Vector3 getWorldScale() { return this->transform.scale; }

    FORCEINLINE Transform getRelativeTransform() const { return this->transform; }
    FORCEINLINE Transform getWorldTransform() const { return this->transform; }

    FORCEINLINE class SceneObject* getParentObject() { return this->parentObject; }
    FORCEINLINE SceneComponent* getParentComponent() { return this->parentComponent; }

    FORCEINLINE Array<SceneComponent> getChildComponents() const  { return this->childComponents; }

    FORCEINLINE bool isRootComponent() const { return !parentComponent; }


protected:
    
    SceneComponent() = default;    
    Transform transform;

private:
    
    Array<SceneComponent> childComponents;
    class SceneObject* parentObject = nullptr;
    SceneComponent* parentComponent = nullptr;    
    bool registered = false;
    std::string name = "";
    
};