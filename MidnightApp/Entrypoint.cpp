//
// Created by sl4sh on 26/04/22.
//

#include <Core/Core.h>
#include <Console/Console.h>
#include <Memory/WeakPointer.h>
#include <Platform/Entry.h>

#include <Scene/FlyingCharacter.h>
#include <Scene/ModelComponent.h>

#include <Rendering/ShaderPrograms.h>
#include <Scene/DirectionalLightComponent.h>

#include <SDL2/SDL_keycode.h>

#include <ActorFactory.h>

#include <Scene/SpotLightComponent.h>

class SpaceScene : public Scene {

public:
    NODISCARD FORCEINLINE String getSceneName() const override { return "SpaceScene"; }

    explicit SpaceScene(PhysicsManager* const manager) : Scene(manager) { }

protected:
    void start() override {

        setWorldColor(Color(10, 10, 10).value);

        this->planet1 = ActorFactory::makeModelActor(this, "Sphere");
        this->planet2 = ActorFactory::makeModelActor(this, "Sphere", "", {{3.0f, 0.0f, 0.0f}, {}, Vector3(0.25f)});
        this->planet3 = ActorFactory::makeModelActor(this, "Sphere", "", {{4.0f, 0.0f, 0.0f}, {}, Vector3(0.125f)});

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
            LinearColor::fromLinearRGB(sin(time), cos(time), tan(time)));

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

        ActorFactory::makeCameraActor(this, Transform({0.0f, 12.0f, 8.0f}, {-90.0f, -45.0f, 0.0f}));
        ActorFactory::makeCameraActor(this, Transform({0.0f, -4.0f, -27.0f}, {90.0f, -20.0f, 0.0f}));
        ActorFactory::makeCameraActor(this, Transform({-60.0f, -5.0f, -25.0f}, {17.5f, -25.0f, 0.0f}));

        this->character = createActor<FlyingCharacter>({{-5.0f, 5.0f, 0.0f}, {0.0f, -45.0f, 0.0f}});

        this->character->setMovementSpeed(50.0f);

        this->skybox = ActorFactory::makeModelActor(this,
                                                    "SkyCube",
                                                    "",
                                                    {
                                                        {},
                                                        {90.0f, 0.0f, 0.0f},
                                                        Vector3(10000.0f)
                                                    });

        this->light = ActorFactory::makeDirectionalLightActor(this,
                                                              {0.5f, -1.0f, 0.0f},
                                                              LinearColor::fromRGB(106, 77, 102));


        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setTexture(0, "Space");
        comp->setShader(0, ShaderPrograms::skyboxShader);

        for (int i = 0; i < 5; ++i) {
            if (i % 2 == 0) {
                ActorFactory::makeStaticMeshCollider(this,
                                                     "Cascade",
                                                     "",
                                                     {{0, -20.0f * i, 0}, {180.0f, 0.0f, 0.0f}});
            }
            else {
                
                ActorFactory::makeStaticMeshCollider(this,
                                                     "Cascade",
                                                     "",
                                                     {{0, -20.0f * i, -15.0f}, {0.0f, 0.0f, 180.0f}});
            }
        }

        InputManager* manager = Engine::getInstance()->getInputManager();

        manager->bindEvent(this, KeyBind(SDLK_KP_0), EInputEvent::Pressed, &RenderScene::setTexture0);
        manager->bindEvent(this, KeyBind(SDLK_KP_1), EInputEvent::Pressed, &RenderScene::setTexture1);
        manager->bindEvent(this, KeyBind(SDLK_KP_2), EInputEvent::Pressed, &RenderScene::setTexture2);
        manager->bindEvent(this, KeyBind(SDLK_KP_3), EInputEvent::Pressed, &RenderScene::setTexture3);

        manager->bindEvent(this, KeyBind(SDLK_1), EInputEvent::Pressed, &RenderScene::imp);
        manager->bindEvent(this, KeyBind(SDLK_2), EInputEvent::Pressed, &RenderScene::imp2);
        manager->bindEvent(this, KeyBind(SDLK_3), EInputEvent::Pressed, &RenderScene::imp3);
        manager->bindEvent(this, KeyBind(SDLK_4), EInputEvent::Pressed, &RenderScene::imp4);

    }

    void imp() { getCameraManager()->setActiveCameraByIndex(0); }

    void imp2() { getCameraManager()->setActiveCameraByIndex(1); }

    void imp3() { getCameraManager()->setActiveCameraByIndex(2); }

    void imp4() { getCameraManager()->setActiveCameraByIndex(3); }

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

        float time = Engine::getInstance()->getTime();
        
        if (time - counter >= 0.2f) {

            ActorFactory::makeSphereCollisionActor(this, "SmoothSphere",
                                       Engine::getInstance()->getResourceLoader()->getRandomTexture()
                                       , 1.0f,
                                       ActorFactory::CollisionType::Dynamic,
                                       Transform({x * 4.0f, 10.0f, 0.0f}));

            counter = time;
            
            if(x == 2.0f) {
                x = -2.0f;
            }
            else {
                x += 1.0f;
            }
            
        }

        
    }

    float x = -2.0f;
    float counter = 0.0f;
    Actor* skybox = nullptr;
    Actor* plane = nullptr;
    Actor* model = nullptr;
    FlyingCharacter* character = nullptr;
    Actor* light = nullptr;

};

class SpotlightScene : public Scene {

public:
    explicit SpotlightScene(PhysicsManager* const manager) : Scene(manager) { }

    void start() override {

        ActorFactory::makeSphereCollisionActor(this,
                                               "SmoothSphere",
                                               "",
                                               1.0f,
                                               ActorFactory::CollisionType::Static,
                                               Transform({-4.0f, 0.0f, 0.0f}));

        ActorFactory::makeSphereCollisionActor(this,
                                               "SmoothSphere",
                                               "",
                                               1.0f,
                                               ActorFactory::CollisionType::Static,
                                               Transform({0.0f, 0.0f, 0.0f}));

        ActorFactory::makeSphereCollisionActor(this,
                                               "SmoothSphere",
                                               "",
                                               1.0f,
                                               ActorFactory::CollisionType::Static,
                                               Transform({4.0f, 0.0f, 0.0f}));

        ActorFactory::makeModelActor(this,
                                     "Cube",
                                     "",
                                     {
                                         {0.0f, -2.0f, 0.0f},
                                         {0.0f, 0.0f, 0.0f},
                                         {12.0f, 1.0f, 4.0f}
                                     });

        const Vector3 lightDir = {0.0f, -1.0f, 0.0f};

        light = ActorFactory::makeSpotLightActor(this, {-4.0f, 0.0f, 0.0f}, lightDir, LinearColors::white);

        light->getFirstComponentOf<SpotLightComponent>()->setCutoff(1.0f);
        light->getFirstComponentOf<SpotLightComponent>()->setOuterCutoff(20.0f);

        ActorFactory::makeCameraActor(this,
                                      {
                                          {0.0f, 3.0f, 3.0f},
                                          {-90.0f, -45.0f, 0.0f},
                                          Vector3(1.0f)
                                      });

    }


    NODISCARD FORCEINLINE String getSceneName() const override { return "SpotlightScene"; }

protected:
    void update(float deltaTime) override {

        Scene::update(deltaTime);

        const float time = Engine::getInstance()->getTime();

        LinearColor& color = light->getFirstComponentOf<LightComponent>()->getDiffuseColor();

        color.setRed(abs(sin(time)));
        color.setGreen(abs(cos(time / 2.0f)));
        color.setBlue(abs(sin(time / 3.0f)));

        light->setWorldPosition({sin(time) * 4.0f, 5.0f, 0.0f});

    }

private:
    Actor* light = nullptr;
};

class TestScene : public Scene {

public:
    NODISCARD FORCEINLINE String getSceneName() const override { return "TestScene"; }

    explicit TestScene(PhysicsManager* const manager) : Scene(manager) { }

protected:
    void start() override {

        setWorldColor(Color(10, 10, 10).value);

        this->planet1 = ActorFactory::makeModelActor(this,
                                                     "SmoothSphere",
                                                     "2k_earth_daymap",
                                                     {{}, {90.0f, 0.0f, 0.0f}});

        this->planet2 = ActorFactory::makeModelActor(this,
                                                     "SmoothSphere",
                                                     "2k_moon",
                                                     Transform({3.0f, 0.0f, 0.0f}, {}, {0.25f, 0.25f, 0.25f}));

        //this->planet2->attachTo(planet1);

        this->skybox = ActorFactory::makeModelActor(this,
                                                    "SkyCube",
                                                    "Space",
                                                    {{}, {90.0f, 0.0f, 0.0f}, Vector3(10000.0f)});

        this->light = ActorFactory::makeDirectionalLightActor(this,
                                                              {0.0f, -0.5f, 0.5f},
                                                              LinearColors::white);

        this->character = createActor<FlyingCharacter>({{-5.0f, 5.0f, 0.0f}, {0.0f, -45.0f, 0.0f}});

        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::unlit);
        planet1->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4(0.0f);

        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setShader(0, ShaderPrograms::skyboxShader);
        comp->setVisible(false);

        InputManager* manager = Engine::getInstance()->getInputManager();

        manager->bindEvent(this, KeyBind(SDLK_l), EInputEvent::Pressed, &TestScene::setLight);
        manager->bindEvent(this, KeyBind(SDLK_f), EInputEvent::Pressed, &TestScene::setWireframe);
        manager->bindEvent(this, KeyBind(SDLK_k), EInputEvent::Pressed, &TestScene::setSkybox);
        manager->bindEvent(this, KeyBind(SDLK_u), EInputEvent::Pressed, &TestScene::setUnlit);

    }

    void setSkybox() {

        auto* model = this->skybox->getFirstComponentOf<ModelComponent>();
        model->setVisible(!model->isVisible());

    }

    void setLight() {
        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::materialShader);
        light->getFirstComponentOf<LightComponent>()->setDiffuseColor(LinearColors::white);
    }

    void setUnlit() {
        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::unlit);
        planet1->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;
    }

    void setWireframe() {
        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
        planet1->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;
    }

    void update(const float deltaTime) override {

        Scene::update(deltaTime);

        const float time = Engine::getInstance()->getTime();

        planet1->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});
        planet2->addWorldRotation({0.0f, 25.0f * deltaTime, 0.0f});

        planet2->getRootComponent()->rotateAround({}, {0.0f, 1.0f, 0.0f}, {rot, 0.0f, rot});

        rot += 10.0f * deltaTime;

    }

    float rot = 0.0f;

    FlyingCharacter* character = nullptr;
    Actor* skybox = nullptr;
    Actor* light = nullptr;
    Actor* planet1 = nullptr;
    Actor* planet2 = nullptr;

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
        const auto camInfoNode = CommandNode::make("camera.info");

        unloadNode->setNodeDescription("Unloads the active scene.");
        loadNode->setNodeDescription("Loads a scene.");
        addMeshNode->setNodeDescription("Adds a mesh to the current scene");
        listObjNode->setNodeDescription("Lists the objects present in the scene.");
        deleteObjNode->setNodeDescription("Removes an object in the scene.");
        camInfoNode->setNodeDescription("Gets info about the current camera.");

        camInfoNode->addExecutable([this](const auto* context) {
            auto scene = getActiveScene();
            Vector3 pos = scene->getCameraManager()->getActiveCamera()->getWorldPosition();
            Vector3 rot = scene->getCameraManager()->getActiveCamera()->getWorldRotation();

            Console::getLogger()->info("Camera Info: \nPosition: x: {}, y: {}, z: {}\nRotation: x: {}, y: {}, z: {}",
                                       pos.x, pos.y, pos.z, rot.x, rot.y, rot.z);
        });

        unloadNode->addExecutable([this](const auto* context) { unloadScene(); });

        loadNode->addArgument("sceneName", ArgumentType::String)
                ->addExecutable([this](const auto* context) {
                    if (String sceneName = context->getString("sceneName"); sceneName == "SpaceScene") {
                        loadScene<SpaceScene>();
                    }
                    else if (sceneName == "RenderScene") { loadScene<RenderScene>(); }
                    else if (sceneName == "SpotlightScene") { loadScene<SpotlightScene>(); }
                    else if (sceneName == "TestScene") { loadScene<TestScene>(); }
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

                ActorFactory::makeModelActor(getActiveScene().raw(), modelName, "", Transform(Vector3(x, y, z)));
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
        Console::registerCommand(camInfoNode);

        Console::exec("scene.load RenderScene");

        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F1), EInputEvent::Pressed, &MyEngine::loadScene<RenderScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F2), EInputEvent::Pressed, &MyEngine::loadScene<SpaceScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F3), EInputEvent::Pressed, &MyEngine::loadScene<TestScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F4), EInputEvent::Pressed, &MyEngine::loadScene<SpotlightScene>);

    }

    float counter = 0.0f;
    bool b = false;
    void onUpdate() override {

        float time = getTime();

        if(time - counter >= 0.1f) {

            if(b) {
                loadScene<RenderScene>();
            }
            else {
                loadScene<SpaceScene>();
            }

            b = !b;
            counter = time;
            
        }
                
    }

};

#define DEBUG_LEAKS

int main(const int argc, const char** argv) {

#if defined(_WIN64) && defined(DEBUG_LEAKS) && defined(DEBUG)

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
