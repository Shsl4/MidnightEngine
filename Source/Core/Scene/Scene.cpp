#include "Scene.h"
#include <Rendering/Renderable.h>

void Scene::renderComponents()
{

	for (SceneComponent* component : registeredComponents)
	{
		if (component->instanceOf<Renderable>()) {

			Renderable* renderable = component->cast<Renderable>();
			renderable->render();

		}

	}

}

void Scene::updateScene(float deltaTime)
{

	for (SceneComponent* object : registeredComponents) {

		object->update(deltaTime);

	}

}