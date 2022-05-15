#include <Scene/Scene.h>
#include <Core/Engine.h>
#include <Input/InputManager.h>
#include <Memory/UniquePointer.h>
#include <bgfx/bgfx.h>

Color::Color(const UInt8 red, const UInt8 green, const UInt8 blue) {
        
    this->value = 0;
    setByte(this->value, red, 3);
    setByte(this->value, green, 2);
    setByte(this->value, blue, 1);
    setByte(this->value, 255, 0);
        
}

Scene::Scene() : cameraManager(UniquePointer<CameraManager>::make(this)) {
    
}

void Scene::load()
{
    this->state = State::Loading;
    start();
    this->state = State::Loaded;
}

void Scene::cleanup()
{
    this->state = State::Unloading;
    
    const auto inputManager = Engine::getInstance()->getInputManager();

    for (auto const& e : registeredObjects) {
        inputManager->unbindAll(e.raw());
    }

    registeredObjects.clear();
    registeredComponents.clear();
    
    setWorldColor(0);

    this->state = State::Unloaded;
}

void Scene::listObjects() const {

    Console::getLogger()->info("Listing {} active objects: ", getSceneName());

    size_t counter = 0;
    
    for (const auto& object : registeredObjects)
    {
        Console::getLogger()->info("[{}] {} ({})", counter, object->getClassName(), fmt::ptr(object.raw()));
        ++counter;
    }
    
}


void Scene::listComponents() const {

    Console::getLogger()->info("Listing {} active components: ", getSceneName());

    size_t counter = 0;
    
    for (const auto& component : registeredComponents)
    {
        Console::getLogger()->info("[{}] {} ({})", counter, component->getClassName(), fmt::ptr(component.raw()));
        ++counter;
    }
    
}


void Scene::setWorldColor(const UInt32 color) const
{
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, color, 1.0f, 0);
}

SceneObject* Scene::getObjectByIndex(size_t index) const {
    
    if(index < 0 || index >= registeredObjects.getSize()) { return nullptr; }

    return registeredObjects[index].raw();
    
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

void Scene::update(const float deltaTime) {
    
    // Updates all the registered SceneObjects.
    for (auto const& object: registeredObjects) {
        object->update(deltaTime);
    }

}

void Scene::setupInput(SceneObject *object) {

    // Forward the call to the object.
    object->setupInput(Engine::getInstance()->getInputManager());

}

bool Scene::destroyObject(SceneObject* object)
{

    if (!object) { return false; }

    destroyComponent(object->getRootComponent());

    size_t i = 0;
    
    for (auto const& e : registeredObjects) {
        if (object == e.raw()) {
            break;
        }
        ++i;
    }
    
    registeredObjects.removeAt(i);

    return true;    
}

bool Scene::destroyComponent(SceneComponent *component){
    
    if (!component) { return false; }

    component->detachFromComponent();

    for (const auto& child : component->getChildComponents())
    {
        if(!child->isRootComponent())
        {
            destroyComponent(child);
        }
        else
        {
            child->parentComponent = nullptr;
        }
    }
    
    // If the component is a CameraComponent, unregister it from the CameraManager.
    if (component->inherits<CameraComponent>()) {

        cameraManager->unregisterCamera(component->cast<CameraComponent>());

    }

    size_t i = 0;
    
    for (auto const& e : registeredComponents) {
        if (component == e.raw()) {
            break;
        }
        ++i;
    }
    
    registeredComponents.removeAt(i);

    return true;
    
}
