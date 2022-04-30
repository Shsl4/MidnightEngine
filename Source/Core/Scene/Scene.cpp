#include "Scene.h"
#include <Engine.h>
#include <Memory/UniquePtr.h>

Scene::Scene() : logger(UniquePtr<Logger>::make("Scene")), cameraManager(UniquePtr<CameraManager>::make(this)) {
    
}

void Scene::renderComponents() const {
    
    // For each registered SceneComponent
    for (auto const &component: registeredComponents) {

        // If it is renderable
        if (component->inherits<Renderable>()) {

            // Cast to renderable and call render.
            auto* renderable = component->cast<Renderable>();
            renderable->render();

        }

    }

}

void Scene::updateScene(float deltaTime) const {
    
    // Updates all the registered SceneObjects.
    for (auto const& object: registeredObjects) {
        object->update(deltaTime);
    }

}

void Scene::setupInput(SceneObject *object) {

    // Forward the call to the object.
    object->setupInput(Engine::getInstance()->getInputManager());

}

bool Scene::destroyComponent(SceneComponent *component){
    
    if (!component) { return false; }

    component->detachFromComponent();

    // If the component is a CameraComponent, unregister it from the CameraManager.
    if (component->inherits<CameraComponent>()) {

        cameraManager->unregisterCamera(component->cast<CameraComponent>());

    }

    return registeredComponents.removeFirstOf(component);
    
}
