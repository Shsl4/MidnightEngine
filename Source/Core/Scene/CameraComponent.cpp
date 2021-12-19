#include <Scene/CameraComponent.h>
#include <Engine.h>

CameraComponent::CameraComponent(const float fieldOfView, const float aspectRatio, const float renderDistance)
	: fieldOfView(fieldOfView), aspectRatio(aspectRatio), renderDistance(renderDistance)
{

}

CameraComponent::CameraComponent() : CameraComponent(90.0f, 16.0f / 9.0f, 150.0f)
{

}

void CameraComponent::construct(Transform const & relativeTransform)
{

	Super::construct(relativeTransform);
	upVector = Vector3::up;
	updateMatrices();

}

void CameraComponent::start()
{
}

void CameraComponent::update(float deltaTime)
{
}

void CameraComponent::setFieldOfView(const float newFov)
{
	fieldOfView = newFov;
	updateProjectionMatrix();
}

void CameraComponent::setAspectRatio(const float newRatio)
{
	aspectRatio = newRatio;
	updateProjectionMatrix();
}

void CameraComponent::setRenderDistance(const float newDistance)
{
	renderDistance = newDistance;
	updateProjectionMatrix();
}

void CameraComponent::addCameraYawInput(const float yaw)
{
	transform.rotation.x -= yaw;
	updateViewMatrix();
}

void CameraComponent::addCameraPitchInput(const float pitch)
{
	transform.rotation.y = Math::clamp(this->transform.rotation.y - pitch, -89.0f, 89.0f);
	updateViewMatrix();
}

void CameraComponent::addMovementInput(const Vector3 direction, const float scale, const float deltaTime)
{
	const Vector3 newVector = direction * scale * speed * deltaTime;
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
	const float pitchRad = Math::toRadians(transform.rotation.y);
	const float yawRad = Math::toRadians(transform.rotation.x);

	const float cy = cos(yawRad);
	const float cp = cos(pitchRad);
	const float sy = sin(yawRad);
	const float sp = sin(pitchRad);

	const auto direction = Vector3(cy * cp, sp, sy * cp);

	forwardVector = Vector3::normalize(direction);
	rightVector = Vector3::normalize(Vector3::cross(Vector3::up, forwardVector));

	viewMatrix = Matrix4::lookAt(getWorldPosition(), getWorldPosition() + forwardVector, upVector);

}

void CameraComponent::updateProjectionMatrix()
{
	projectionMatrix = Matrix4::perspective(fieldOfView, aspectRatio, 0.01f, renderDistance);
}