//
// Created by sl4sh on 26/04/22.
//

#include <Core/Core.h>
#include <Console/Console.h>
#include <Memory/WeakPointer.h>
#include <Platform/Entry.h>

#include <Scene/PhysicsModelActor.h>
#include <Scene/FlyingCharacter.h>
#include <Scene/ModelComponent.h>

#include <Rendering/ShaderPrograms.h>
#include <Scene/DirectionalLightComponent.h>

#include <Scene/PointLightComponent.h>

#include "PxRigidBody.h"
#include "PxRigidStatic.h"
#include "extensions/PxSimpleFactory.h"
#include "SDL2/SDL_keycode.h"

class ActorFactory {

public:
    
    static Actor* makePointLightActor(Scene* scene, Vector3 const& position, LinearColor const& color) {
        
        auto* actor = scene->createActor<Actor>(Transform(position));
        auto* comp = actor->createComponent<PointLightComponent>("PointLight");
        comp->setDiffuseColor(color);
        comp->attachTo(actor);

        return actor;
        
    }

    static Actor* makeDirectionalLightActor(Scene* scene, Vector3 const& direction, LinearColor const& color) {
        
        auto* actor = scene->createActor<Actor>();
        auto* comp = actor->createComponent<DirectionalLightComponent>("DirectionalLight");
        comp->setLightDirection(direction);
        comp->setDiffuseColor(color);
        comp->attachTo(actor);

        return actor;
        
    }

    static Actor* makeModelActor(Scene* scene, String const& modelPath, Vector3 const& position = Vector3::zero,
        Vector3 const& rotation = Vector3::zero, Vector3 const& scale = Vector3::one) {
        
        auto* actor = scene->createActor<Actor>(Transform(position, rotation, scale));
        auto* comp = actor->createComponent<ModelComponent>("Model");
        comp->setModel(modelPath);
        comp->attachTo(actor);
        comp->setWorldTransform({ position, rotation, scale });

        return actor;
        
    }

    static Actor* makePhysicsModelActor(Scene* scene, String const& modelPath, Vector3 const& position = Vector3::zero,
        Vector3 const& rotation = Vector3::zero, Vector3 const& scale = Vector3::one) {
        
        auto* actor = scene->createActor<Actor>(Transform(position, rotation, scale));
        auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
        auto* model = actor->createComponent<ModelComponent>("Model");
        
        actor->setRootComponent(physicsComp);
        model->attachTo(physicsComp);

        actor->setWorldTransform({ position, rotation, scale });
        
        model->setModel(modelPath);
        physicsComp->makeSphereCollider();

        return actor;
        
    }
    
};

class SpaceScene : public Scene {

public:
    
    NODISCARD FORCEINLINE String getSceneName() const override { return "SpaceScene"; }

    explicit SpaceScene(PhysicsManager* const manager) : Scene(manager) { }

protected:
    
    void start() override {

        setWorldColor(Color(10, 10, 10).value);

        this->planet1 = ActorFactory::makeModelActor(this, "Cube");
        this->planet2 = ActorFactory::makeModelActor(this, "Cube", { 3.0f, 0.0f, 0.0f }, {}, Vector3(0.25f));
        this->planet3 = ActorFactory::makeModelActor(this, "Cube", { 4.0f, 0.0f, 0.0f }, {}, Vector3(0.125f));
        
        this->light = ActorFactory::makeDirectionalLightActor(this, {0.5f, -1.0f, 0.0f}, LinearColors::white);
        this->light = ActorFactory::makeDirectionalLightActor(this, {-0.5f, 1.0f, 0.0f}, LinearColors::red);
        
        this->character = createActor<FlyingCharacter>({{-5.0f, 5.0f, 0.0f}, {0.0f, -45.0f, 0.0f}});

        planet2->attachTo(planet1);
        planet3->attachTo(planet2);

        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
        planet1->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;

        planet2->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
        planet2->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::red;

        planet3->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
        planet3->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::green;

    }

    void update(const float deltaTime) override {

        Scene::update(deltaTime);

        const float time = Engine::getInstance()->getTime();

        planet1->addWorldRotation({5.0f * deltaTime, 50.0f * deltaTime, 0.0f});
        planet2->addWorldRotation({0.0f, 25.0f * deltaTime, 10.0f * deltaTime});
        this->light->getFirstComponentOf<DirectionalLightComponent>()->setDiffuseColor(
            LinearColor::fromLinearRGB( sin(time), cos(time), tan(time)));

    }

    FlyingCharacter* character = nullptr;
    Actor* light = nullptr;

    Actor* planet1 = nullptr;
    Actor* planet2 = nullptr;
    Actor* planet3 = nullptr;

};

class RenderScene : public Scene {

public:
    
    explicit RenderScene(PhysicsManager* const manager) : Scene(manager) { }

    NODISCARD FORCEINLINE String getSceneName() const override { return "RenderScene"; }

protected:
    
    void start() override {

        setWorldColor(Color(0, 0, 0).value);

        this->character = createActor<FlyingCharacter>({{-5.0f, 5.0f, 0.0f}, {0.0f, -45.0f, 0.0f}});
        this->skybox = ActorFactory::makeModelActor(this, "SkyCube", {}, { 90.0f, 0.0f, 0.0f}, Vector3(10000.0f));
        this->plane = ActorFactory::makeModelActor(this, "Plane", { 0.0f, -1.0f, 0.0f}, { 90.0f, 0.0f, 0.0f}, Vector3(1000.0f));
        this->light = ActorFactory::makeDirectionalLightActor(this, {0.5f, -1.0f, 0.0f}, LinearColor::fromRGB(106, 77, 102));

        plane->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::unlit);
        plane->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColor::fromLinearRGB(0.8f, 0.8f, 0.8f);

        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setTexture(0, "Space");
        comp->setShader(0, ShaderPrograms::skyboxShader);

#ifdef MULTI
        
        for (Int64 i = -4; i < 5; ++i) {

            this->model = createObject<ModelActor>(Transform({ 10.0f * i, 0.0f, 0.0f }, { 90.0f, 0.0f, 0.0f }, Vector3(10.0f)));
            model->setModel("marble_bust_01_2k");
            
        }

#else

        this->model = ActorFactory::makePhysicsModelActor(this, "Sphere", {0.0f, 0.0f, 0.0f});
        
        for (Int64 i = 1; i < 100; ++i ) {

            ActorFactory::makePhysicsModelActor(this, "Sphere", {0.0f, 10.0f * i, 0.0f});
            //model->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
            //model->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;
            
        }
        
        for (Int64 i = 1; i < 100; ++i ) {

            ActorFactory::makePhysicsModelActor(this, "Sphere", {10.0f, 10.0f * i, 0.0f});
            //model->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
            //model->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;
            
        }

        for (Int64 i = 1; i < 100; ++i ) {

            ActorFactory::makePhysicsModelActor(this, "Sphere", {-10.0f, 10.0f * i, 0.0f});
            //model->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
            //model->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;
            
        }


#endif

        InputManager* manager = Engine::getInstance()->getInputManager();

        manager->bindEvent(this, KeyBind(SDLK_KP_0), EInputEvent::Pressed, &RenderScene::setTexture0);
        manager->bindEvent(this, KeyBind(SDLK_KP_1), EInputEvent::Pressed, &RenderScene::setTexture1);
        manager->bindEvent(this, KeyBind(SDLK_KP_2), EInputEvent::Pressed, &RenderScene::setTexture2);
        manager->bindEvent(this, KeyBind(SDLK_KP_3), EInputEvent::Pressed, &RenderScene::setTexture3);

        manager->bindEvent(this, KeyBind(SDLK_KP_4), EInputEvent::Pressed, &RenderScene::imp);
        manager->bindEvent(this, KeyBind(SDLK_KP_5), EInputEvent::Pressed, &RenderScene::imp2);
        manager->bindEvent(this, KeyBind(SDLK_KP_6), EInputEvent::Pressed, &RenderScene::imp3);
        
        this->character->setMovementSpeed(50.0f);

        for (const auto& actor : this->registeredActors) {
            actor->start();
        }

    }

    void imp() {

        model->getFirstComponentOf<PhysicsComponent>()->addImpulse({0, 0, 10});
        
    }
    
    void imp2() {

        model->getFirstComponentOf<PhysicsComponent>()->addImpulse({30, 100, 0});
        
    }
    
    void imp3() {

        model->getFirstComponentOf<PhysicsComponent>()->setSimulatePhysics(true);
        
    }
    
    void setTexture0() {
        light->getFirstComponentOf<LightComponent>()->setDiffuseColor(LinearColors::white);
        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setModel("Sphere");
        comp->setTexture(0, "brown_photostudio_02_4k");
        comp->setShader(0, ShaderPrograms::skyboxShader);
    }

    void setTexture1() {
        light->getFirstComponentOf<LightComponent>()->setDiffuseColor(LinearColor::fromRGB(174, 160, 154));
        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setModel("Sphere");
        comp->setTexture(0, "moonless_golf_4k");
        comp->setShader(0, ShaderPrograms::skyboxShader);
    }

    void setTexture2() {
        light->getFirstComponentOf<LightComponent>()->setDiffuseColor(LinearColors::white);
        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setModel("Sphere");
        comp->setTexture(0, "studio_small_09_4k");
        comp->setShader(0, ShaderPrograms::skyboxShader);
    }

    void setTexture3() {
        light->getFirstComponentOf<LightComponent>()->setDiffuseColor(LinearColor::fromRGB(106, 77, 102));
        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setModel("SkyCube");
        comp->setTexture(0, "Space");
        comp->setShader(0, ShaderPrograms::skyboxShader);
    }

    void update(const float deltaTime) override {

        Scene::update(deltaTime);
        
        //const float time = Engine::getInstance()->getTime();

        //this->light->getLightComponent()->setLightDirection({ sin(time), -1.0f, 0.0f});

#ifndef MULTI
        //model->addWorldRotation({ 0.0f, 10.0f * deltaTime, 0.0f });
#endif
        //light->getLightComponent()->setDiffuseColor(LinearColor::fromLinearRGB(sin(time), cos(time), sin(2.0f * time)));

    }

    Actor* skybox = nullptr;
    Actor* plane = nullptr;
    Actor* model = nullptr;
    FlyingCharacter* character = nullptr;
    Actor* light = nullptr;

};

class MyEngine : public Engine {

    void onStart() override {

        const auto unloadNode = CommandNode::make("scene.unload");
        const auto loadNode = CommandNode::make("scene.load");
        const auto addMeshNode = CommandNode::make("scene.mesh.add");
        const auto listObjNode = CommandNode::make("scene.object.list");
        const auto posNode = CommandNode::make("scene.object.pos");
        const auto deleteObjNode = CommandNode::make("scene.object.delete");
        const auto listCompNode = CommandNode::make("scene.component.list");

        unloadNode->setNodeDescription("Unloads the active scene.");
        loadNode->setNodeDescription("Loads a scene.");
        addMeshNode->setNodeDescription("Adds a mesh to the current scene");
        listObjNode->setNodeDescription("Lists the objects present in the scene.");
        deleteObjNode->setNodeDescription("Removes an object in the scene.");

        unloadNode->addExecutable([this](const auto* context) { unloadScene(); });

        loadNode->addArgument("sceneName", ArgumentType::String)
                ->addExecutable([this](const auto* context) {
                    if (String sceneName = context->getString("sceneName"); sceneName == "SpaceScene") {
                        loadScene<SpaceScene>();
                    }
                    else if (sceneName == "RenderScene") { loadScene<RenderScene>(); }
                    else { CommandError::throwError("The scene \"{}\" does not exist.", sceneName); }
                });

        auto* node = addMeshNode->addArgument("modelName", ArgumentType::String);

        node->addExecutable([this](const auto* context) {
            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

            const String modelName = context->getString("modelName");

            ActorFactory::makeModelActor(getActiveScene().raw(), modelName);
            
        });

        node->addArgument("x", ArgumentType::Double)
            ->addArgument("y", ArgumentType::Double)
            ->addArgument("z", ArgumentType::Double)
            ->addExecutable([this](const auto* context) {
                CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

                const String modelName = context->getString("modelName");
                const auto x = static_cast<float>(context->getDouble("x"));
                const auto y = static_cast<float>(context->getDouble("y"));
                const auto z = static_cast<float>(context->getDouble("z"));

                ActorFactory::makeModelActor(getActiveScene().raw(), modelName, Vector3(x, y, z));

        });

        listObjNode->addExecutable([this](const auto* context) {
            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");
            getActiveScene()->listObjects();
        });

        listCompNode->addExecutable([this](const auto* context) {
            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");
            getActiveScene()->listComponents();
        });

        deleteObjNode->addArgument("index", ArgumentType::Int64)
                     ->addExecutable([this](const auto* context) {
                         CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

                         const Int64 index = context->getInt64("index");

                         getActiveScene()->destroyActor(getActiveScene()->getObjectByIndex(index));
                     });

        posNode->addArgument("index", ArgumentType::Int64)->addExecutable([this](const auto* context) {
            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

            const Int64 index = context->getInt64("index");

            const Actor* object = getActiveScene()->getObjectByIndex(index);

            CommandError::throwIf(!object, "The provided object index is invalid");

            const auto pos = object->getWorldPosition();
            Console::getLogger()->info("Position for {} ({}): {{ {}, {}, {} }}", object->getClassName(),
                                       fmt::ptr(object), pos.x, pos.y, pos.z);
        });

        Console::registerCommand(loadNode);
        Console::registerCommand(unloadNode);
        Console::registerCommand(addMeshNode);
        Console::registerCommand(listObjNode);
        Console::registerCommand(deleteObjNode);
        Console::registerCommand(listCompNode);
        Console::registerCommand(posNode);

        Console::exec("scene.load RenderScene");

    }

    void onUpdate() override { }

};

#define DEBUG_LEAKS

int main(const int argc, const char** argv) {

#if defined(_WIN64) && defined(DEBUG_LEAKS)

    // Enables memory leak check on Windows
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    const auto entry = UniquePointer<Entry>::make();
    const auto engine = UniquePointer<MyEngine>::make();
    const auto pointer = engine.raw();

    entry->entry(argc, argv, [pointer]() { return pointer; });

    return 0;

}
