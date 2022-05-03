#include <Scene/Scene.h>
#include <Core/Engine.h>
#include <Input/InputManager.h>
#include <Memory/AutoReleasePointer.h>
#include <bgfx/bgfx.h>

Scene::Scene() : cameraManager(AutoReleasePointer<CameraManager>::make(this)), logger(AutoReleasePointer<Logger>::make("Scene")) {
    
}

void Scene::load()
{
    this->state = State::Loading;
    setup();
    this->state = State::Loaded;
}

void Scene::cleanup()
{
    this->state = State::Unloading;
    
    for (auto const& e : registeredObjects) {
        Engine::getInstance()->getInputManager()->unbindAll(e);
    }
    
    setWorldColor(0);

    this->state = State::Unloaded;
}

void Scene::setWorldColor(UInt32 color) const
{
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, color, 1.0f, 0);
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
