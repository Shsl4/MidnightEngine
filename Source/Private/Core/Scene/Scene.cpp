#include <Scene/Scene.h>
#include <Core/Engine.h>
#include <Input/InputManager.h>
#include <Memory/UniquePointer.h>
#include <bgfx/bgfx.h>

#include <PhysX/extensions/PxDefaultSimulationFilterShader.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>

#include <PhysX/extensions/PxSimpleFactory.h>

#include "PxMaterial.h"

Color::Color(const UInt8 red, const UInt8 green, const UInt8 blue) {
        
    this->value = 0;
    setByte(this->value, red, 3);
    setByte(this->value, green, 2);
    setByte(this->value, blue, 1);
    setByte(this->value, 255, 0);
        
}

Scene::Scene(PhysicsManager* manager) : cameraManager(UniquePointer<CameraManager>::make(this)) {
    
    physx::PxPhysics* physics = manager->getPhysics();
    
    auto sceneDesc = physx::PxSceneDesc(physics->getTolerancesScale());
    
    sceneDesc.gravity = physx::PxVec3(0, -9.81f, 0);
    sceneDesc.cpuDispatcher = manager->getCPUDispatcher();
    sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
    
    physicsScene = physics->createScene(sceneDesc);

    physx::PxMaterial* material = manager->getPhysicsMaterial();
    
    physx::PxRigidStatic* groundPlane = PxCreatePlane(*physics, physx::PxPlane(0, 1, 0, 1), *material);
        
    physicsScene->addActor(*groundPlane);
    
}

void Scene::load() {
    
    this->state = State::Loading;
    
    PhysicsManager::updatePvdClient(physicsScene);
    
    start();
    
    this->state = State::Loaded;
}

void Scene::cleanup()
{
    this->state = State::Unloading;

    physicsScene->lockWrite(__FILE__, __LINE__);
    
    const auto inputManager = Engine::getInstance()->getInputManager();

    for (auto const& e : registeredActors) {
        inputManager->unbindAll(e.raw());
    }

    registeredActors.clear();
    
    setWorldColor(0);
    
    physicsScene->simulate(0.1f);

    physicsScene->fetchResults(true);

    physicsScene->release();

    this->state = State::Unloaded;
}

void Scene::listObjects() const {

    Console::getLogger()->info("Listing {} active objects: ", getSceneName());

    size_t counter = 0;
    
    for (const auto& object : registeredActors)
    {
        Console::getLogger()->info("[{}] {} ({})", counter, object->getClassName(), fmt::ptr(object.raw()));
        ++counter;
    }
    
}


void Scene::listComponents() const {

    Console::getLogger()->info("Listing {} active components: ", getSceneName());

    size_t counter = 0;
    
    for (const auto& actor : registeredActors)
    {
        
        for (const auto& component : actor->getRootComponent()->getChildComponents()) {

            Console::getLogger()->info("[{}] {} ({})", counter, component->getClassName(), fmt::ptr(component));
            
        }
        
        ++counter;
    }
    
}


void Scene::setWorldColor(const UInt32 color) const
{
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, color, 1.0f, 0);
}

Actor* Scene::getObjectByIndex(size_t index) const {
    
    if(index < 0 || index >= registeredActors.getSize()) { return nullptr; }

    return registeredActors[index].raw();
    
}

void Scene::renderComponents() const {
    
    // For each registered Actor
    for (const auto& actor : registeredActors) {

        // \todo refactor this mess
        // If it is renderable
        if (actor->getRootComponent()->inherits<Renderable>()) {

            // Cast to renderable and call render.
            actor->getRootComponent()->cast<Renderable>()->render();

        }
        
        // For each registered SceneComponent
        for (auto* component : actor->getRootComponent()->getChildComponents()) {

            // If it is renderable
            if (component->inherits<Renderable>()) {

                // Cast to renderable and call render.
                component->cast<Renderable>()->render();

            }
            
        }
        
    }
    
}

void Scene::update(const float deltaTime) {
    
    // Updates all the registered Actors.
    for (auto const& object: registeredActors) {
        object->update(deltaTime);
    }

}

void Scene::setupInput(Actor *object) {

    // Forward the call to the object.
    object->setupInput(Engine::getInstance()->getInputManager());

}

void Scene::updatePhysics(float deltaTime) {

    physicsScene->lockWrite(__FILE__, __LINE__);
    physicsScene->simulate(deltaTime);
    
}

void Scene::waitForPhysics()
{
    physicsScene->fetchResults(true);
    physicsScene->unlockWrite();
}

bool Scene::destroyActor(Actor* object) {

    if (!object) { return false; }
    
    size_t i = 0;
    
    for (auto const& e : registeredActors) {
        if (object == e.raw()) {
            break;
        }
        ++i;
    }
    
    registeredActors.removeAt(i);

    return true;    
}