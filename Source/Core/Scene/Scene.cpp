#include "Scene.h"
#include <Engine.h>

Scene::Scene() {
    
    this->logger = std::make_unique<Logger>("Scene");
    this->cameraManager = std::make_unique<CameraManager>(this);
    logger->info("Constructed scene.");

}

void Scene::renderComponents() const {
    
    // For each registered SceneComponent
    for (auto component: registeredComponents) {

        // If it is renderable
        if (component->instanceOf<IRenderable>()) {

            // Cast to renderable and call render()
            IRenderable *renderable = component->cast<IRenderable>();
            renderable->render();

        }

    }

}

void Scene::updateScene(float deltaTime) const {
    
    // Updates all the registered SceneObjects
    for (auto object: registeredObjects) {
        object->update(deltaTime);
    }

}

void Scene::setupInput(SceneObject *object) {

    object->setupInput(Engine::getInstance()->getInputManager());

}
