//
// Created by sl4sh on 26/04/22.
//

#include <Core/Core.h>
#include <Console/Console.h>
#include <Memory/WeakPointer.h>
#include <Platform/Entry.h>

#include <Scene/ModelActor.h>
#include <Scene/FlyingCharacter.h>
#include <Scene/ModelComponent.h>

#include <Rendering/ShaderPrograms.h>
#include <Scene/DirectionalLightComponent.h>

#include <Scene/PointLightComponent.h>

#include "SDL2/SDL_keycode.h"

class LightActor : public Actor {
    
public:

    LightActor() {

        this->light = createComponent<DirectionalLightComponent>("DirectionalLight");
        this->model = createComponent<ModelComponent>("Model");
        light->setLightDirection({1.0f, -1.0f, 0.0f});
        setRootComponent(model);
        light->attachTo(model);
        
    }

    FORCEINLINE DirectionalLightComponent* getLightComponent() const {
        return light;
    }
    
private:
    
    DirectionalLightComponent* light;
    ModelComponent* model;
    Vector3 rotation = Vector3::zero;
    
};

class SpaceScene : public Scene
{

public:

    NODISCARD FORCEINLINE String getSceneName() const override
    {
        return "SpaceScene";
    }

protected:
    
    void start() override {

        setWorldColor(Color(10, 10, 10).value);

        this->planet1 = createObject<ModelActor>();
        this->planet2 = createObject<ModelActor>(Transform({ 3.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(0.25f)));
        this->planet3 = createObject<ModelActor>(Transform({ 4.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(0.125f)));
        
        this->planet1->setModel("Sphere");
        this->planet2->setModel("Sphere");
        this->planet3->setModel("Sphere");
        
        this->light = createObject<LightActor>(Transform({ 0.0f, 0.0f, 0.0f }, {}, { 0.25f, 0.25f, 0.25f }));

        this->character = createObject<FlyingCharacter>({ { -5.0f, 5.0f, 0.0f }, { 0.0f, -45.0f, 0.0f } });

        planet2->attachTo(planet1);
        planet3->attachTo(planet2);

    }
    
    void update(const float deltaTime) override {

        Scene::update(deltaTime);

        const float time = Engine::getInstance()->getTime();
        
        planet1->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        planet2->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        this->light->addWorldPosition({ 0.0f, sin(time) * deltaTime, 0.0f });
        
    }
        
    FlyingCharacter* character = nullptr;
    LightActor* light = nullptr;
    
    ModelActor* planet1 = nullptr;
    ModelActor* planet2 = nullptr;
    ModelActor* planet3 = nullptr;
    
};

class RenderScene : public Scene
{
public:
    
    void start() override {

        setWorldColor(Color(0, 0, 0).value);
        
        this->skybox = createObject<ModelActor>(Transform({}, { -90.0f, 0.0f, 0.0f }, Vector3(1000.0f)));
        this->character = createObject<FlyingCharacter>({ { -5.0f, 5.0f, 0.0f }, { 0.0f, -45.0f, 0.0f } });
        this->light = createObject<LightActor>(Transform({}, {}, { 0.25f, 0.25f, 0.25f }));

        light->getLightComponent()->setDiffuseColor(LinearColor::fromRGB(106, 77, 102));

        skybox->setModel("SkyCube");
        skybox->setTexture(0, "Space");
        skybox->setShader(0, ShaderPrograms::skyboxShader);

#ifdef MULTI
        
        for (Int64 i = -4; i < 5; ++i) {

            this->model = createObject<ModelActor>(Transform({ 10.0f * i, 0.0f, 0.0f }, { 90.0f, 0.0f, 0.0f }, Vector3(10.0f)));
            model->setModel("marble_bust_01_2k");
            
        }

#else

        this->model = createObject<ModelActor>(Transform({ 10.0f, 0.0f, 0.0f }, { 90.0f, 0.0f, 0.0f }, Vector3(10.0f)));
        model->setModel("marble_bust_01_2k");

#endif
        
        InputManager* manager = Engine::getInstance()->getInputManager();

        manager->bindEvent(this, KeyBind(SDLK_KP_0), EInputEvent::Pressed, &RenderScene::setTexture0);
        manager->bindEvent(this, KeyBind(SDLK_KP_1), EInputEvent::Pressed, &RenderScene::setTexture1);
        manager->bindEvent(this, KeyBind(SDLK_KP_2), EInputEvent::Pressed, &RenderScene::setTexture2);
        manager->bindEvent(this, KeyBind(SDLK_KP_3), EInputEvent::Pressed, &RenderScene::setTexture3);
        
        this->character->setMovementSpeed(5.0f);
        
    }

    void setTexture0() {
        light->getLightComponent()->setDiffuseColor(LinearColors::white);
        skybox->setModel("Sphere");
        skybox->setTexture(0, "brown_photostudio_02_4k");
        skybox->setShader(0, ShaderPrograms::skyboxShader);
    }

    void setTexture1() {
        light->getLightComponent()->setDiffuseColor(LinearColor::fromRGB(174, 160, 154));
        skybox->setModel("Sphere");
        skybox->setTexture(0, "moonless_golf_4k");
        skybox->setShader(0, ShaderPrograms::skyboxShader);
    }

    void setTexture2() {
        light->getLightComponent()->setDiffuseColor(LinearColors::white);
        skybox->setModel("Sphere");
        skybox->setTexture(0, "studio_small_09_4k");
        skybox->setShader(0, ShaderPrograms::skyboxShader);
    }

    void setTexture3() {
        light->getLightComponent()->setDiffuseColor(LinearColor::fromRGB(106, 77, 102));
        skybox->setModel("SkyCube");
        skybox->setTexture(0, "Space");
        skybox->setShader(0, ShaderPrograms::skyboxShader);
    }
    
    void update(const float deltaTime) override {

        Scene::update(deltaTime);

        //const float time = Engine::getInstance()->getTime();

        //this->light->getLightComponent()->setLightDirection({ sin(time), -1.0f, 0.0f});

#ifndef MULTI
        model->addWorldRotation({ 0.0f, 10.0f * deltaTime, 0.0f });
#endif
        //light->getLightComponent()->setDiffuseColor(LinearColor::fromLinearRGB(sin(time), cos(time), sin(2.0f * time)));
        
    }
    
    NODISCARD FORCEINLINE String getSceneName() const override
    {
        return "RenderScene";
    }

    ModelActor* skybox = nullptr;
    ModelActor* model = nullptr;
    FlyingCharacter* character = nullptr;
    LightActor* light = nullptr;

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

        unloadNode->addExecutable([this](const auto* context) {
            unloadScene();
        });

        loadNode->addArgument("sceneName", ArgumentType::String)
                ->addExecutable([this](const auto* context) {
                    
                    if(String sceneName = context->getString("sceneName"); sceneName == "SpaceScene")
                    {
                        loadScene<SpaceScene>();
                    }
                    else if(sceneName == "RenderScene")
                    {
                        loadScene<RenderScene>();
                    }
                    else
                    {
                        CommandError::throwError("The scene \"{}\" does not exist.", sceneName);
                    }                    
                    
                });
        
        auto* node = addMeshNode->addArgument("modelName", ArgumentType::String);
        
        node->addExecutable([this](const auto* context) {

            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

            const String modelName = context->getString("modelName");
                       
            getActiveScene()->createObject<ModelActor>()->setModel(modelName);                       
                                              
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
                       
                getActiveScene()->createObject<ModelActor>(Transform(Vector3(x, y, z)))->setModel(modelName);                       
                                              
            });

        listObjNode->addExecutable([this](const auto* context)
        {
            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");
            getActiveScene()->listObjects();            
        });
        
        listCompNode->addExecutable([this](const auto* context)
        {
            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");
            getActiveScene()->listComponents();            
        });

        deleteObjNode->addArgument("index", ArgumentType::Int64)
                     ->addExecutable([this](const auto* context) {

                         CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

                         const Int64 index = context->getInt64("index");
                         
                         getActiveScene()->destroyActor(getActiveScene()->getObjectByIndex(index));
                         
                     });

        posNode->addArgument("index", ArgumentType::Int64)->addExecutable([this](const auto* context)
        {

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

    void onUpdate() override {

    }

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
    
    entry->entry(argc, argv, [pointer]() {
        return pointer;
    });

    return 0;

}