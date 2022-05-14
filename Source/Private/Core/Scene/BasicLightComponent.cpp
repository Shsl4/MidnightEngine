#include <Scene/BasicLightComponent.h>

#include "Engine.h"

void BasicLightComponent::update(float deltaTime) {

    SceneComponent::update(deltaTime);

    const float time = Engine::getInstance()->getTime();
    
    this->diffuseColor.setRed(abs(sin(time)));
    this->diffuseColor.setGreen(abs(cos(time / 2.0f)));
    this->diffuseColor.setBlue(abs(sin(time / 3.0f)));
    
}
