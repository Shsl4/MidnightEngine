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
    
    void load() override {

        setWorldColor(0xc7efffff);

        createObject<MeshObject>(Transform({ 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 90.0f }, Vector3(5.0f)), "Donut");
        createObject<MeshObject>(Transform({ 5.0f, 0.0f, 0.0f }), "Lamp1");
        createObject<MeshObject>(Transform({ -5.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 90.0f }, Vector3(0.5f)), "Lamp1");
        createObject<MeshObject>(Transform({ 0.0f, 0.0f, -5.0f }), "Cube");

        createObject<FlyingCharacter>(Transform(Vector3(0.0, 0.0, 0.0f), Vector3(-90.0f, 0.0f, 0.0f)));

    }

};

class OtherScene : public Scene
{

    NODISCARD FORCEINLINE String getSceneName() const override
    {
        return "OtherScene";
    }
    
    void load() override {

        setWorldColor(0xffe18fff);

        for (int i = -5; i <= 5; i++)
        {
            createObject<MeshObject>(Transform({ 0.0f, 0.0f, 10.0f * i }, { 0.0f, 0.0f, 0.0f }), "Monkey");

        }

        createObject<FlyingCharacter>(Transform(Vector3(5.0, 0.0, 0.0f), Vector3(180.0f, 0.0f, 0.0f)));

    }
    
};

class MyEngine : public Engine {

    void load() {
        loadScene<MyScene>();
    }

    void load2() {
        loadScene<OtherScene>();
    }

    void onStart() override {
        
        getInputManager()->bindEvent(this->cast<Engine>(), KeyBind('u'), EInputEvent::Pressed, &MyEngine::unloadScene);
        getInputManager()->bindEvent(this, KeyBind('l'), EInputEvent::Pressed, &MyEngine::load);
        getInputManager()->bindEvent(this, KeyBind('o'), EInputEvent::Pressed, &MyEngine::load2);

    }

    void onUpdate() override {

    }

};

int main(int argc, const char** argv) {

#ifdef __APPLE__

    return macOS_main(argc, argv);

#else

#if defined(_WIN64) && defined(DEBUG_LEAKS)

    // Enables memory leak check on Windows
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

    const auto entry = ARPointer<Entry>::make();
    const auto engine = ARPointer<MyEngine>::make();
    const auto pointer = engine.raw();

    entry->entry(argc, argv, [pointer]() { return pointer; });

#endif

    return 0;

}