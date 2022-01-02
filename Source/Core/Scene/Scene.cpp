#include "Scene.h"
#include <Rendering/Renderable.h>
#include <Engine.h>

void Scene::renderComponents() const
{

	for (auto component : registeredComponents)
	{
		if (component->instanceOf<Renderable>()) {

			Renderable* renderable = component->cast<Renderable>();
			renderable->render();

		}

	}

}

void Scene::updateScene(float deltaTime) const
{

	for (auto object : registeredObjects) {

		object->update(deltaTime);

	}

}

void Scene::setupInput(SceneObject* object){
    
    object->setupInput(MEngine::getInstance()->getInputManager());
    
}
