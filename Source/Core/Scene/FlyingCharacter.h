#pragma once

#include <Scene/SceneObject.h>

class ENGINE_API FlyingCharacter : public SceneObject {

    using Super = SceneObject;

public:
    void mouseX(Int32 x);
    void mouseY(Int32 y);
    void createComponents(class Scene *scene, Transform transform) override;

protected:

    void setupInput(class InputManager *manager) override;

    void update(float deltaTime) override;

#define inputBoilerplate(name) void name##Pressed() {  _##name##Pressed = true; } \
                               void name##Released() {  _##name##Pressed = false; } \
                               bool _##name##Pressed = false;

    inputBoilerplate(w)

    inputBoilerplate(a)

    inputBoilerplate(s)

    inputBoilerplate(d)

    inputBoilerplate(space)

    inputBoilerplate(shift)

    inputBoilerplate(up)

    inputBoilerplate(down)


private:

    CameraComponent *camera = nullptr;

};
