#include "CameraComponent.h"
#include <Engine.h>
#include <bgfx/bgfx_utils.h>

CameraComponent::CameraComponent(float fieldOfView, float aspectRatio, float renderDistance)
{

	this->fieldOfView = fieldOfView;
	this->aspectRatio = aspectRatio;
	this->renderDistance = renderDistance;
	this->speed = 2.5f;

}

CameraComponent::CameraComponent() : CameraComponent(90.0f, 16.0f / 9.0f, 150.0f)
{

}

void CameraComponent::construct(Transform transform)
{

	Super::construct(transform);
	upVector = Vector3::Up;
	updateMatrices();

}

void CameraComponent::start()
{
}

void CameraComponent::update(float deltaTime)
{
}

void CameraComponent::setFieldOfView(float newFov)
{
	fieldOfView = newFov;
	updateProjectionMatrix();
}

void CameraComponent::setAspectRatio(float newRatio)
{
	aspectRatio = newRatio;
	updateProjectionMatrix();
}

void CameraComponent::setRenderDistance(float newDistance)
{
	renderDistance = newDistance;
	updateProjectionMatrix();
}

void CameraComponent::addCameraYawInput(float yaw)
{
	transform.rotation.x -= yaw;
	updateViewMatrix();
}

void CameraComponent::addCameraPitchInput(float pitch)
{
	transform.rotation.y = Math::clamp(this->transform.rotation.y - pitch, -89.0f, 89.0f);
	updateViewMatrix();
}

void CameraComponent::addMovementInput(Vector3 direction, float scale, float deltaTime)
{
	Vector3 newVector = direction * scale * speed * deltaTime;
	transform.position += newVector;
	updateViewMatrix();

}

void CameraComponent::updateMatrices()
{
	updateViewMatrix();
	updateProjectionMatrix();
}

void CameraComponent::updateViewMatrix()
{

	float pitchRad = Math::toRadians(transform.rotation.y);
	float yawRad = Math::toRadians(transform.rotation.x);

	float _cy = cos(yawRad);
	float _cp = cos(pitchRad);
	float _sy = sin(yawRad);
	float _sp = sin(pitchRad);

	Vector3 direction = Vector3(_cy * _cp, _sp, _sy * _cp);

	forwardVector = Vector3::normalize(direction);
	rightVector = Vector3::normalize(Vector3::cross(Vector3::Up, forwardVector));

	viewMatrix = Matrix4::lookAt(getWorldPosition(), getWorldPosition() + forwardVector, upVector);

}

void CameraComponent::updateProjectionMatrix()
{
	projectionMatrix = Matrix4::perspective(fieldOfView, aspectRatio, 0.01f, renderDistance);
}