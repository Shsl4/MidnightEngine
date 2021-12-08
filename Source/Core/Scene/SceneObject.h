#pragma once

#include <EngineTypes.h>
#include <Math/Vector3.h>
#include <Memory/Array.h>
#include <Scene/SceneComponent.h>
#include <Object.h>

class SceneObject : public Object {

public:

	void start();
	void update(float deltaTime);

private:

	SceneComponent* rootComponent;
  
};