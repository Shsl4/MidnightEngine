#include "SceneObject.h"

void SceneObject::start()
{
	rootComponent->start();
}

void SceneObject::update(float deltaTime)
{
	rootComponent->update(deltaTime);
}
