#include <Scene/FlyingCharacter.h>
#include <Input/InputManager.h>

#include <Scene/Scene.h>

#define SHIFT (1<<30)
#define CONVERT(X) ((X) | SHIFT)

enum SpecialKeys {

    LeftShift = CONVERT(225),
    Space = ' ',
    Up = CONVERT(82),
    Down = CONVERT(81)

};

FlyingCharacter::FlyingCharacter() {

    this->camera = createComponent<CameraComponent>("CharacterCamera");
    setRootComponent(camera);
    
}

void FlyingCharacter::mouseX(const Int32 x) {

    // Add camera input on mouse movement.
    camera->addCameraYawInput(static_cast<float>(x) / 10.0f);
    
}

void FlyingCharacter::mouseY(const Int32 y) {

    // Add camera input on mouse movement.
    camera->addCameraPitchInput(static_cast<float>(y) / 10.0f);

}

void FlyingCharacter::setupInput(InputManager *manager) {

    // Bind all the required controls.
    manager->bindEvent(this, KeyBind('w'), EInputEvent::Pressed, &FlyingCharacter::wPressed);
    manager->bindEvent(this, KeyBind('s'), EInputEvent::Pressed, &FlyingCharacter::sPressed);
    manager->bindEvent(this, KeyBind('a'), EInputEvent::Pressed, &FlyingCharacter::aPressed);
    manager->bindEvent(this, KeyBind('d'), EInputEvent::Pressed, &FlyingCharacter::dPressed);
    manager->bindEvent(this, KeyBind(SpecialKeys::LeftShift), EInputEvent::Pressed, &FlyingCharacter::shiftPressed);
    manager->bindEvent(this, KeyBind(SpecialKeys::Space), EInputEvent::Pressed, &FlyingCharacter::spacePressed);
    
    manager->bindEvent(this, KeyBind('w'), EInputEvent::Released, &FlyingCharacter::wReleased);
    manager->bindEvent(this, KeyBind('s'), EInputEvent::Released, &FlyingCharacter::sReleased);
    manager->bindEvent(this, KeyBind('a'), EInputEvent::Released, &FlyingCharacter::aReleased);
    manager->bindEvent(this, KeyBind('d'), EInputEvent::Released, &FlyingCharacter::dReleased);
    manager->bindEvent(this, KeyBind(SpecialKeys::LeftShift), EInputEvent::Released, &FlyingCharacter::shiftReleased);
    manager->bindEvent(this, KeyBind(SpecialKeys::Space), EInputEvent::Released, &FlyingCharacter::spaceReleased);

    manager->bindEvent(this, KeyBind(SpecialKeys::Up), EInputEvent::Pressed, &FlyingCharacter::upPressed);
    manager->bindEvent(this, KeyBind(SpecialKeys::Up), EInputEvent::Released, &FlyingCharacter::upReleased);

    manager->bindEvent(this, KeyBind(SpecialKeys::Down), EInputEvent::Pressed, &FlyingCharacter::downPressed);
    manager->bindEvent(this, KeyBind(SpecialKeys::Down), EInputEvent::Released, &FlyingCharacter::downReleased);

    manager->bindAxis(this, EAxisType::MouseX, &FlyingCharacter::mouseX);
    manager->bindAxis(this, EAxisType::MouseY, &FlyingCharacter::mouseY);

}

void FlyingCharacter::update(float deltaTime) {

    Super::update(deltaTime);

    // Add movement input.
    camera->addMovementInput(camera->getForwardVector(), static_cast<float>(_wPressed - _sPressed) * movementSpeed, deltaTime);
    camera->addMovementInput(camera->getRightVector(), static_cast<float>(_dPressed - _aPressed) * movementSpeed, deltaTime);
    camera->addMovementInput(camera->getUpVector(), static_cast<float>(_spacePressed - _shiftPressed) * movementSpeed, deltaTime);
    camera->setFieldOfView(camera->getFieldOfView() + static_cast<float>(_downPressed - _upPressed) * 50.0f * deltaTime);

}


