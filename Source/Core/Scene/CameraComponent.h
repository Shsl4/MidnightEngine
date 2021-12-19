#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>
#include <Math/Matrix4.h>

class CameraComponent : public SceneComponent
{

private:

	using Super = SceneComponent;

public:

	CameraComponent();
	CameraComponent(float fieldOfView, float aspectRatio, float renderDistance);

	void start() override;
	void update(float deltaTime) override;
	void construct(Transform const & relativeTransform) override;

	void setFieldOfView(float newFov);
	void setAspectRatio(float newRatio);
	void setRenderDistance(float newDistance);

	void addCameraYawInput(float yaw);
	void addCameraPitchInput(float pitch);

	void addMovementInput(Vector3 direction, float scale, float deltaTime);

	FORCEINLINE float getFieldOfView() const { return this->fieldOfView; }
	FORCEINLINE float getAspectRatio() const { return this->aspectRatio; }
	FORCEINLINE float getRenderDistance() const { return this->renderDistance; }

	FORCEINLINE Matrix4 getViewMatrix() const { return this->viewMatrix; }
	FORCEINLINE Matrix4 getProjectionMatrix() const { return this->projectionMatrix; }

	FORCEINLINE Vector3 getRightVector() const { return this->rightVector; }
	FORCEINLINE Vector3 getUpVector() const { return this->upVector; }
	FORCEINLINE Vector3 getForwardVector() const { return this->forwardVector; }

private:

	void updateMatrices();
	void updateViewMatrix();
	void updateProjectionMatrix();

	Matrix4 viewMatrix;
	Matrix4 projectionMatrix;

	Vector3 rightVector;
	Vector3 upVector;
	Vector3 forwardVector;

	float fieldOfView;
	float aspectRatio;
	float renderDistance;
	float speed = 2.5f;

};

