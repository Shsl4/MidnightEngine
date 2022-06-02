#pragma once

#include <Scene/Actor.h>
#include <Scene/CameraComponent.h>

#include <Memory/WeakPointer.h>

class FlyingCharacter : public Actor {

    using Super = Actor;

public:
    
    FlyingCharacter();
    
    void mouseX(Int32 x);
    void mouseY(Int32 y);

    NODISCARD FORCEINLINE float getMovementSpeed() const { return movementSpeed; }
    
    FORCEINLINE void setMovementSpeed(float speed) { this->movementSpeed = speed; }
    
protected:

    void setupInput(InputManager *manager) override;

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

    CameraComponent* camera = nullptr;

    float movementSpeed = 1.0f;

};
