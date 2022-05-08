//
// Created by sl4sh on 26/04/22.
//

#include <Core/Core.h>
#include <Platform/Entry.h>

#include <Scene/MeshObject.h>
#include <Scene/FlyingCharacter.h>

#include "SDL2/SDL_egl.h"

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

        if(planet1) {
            planet1->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        }

        if(planet2) {
            planet2->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        }        
        
    }

    bool destroyObject(SceneObject *object) override
    {

        if(object == planet1)
        {
            planet1 = nullptr;
        }

        if(object == planet2)
        {
            planet2 = nullptr;
        }

        if(object == planet3)
        {
            planet3 = nullptr;
        }
        
        return Scene::destroyObject(object);
        
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

        CommandNode* unloadNode = CommandNode::make("scene.unload");
        CommandNode* loadNode = CommandNode::make("scene.load");
        CommandNode* addMeshNode = CommandNode::make("scene.mesh.add");
        CommandNode* listObjNode = CommandNode::make("scene.object.list");
        CommandNode* deleteObjNode = CommandNode::make("scene.object.delete");
        CommandNode* listCompNode = CommandNode::make("scene.component.list");

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
                    else if(sceneName == "MyScene")
                    {
                        loadScene<MyScene>();
                    }
                    else
                    {
                        CommandError::throwError("The scene \"{}\" does not exist.", sceneName);
                    }                    
                    
                });

        auto* node = addMeshNode->addArgument("meshName", ArgumentType::String);
        
        node->addExecutable([this](const auto* context) {

            CommandError::throwIf(!getActiveScene(), "No scene is loaded.");

            String meshName = context->getString("meshName");
                       
            getActiveScene()->createObject<MeshObject>(Transform(), meshName);                       
                                              
        });

        node->addArgument("x", ArgumentType::Double)
            ->addArgument("y", ArgumentType::Double)
            ->addArgument("z", ArgumentType::Double)
            ->addExecutable([this](const auto* context) {

                CommandError::throwIf(!getActiveScene(), "No scene is loaded.");

                String meshName = context->getString("meshName");
                const auto x = static_cast<float>(context->getDouble("x"));
                const auto y = static_cast<float>(context->getDouble("y"));
                const auto z = static_cast<float>(context->getDouble("z"));
                       
                getActiveScene()->createObject<MeshObject>(Transform(Vector3(x, y, z)), meshName);                       
                                              
            });

        listObjNode->addExecutable([this](const auto* context)
        {
            CommandError::throwIf(!getActiveScene(), "No scene is loaded.");
            getActiveScene()->listObjects();            
        });
        
        listCompNode->addExecutable([this](const auto* context)
        {
            CommandError::throwIf(!getActiveScene(), "No scene is loaded.");
            getActiveScene()->listComponents();            
        });

        deleteObjNode->addArgument("index", ArgumentType::Int64)
                     ->addExecutable([this](const auto* context) {

                         CommandError::throwIf(!getActiveScene(), "No scene is loaded.");

                         const Int64 index = context->getInt64("index");
                         
                         getActiveScene()->destroyObject(getActiveScene()->getObjectByIndex(index));
                         
                     });
        
        getConsole()->registerCommand(loadNode);
        getConsole()->registerCommand(unloadNode);
        getConsole()->registerCommand(addMeshNode);
        getConsole()->registerCommand(listObjNode);
        getConsole()->registerCommand(deleteObjNode);
        getConsole()->registerCommand(listCompNode);

        getConsole()->execute("scene.load SpaceScene");

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
