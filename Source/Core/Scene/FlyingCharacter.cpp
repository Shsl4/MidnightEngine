#include <Scene/FlyingCharacter.h>
#include <Input/InputManager.h>

void FlyingCharacter::mouseX(const Int32 x) {

    // Add camera input on mouse movement.
    camera->addCameraYawInput(static_cast<float>(x) / 10.0f);

}

void FlyingCharacter::mouseY(const Int32 y) {

    // Add camera input on mouse movement.
    camera->addCameraPitchInput(static_cast<float>(y) / 10.0f);

}

void FlyingCharacter::createComponents(Scene *scene, Transform transform) {

    // Create a CameraComponent and set it as root.
    this->camera = scene->createComponent<CameraComponent>(transform, 90.0f, 16.0f / 9.0f, 500.0f);
    setRootComponent(camera);

}

void FlyingCharacter::setupInput(InputManager *manager) {

    // Bind all the required controls.
    manager->bindEvent(this, KeyBind(SDLK_z), EInputEvent::Pressed, &FlyingCharacter::wPressed);
    manager->bindEvent(this, KeyBind(SDLK_s), EInputEvent::Pressed, &FlyingCharacter::sPressed);
    manager->bindEvent(this, KeyBind(SDLK_q), EInputEvent::Pressed, &FlyingCharacter::aPressed);
    manager->bindEvent(this, KeyBind(SDLK_d), EInputEvent::Pressed, &FlyingCharacter::dPressed);
    manager->bindEvent(this, KeyBind(SDLK_LSHIFT), EInputEvent::Pressed, &FlyingCharacter::shiftPressed);
    manager->bindEvent(this, KeyBind(SDLK_SPACE), EInputEvent::Pressed, &FlyingCharacter::spacePressed);

    manager->bindEvent(this, KeyBind(SDLK_z), EInputEvent::Released, &FlyingCharacter::wReleased);
    manager->bindEvent(this, KeyBind(SDLK_s), EInputEvent::Released, &FlyingCharacter::sReleased);
    manager->bindEvent(this, KeyBind(SDLK_q), EInputEvent::Released, &FlyingCharacter::aReleased);
    manager->bindEvent(this, KeyBind(SDLK_d), EInputEvent::Released, &FlyingCharacter::dReleased);
    manager->bindEvent(this, KeyBind(SDLK_LSHIFT), EInputEvent::Released, &FlyingCharacter::shiftReleased);
    manager->bindEvent(this, KeyBind(SDLK_SPACE), EInputEvent::Released, &FlyingCharacter::spaceReleased);

    manager->bindAxis(this, EAxisType::MouseX, &FlyingCharacter::mouseX);
    manager->bindAxis(this, EAxisType::MouseY, &FlyingCharacter::mouseY);

}

void FlyingCharacter::update(float deltaTime) {

    Super::update(deltaTime);

    // Add movement input.
    camera->addMovementInput(camera->getForwardVector(), static_cast<float>(_wPressed - _sPressed), deltaTime);
    camera->addMovementInput(camera->getRightVector(), static_cast<float>(_dPressed - _aPressed), deltaTime);
    camera->addMovementInput(camera->getUpVector(), static_cast<float>(_spacePressed - _shiftPressed), deltaTime);

}



