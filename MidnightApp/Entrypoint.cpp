//
// Created by sl4sh on 26/04/22.
//

#include <Core/Core.h>
#include <Platform/Entry.h>

#include <Scene/MeshObject.h>
#include <Scene/FlyingCharacter.h>

class MyScene : public Scene {
    
    NODISCARD FORCEINLINE String getSceneName() const override
    {
        return "MyScene";
    }
    
    void start() override {

        setWorldColor(0xc7efffff);

        createObject<MeshObject>(Transform({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 90.0f }, Vector3(5.0f)), "Donut");
        createObject<MeshObject>(Transform({ 5.0f, 0.0f, 0.0f }), "Lamp1");
        createObject<MeshObject>(Transform({ -5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 90.0f }, Vector3(0.5f)), "Lamp1");
        createObject<MeshObject>(Transform({ 0.0f, 0.0f, -5.0f }), "Cube");

        createObject<FlyingCharacter>(Transform(Vector3(0.0, 0.0, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)));

    }

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

        setWorldColor(0x101010ff);

        this->planet1 = createObject<MeshObject>(Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(1.0f)), "Sphere");
        this->planet2 = createObject<MeshObject>(Transform({ 3.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(0.25f)), "Sphere");
        this->planet3 = createObject<MeshObject>(Transform({ 4.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(0.125f)), "Sphere");
        
        this->character = createObject<FlyingCharacter>({ { -5.0f, 5.0f, 0.0f }, { 0.0f, -45.0f, 0.0f } });

        planet2->attachTo(planet1);
        planet3->attachTo(planet2);

    }
    
    void update(const float deltaTime) override {

        Scene::update(deltaTime);
     
        planet1->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        planet2->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        
    }

    FlyingCharacter* character = nullptr;
    MeshObject* planet1 = nullptr;
    MeshObject* planet2 = nullptr;
    MeshObject* planet3 = nullptr;
    
};

class MyEngine : public Engine {

    void load() {
        loadScene<MyScene>();
    }

    void load2() {
        loadScene<SpaceScene>();
    }

    void onStart() override {
        
        CommandNode* stopNode = CommandNode::make("exit");
        CommandNode* unloadNode = CommandNode::make("scene.unload");

        stopNode->setExecutable([this](const auto* node) {
            stop();
            return CommandResult::Success;
            });

        unloadNode->setExecutable([this](const auto* node) {
            unloadScene();
            return CommandResult::Success;
            });

        getConsole()->registerCommand(stopNode);
        getConsole()->registerCommand(unloadNode);

        getInputManager()->bindEvent(this->cast<Engine>(), KeyBind('u'), EInputEvent::Pressed, &MyEngine::unloadScene);
        getInputManager()->bindEvent(this, KeyBind('l'), EInputEvent::Pressed, &MyEngine::load);
        getInputManager()->bindEvent(this, KeyBind('o'), EInputEvent::Pressed, &MyEngine::load2);

    }

    void onUpdate() override {

    }

};

int main(const int argc, const char** argv) {

#if defined(_WIN64) && defined(DEBUG_LEAKS)

    // Enables memory leak check on Windows
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    const auto entry = AutoReleasePointer<Entry>::make();
    const auto engine = AutoReleasePointer<MyEngine>::make();
    const auto pointer = engine.raw();

    entry->entry(argc, argv, [pointer]()
    {
        return pointer;
    });

    return 0;

}
