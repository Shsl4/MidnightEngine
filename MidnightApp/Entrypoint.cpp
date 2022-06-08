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
#include <PhysX/extensions/PxDefaultSimulationFilterShader.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxRigidStatic.h>
#include <PhysX/extensions/PxSimpleFactory.h>

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
                                                    "SmoothSphere",
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
        comp->setTexture(0, "2k_stars_milky_way");
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
        manager->bindEvent(this, KeyBind(SDLK_5), EInputEvent::Pressed, &RenderScene::imp5);

    }

    void imp() { getCameraManager()->setActiveCameraByIndex(0); }

    void imp2() { getCameraManager()->setActiveCameraByIndex(1); }

    void imp3() { getCameraManager()->setActiveCameraByIndex(2); }

    void imp4() { getCameraManager()->setActiveCameraByIndex(3); }

    void imp5() { state = !state; }

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

        if (time - counter >= 0.05f) {
            
            if (state) {
                
                ActorFactory::makeSphereCollisionActor(this, "SmoothSphere",
                                                       Engine::getInstance()->getResourceLoader()->getRandomTexture()
                                                       , 1.0f,
                                                       ActorFactory::CollisionType::Dynamic,
                                                       Transform({0.0f, 10.0f, 0.0f}));

                ActorFactory::makeBoxCollisionActor(this, "Cube",
                                                    Engine::getInstance()->getResourceLoader()->getRandomTexture(),
                                                    Vector3(1.0f), ActorFactory::CollisionType::Dynamic,
                                                    Transform({0.0f, 10.0f, 0.0f}));
                
            }
            else {
                ActorFactory::makeSphereCollisionActor(this, "SmoothSphere",
                                                       Engine::getInstance()->getResourceLoader()->getRandomTexture()
                                                       , 1.0f,
                                                       ActorFactory::CollisionType::Dynamic,
                                                       Transform({x * 4.0f, 10.0f, 0.0f}));
            }


            counter = time;

            if (x == 2.0f) { x = -2.0f; }
            else { x += 1.0f; }
        }


    }

    bool state = false;
    float x = -2.0f;
    float counter = 0.0f;
    Actor* skybox = nullptr;
    Actor* plane = nullptr;
    Actor* model = nullptr;
    FlyingCharacter* character = nullptr;
    Actor* light = nullptr;

};

class PhysXScene : public Scene {

public:
    explicit PhysXScene(PhysicsManager* const manager) : Scene(manager) { }

    NODISCARD FORCEINLINE String getSceneName() const override { return "PhysXScene"; }

protected:
    void start() override {

        setWorldColor(Color(0, 0, 0).value);

        this->character = createActor<FlyingCharacter>({{-5.0f, 5.0f, 0.0f}, {0.0f, -45.0f, 0.0f}});

        this->skybox = ActorFactory::makeModelActor(this,
                                                    "SkyCube",
                                                    "",
                                                    {
                                                        {},
                                                        {90.0f, 0.0f, 0.0f},
                                                        Vector3(10000.0f)
                                                    });


        this->plane = ActorFactory::makeModelActor(this, "Plane", "", {
                                                       {0, -1, 0}, {-90.0f, 0.0f, 0.0f}, Vector3(10000.0f, 10000.0f, 1)
                                                   });

        this->light = ActorFactory::makeDirectionalLightActor(this,
                                                              {0.5f, -1.0f, 0.0f},
                                                              LinearColor::fromRGB(106, 77, 102));

        plane->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::unlit);
        plane->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColor::fromLinearRGB(
            0.8f, 0.8f, 0.8f);

        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setTexture(0, "Space");
        comp->setShader(0, ShaderPrograms::skyboxShader);

        this->model = ActorFactory::makeSphereCollisionActor(this, "SmoothSphere",
                                                             ""
                                                             , 1.0f,
                                                             ActorFactory::CollisionType::Dynamic,
                                                             Transform({0.0f, 0.0f, 0.0f}));
        model->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::materialShader);
        model->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::red;

        auto* physicsScene = this->getPhysicsScene();
        auto& physics = physicsScene->getPhysics();

        auto* material = physics.createMaterial(2.0f, 0.5f, 0.5f);


        physx::PxRigidStatic* groundPlane = PxCreatePlane(physics, physx::PxPlane(0, 1, 0, 1), *material);
        physicsScene->addActor(*groundPlane);


        InputManager* manager = Engine::getInstance()->getInputManager();

        manager->bindEvent(this, KeyBind(SDLK_1), EInputEvent::Pressed, &PhysXScene::setTexture0);
        manager->bindEvent(this, KeyBind(SDLK_2), EInputEvent::Pressed, &PhysXScene::setTexture1);
        manager->bindEvent(this, KeyBind(SDLK_3), EInputEvent::Pressed, &PhysXScene::setTexture2);
        manager->bindEvent(this, KeyBind(SDLK_4), EInputEvent::Pressed, &PhysXScene::setTexture3);

        manager->bindEvent(this, KeyBind(SDLK_5), EInputEvent::Pressed, &PhysXScene::imp);
        manager->bindEvent(this, KeyBind(SDLK_6), EInputEvent::Pressed, &PhysXScene::imp2);
        manager->bindEvent(this, KeyBind(SDLK_7), EInputEvent::Pressed, &PhysXScene::imp3);
        manager->bindEvent(this, KeyBind(SDLK_8), EInputEvent::Pressed, &PhysXScene::imp4);
        manager->bindEvent(this, KeyBind(SDLK_8), EInputEvent::Pressed, &PhysXScene::imp5);


        for (const auto& actor : this->registeredActors) { actor->start(); }

    }

    void imp() { model->getFirstComponentOf<PhysicsComponent>()->addImpulse({10, 0, 10}); }

    void imp2() { model->getFirstComponentOf<PhysicsComponent>()->addImpulse({-10, 0, 0}); }

    void imp3() { model->getFirstComponentOf<PhysicsComponent>()->addImpulse({0, 0, 10}); }

    void imp4() { model->getFirstComponentOf<PhysicsComponent>()->addImpulse({0, 0, -10}); }

    void imp5() { model->getFirstComponentOf<PhysicsComponent>()->addImpulse({0, 50, 0}); }

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

    void update(const float deltaTime) override { Scene::update(deltaTime); }

    Actor* skybox = nullptr;
    Actor* plane = nullptr;
    Actor* model = nullptr;
    FlyingCharacter* character = nullptr;
    Actor* light = nullptr;

};

class ModelScene : public Scene {

public:
    explicit ModelScene(PhysicsManager* const manager) : Scene(manager) { }

    void start() override {

        ActorFactory::makeModelActor(this, "marble_bust_01_2k", "", {{}, {-90.0f, 0.0f, 0.0f}});
        ActorFactory::makeDirectionalLightActor(this, {-0.5f, -1.0f, 0.0f}, LinearColors::white);
        createActor<FlyingCharacter>({ { 0.0f, 0.25f, 1.0f }, { -90.0f, 0.0f, 0.0f } });
    }

    NODISCARD FORCEINLINE String getSceneName() const override { return "ModelScene"; };
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

class DemoScene : public Scene {

public:
    NODISCARD FORCEINLINE String getSceneName() const override { return "DemoScene"; }

    explicit DemoScene(PhysicsManager* const manager) : Scene(manager) { }

protected:
    void start() override {

        setWorldColor(Color(10, 10, 10).value);

        this->planet1 = ActorFactory::makeModelActor(this,
                                                     "SmoothSphere",
                                                     "2k_earth_daymap",
                                                     {{}, {90.0f, 0.0f, 0.0f}});

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
        planet1->getFirstComponentOf<ModelComponent>()->setVisible(false);

        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setShader(0, ShaderPrograms::skyboxShader);
        comp->setVisible(false);

        InputManager* manager = Engine::getInstance()->getInputManager();

        manager->bindEvent(this, KeyBind(SDLK_1), EInputEvent::Pressed, &DemoScene::toggleVisible);
        manager->bindEvent(this, KeyBind(SDLK_2), EInputEvent::Pressed, &DemoScene::setWireframe);
        manager->bindEvent(this, KeyBind(SDLK_3), EInputEvent::Pressed, &DemoScene::setSkybox);
        manager->bindEvent(this, KeyBind(SDLK_4), EInputEvent::Pressed, &DemoScene::setLight);
        manager->bindEvent(this, KeyBind(SDLK_0), EInputEvent::Pressed, &DemoScene::setUnlit);

    }

    void setSkybox() {

        auto* model = this->skybox->getFirstComponentOf<ModelComponent>();
        model->setVisible(!model->isVisible());

    }

    void toggleVisible() {

        auto* model = this->planet1->getFirstComponentOf<ModelComponent>();
        model->setVisible(!model->isVisible());

    }

    void setLight() {

        planet1->getFirstComponentOf<ModelComponent>()->setModel("SmoothSphere");
        planet1->getFirstComponentOf<ModelComponent>()->setTexture(0, "2k_earth_daymap");
        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::materialShader);

        skybox->getFirstComponentOf<ModelComponent>()->setModel("Sphere");
        skybox->getFirstComponentOf<ModelComponent>()->setTexture(0, "2k_stars_milky_way");
        skybox->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::skyboxShader);

        light->getFirstComponentOf<LightComponent>()->setDiffuseColor(LinearColors::white);

    }

    void setUnlit() {

        planet1->getFirstComponentOf<ModelComponent>()->setModel("SmoothSphere");
        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::unlit);
        planet1->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;

    }

    void setWireframe() {

        planet1->getFirstComponentOf<ModelComponent>()->setModel("Sphere");
        planet1->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
        planet1->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;

        skybox->getFirstComponentOf<ModelComponent>()->setModel("Sphere");
        skybox->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::wireframeShader);
        skybox->getFirstComponentOf<ModelComponent>()->getMaterial(0).ambientColor = LinearColors::white;

    }

    void update(const float deltaTime) override {

        Scene::update(deltaTime);

        planet1->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});

        rot += 10.0f * deltaTime;

    }

    float rot = 0.0f;

    FlyingCharacter* character = nullptr;
    Actor* skybox = nullptr;
    Actor* light = nullptr;
    Actor* planet1 = nullptr;

};

class SolarSystem : public Scene {

public:
    NODISCARD FORCEINLINE String getSceneName() const override { return "SolarSystem"; }

    explicit SolarSystem(PhysicsManager* const manager) : Scene(manager) { }

protected:
    void start() override {

        setWorldColor(Color(10, 10, 10).value);

        this->earth = ActorFactory::makeModelActor(this,
                                                   "SmoothSphere",
                                                   "2k_earth_daymap",
                                                   Transform({12.30f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                             {0.50f, 0.50f, 0.50f}));

        this->moon = ActorFactory::makeModelActor(this,
                                                  "SmoothSphere",
                                                  "2k_moon",
                                                  Transform({13.0f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                            {0.05f, 0.05f, 0.05f}));

        this->sun = ActorFactory::makeModelActor(this,
                                                 "SmoothSphere",
                                                 "2k_sun",
                                                 Transform({0.0f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                           {2.0f, 2.0f, 2.0f}));

        this->mercury = ActorFactory::makeModelActor(this,
                                                     "SmoothSphere",
                                                     "2k_mercury",
                                                     Transform({4.58f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                               {0.10f, 0.10f, 0.10f}));

        this->venus = ActorFactory::makeModelActor(this,
                                                   "SmoothSphere",
                                                   "2k_venus_surface",
                                                   Transform({8.08f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                             {0.50f, 0.50f, 0.50f}));

        this->mars = ActorFactory::makeModelActor(this,
                                                  "SmoothSphere",
                                                  "2k_mars",
                                                  Transform({14.28f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                            {0.25f, 0.25f, 0.25f}));

        this->jupiter = ActorFactory::makeModelActor(this,
                                                     "SmoothSphere",
                                                     "2k_jupiter",
                                                     Transform({19.0f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                               {1.00f, 1.00f, 1.00f}));

        this->saturn = ActorFactory::makeModelActor(this,
                                                    "SmoothSphere",
                                                    "2k_saturn",
                                                    Transform({25.0f, 0.0f, 0.0f}, {90.0f, 0.0f, 0.0f},
                                                              {1.00f, 1.00f, 1.00f}));

        this->belt = ActorFactory::makeModelActor(this,
                                                  "SmoothSphere",
                                                  "2k_belt",
                                                  Transform({25.0f, 0.0f, 0.0f}, {}, {2.00f, 0.05f, 2.00f}));


        this->sun->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->mercury->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->venus->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->earth->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->moon->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->mars->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->jupiter->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->venus->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->saturn->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;
        this->belt->getFirstComponentOf<ModelComponent>()->getMaterial(0).specular = Vector4::zero;

        this->sun->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::unlit);

        //this->planet2->attachTo(planet1);
        //this->moon->attachTo(earth);
        this->belt->attachTo(saturn);
        this->moon->attachTo(earth);

        this->skybox = ActorFactory::makeModelActor(this,
                                                    "SkyCube",
                                                    "2k_stars_milky_way",
                                                    {{}, {0.0f, 0.0f, 0.0f}, Vector3(10000.0f)});

        //ActorFactory::makeDirectionalLightActor(this, {0.0f, -1.0f, 0.0f }, LinearColors::white);

        this->light = ActorFactory::makePointLightActor(this, {}, LinearColors::white);

        this->light->getFirstComponentOf<PointLightComponent>()->setSpecularColor(LinearColors::black);
        this->light->getFirstComponentOf<PointLightComponent>()->setLinear(0.0f);
        this->light->getFirstComponentOf<PointLightComponent>()->setConstant(1.0f);
        this->light->getFirstComponentOf<PointLightComponent>()->setQuadratic(0.0f);

        this->character = createActor<FlyingCharacter>({{-5.0f, 5.0f, 0.0f}, {0.0f, -45.0f, 0.0f}});

        earth->getFirstComponentOf<ModelComponent>()->setModel("SmoothSphere");
        earth->getFirstComponentOf<ModelComponent>()->setTexture(0, "2k_earth_daymap");
        earth->getFirstComponentOf<ModelComponent>()->setShader(0, ShaderPrograms::materialShader);
        light->getFirstComponentOf<LightComponent>()->setDiffuseColor(LinearColors::white);

        auto* comp = skybox->getFirstComponentOf<ModelComponent>();
        comp->setShader(0, ShaderPrograms::skyboxShader);

    }

    void update(const float deltaTime) override {

        Scene::update(deltaTime);

        const float time = Engine::getInstance()->getTime();

        earth->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});
        moon->addWorldRotation({0.0f, 25.0f * deltaTime, 0.0f});
        sun->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});
        mercury->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});
        venus->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});
        mars->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});
        jupiter->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});
        saturn->addWorldRotation({0.0f, 50.0f * deltaTime, 0.0f});

        //moon->getRootComponent()->rotateAround(earth->getWorldPosition(), earth->getForwardVector(), { 0.0f, 0.0f, 0.0f });
        earth->getRootComponent()->rotateAround({}, {1.0f, 0.0f, 0.0f}, {0.0f, rot2, 0.0f});
        mercury->getRootComponent()->rotateAround({}, {1.0f, 0.0f, 0.0f}, {0.0f, rot, 0.0f});
        venus->getRootComponent()->rotateAround({}, {1.0f, 0.0f, 0.0f}, {0.0f, rot1, 0.0f});
        mars->getRootComponent()->rotateAround({}, {1.0f, 0.0f, 0.0f}, {0.0f, rot3, 0.0f});
        jupiter->getRootComponent()->rotateAround({}, {1.0f, 0.0f, 0.0f}, {0.0f, rot4, 0.0f});
        saturn->getRootComponent()->rotateAround({}, {1.0f, 0.0f, 0.0f}, {0.0f, rot5, 0.0f});

        rot += 10.0f * deltaTime;
        rot1 += 8.0f * deltaTime;
        rot2 += 7.0f * deltaTime;
        rot3 += 6.0f * deltaTime;
        rot4 += 5.0f * deltaTime;
        rot5 += 4.0f * deltaTime;
        rot6 = +14.0f * deltaTime;

    }

    float rot = 0.0f;
    float rot1 = 0.0f;
    float rot2 = 0.0f;
    float rot3 = 0.0f;
    float rot4 = 0.0f;
    float rot5 = 0.0f;
    float rot6 = 0.0f;

    FlyingCharacter* character = nullptr;
    Actor* skybox = nullptr;
    Actor* light = nullptr;
    Actor* earth = nullptr;
    Actor* moon = nullptr;
    Actor* sun = nullptr;
    Actor* mercury = nullptr;
    Actor* venus = nullptr;
    Actor* mars = nullptr;
    Actor* jupiter = nullptr;
    Actor* saturn = nullptr;
    Actor* belt = nullptr;
    //Actor* light2 = nullptr;

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
                    else if (sceneName == "SolarSystem") { loadScene<SolarSystem>(); }
                    else if (sceneName == "DemoScene") { loadScene<DemoScene>(); }
                    else if (sceneName == "ModelScene") { loadScene<ModelScene>(); }
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

        Console::exec("scene.load DemoScene");

        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F1), EInputEvent::Pressed, &MyEngine::loadScene<DemoScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F2), EInputEvent::Pressed, &MyEngine::loadScene<SpaceScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F3), EInputEvent::Pressed, &MyEngine::loadScene<SolarSystem>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F4), EInputEvent::Pressed, &MyEngine::loadScene<SpotlightScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F5), EInputEvent::Pressed, &MyEngine::loadScene<ModelScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F6), EInputEvent::Pressed, &MyEngine::loadScene<RenderScene>);
        getInputManager()->bindEvent<MyEngine>(this, KeyBind(SDLK_F7), EInputEvent::Pressed, &MyEngine::loadScene<PhysXScene>);

    }

    void onUpdate() override { }

};

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
