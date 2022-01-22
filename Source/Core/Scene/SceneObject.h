#pragma once

#include <EngineTypes.h>
#include <Math/Vector3.h>
#include <Memory/Array.h>
#include <Logging/Logger.h>
#include <Math/Transform.h>
#include <Scene/Scene.h>
#include <Object.h>

class SceneObject : public Object {

public:

    SceneObject() = default;

    virtual void start();

    virtual void update(float deltaTime);

    virtual void onComponentAttached(class SceneComponent *component);

    virtual void onComponentDetached(class SceneComponent *component);

    virtual void createComponents(class Scene *scene, Transform transform) = 0;

    FORCEINLINE bool isValid() const {
        return true;
    }

    FORCEINLINE class SceneComponent *getRootComponent() const {
        return this->rootComponent;
    }

protected:

    virtual void setupInput(class InputManager *manager);

    void setRootComponent(class SceneComponent *component);

private:

    friend class Scene;

    class SceneComponent *rootComponent;

};
