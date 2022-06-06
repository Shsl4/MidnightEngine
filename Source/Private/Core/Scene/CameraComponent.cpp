#include <Scene/CameraComponent.h>

#include "Scene/Scene.h"

CameraComponent::CameraComponent() {

    updateMatrices();

}

void CameraComponent::start() {
    
    Super::start();
    
}


void CameraComponent::update(float deltaTime) {
    
    Super::update(deltaTime);

    
}

void CameraComponent::setFieldOfView(const float fov) {

    if (fov >= 170.0f){
        fieldOfView = 170.0f;
    }
    else if (fov <= 10.0f){
        fieldOfView = 10.0f;
    }
    else{
        fieldOfView = fov;
    }

    updateProjectionMatrix();
    
}

void CameraComponent::setAspectRatio(const float ratio) {
    
    aspectRatio = ratio;
    updateProjectionMatrix();
    
}

void CameraComponent::setRenderDistance(const float distance) {
    
    renderDistance = distance;
    updateProjectionMatrix();
    
}

void CameraComponent::addCameraYawInput(const float yaw) {
    
    // Clamp the value around -180.0 and 180.0 degrees.
    const auto rotation = getWorldRotation();
    setWorldRotation({ Math::clampAround(rotation.x - yaw, -180.0f, 180.0f), rotation.y, rotation.z });
    updateViewMatrix();
    
}

void CameraComponent::addCameraPitchInput(const float pitch) {
    
    // Clamp the value between -89.0 and 89.0 degrees to prevent the camera from doing complete vertical rotations.
    //transform.rotation.y = Math::clamp(this->transform.rotation.y - pitch, -89.0f, 89.0f);
    const auto rotation = getWorldRotation();
    setWorldRotation({ rotation.x, Math::clamp(rotation.y - pitch, -89.0f, 89.0f), rotation.z });
    updateViewMatrix();
    
}

void CameraComponent::addMovementInput(const Vector3 direction, const float scale, const float deltaTime) {
    
    // Add movement input in the looking direction.
    addWorldPosition(direction * scale * speed * deltaTime);
    updateViewMatrix();
    
}

Vector3 CameraComponent::getRightVector() const {
    return this->rightVector;
}

Vector3 CameraComponent::getUpVector() const {
    return this->upVector;
}

Vector3 CameraComponent::getForwardVector() const {
    return this->forwardVector;
}

void CameraComponent::updateMatrices() {
    
    updateViewMatrix();
    updateProjectionMatrix();
    
}

void CameraComponent::updateViewMatrix() {
    
    // Quick maths 😎
    const auto rotation = getWorldRotation();
    const float pitchRad = Math::toRadians(rotation.y);
    const float yawRad = Math::toRadians(rotation.x);

    const float cy = cos(yawRad);
    const float cp = cos(pitchRad);
    const float sy = sin(yawRad);
    const float sp = sin(pitchRad);

    const auto direction = Vector3(cy * cp, sp, sy * cp);

    forwardVector = Vector3::normalize(direction);
    rightVector = Vector3::normalize(Vector3::cross(Vector3::up, forwardVector));

    viewMatrix = Matrix4::lookAt(getWorldPosition(), getWorldPosition() + forwardVector, upVector);
    
}

void CameraComponent::updateProjectionMatrix() {
    
    // Math magic
    projectionMatrix = Matrix4::perspective(fieldOfView, aspectRatio, 0.01f, renderDistance);
    
}

void CameraComponent::setWorldPosition(const Vector3 &position) {
    SceneComponent::setWorldPosition(position);
    updateViewMatrix();
}

void CameraComponent::setWorldRotation(const Vector3 &rotation) {
    SceneComponent::setWorldRotation(rotation);
    updateViewMatrix();
}
