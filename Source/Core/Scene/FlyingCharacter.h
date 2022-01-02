#pragma once

#include <Scene/SceneObject.h>

class FlyingCharacter : public SceneObject{
    
    using Super = SceneObject;
    
public:
    
    void createComponents(class Scene* scene, Transform transform) override;

protected:
    
    void setupInput(class InputManager* manager) override;
    void update(float deltaTime) override;
    void mouseMotion(int x, int y);
    
#define inputBoilerplate(name) void name##Pressed() {  _##name##Pressed = true; } \
                               void name##Released() {  _##name##Pressed = false; } \
                               bool _##name##Pressed = false;

    inputBoilerplate(w)
    inputBoilerplate(a)
    inputBoilerplate(s)
    inputBoilerplate(d)
    inputBoilerplate(space)
    inputBoilerplate(shift)
    
private:
    
    class CameraComponent* camera;
    
};
