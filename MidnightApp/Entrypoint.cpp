//
// Created by sl4sh on 26/04/22.
//

#include <Core/Core.h>
#include <Console/Console.h>
#include <Memory/WeakPointer.h>
#include <Platform/Entry.h>

#include <Scene/MeshObject.h>
#include <Scene/FlyingCharacter.h>
#include <Scene/MeshComponent.h>

#include "Rendering/ShaderPrograms.h"

class LightObject : public SceneObject {
    
public:
    
    void createComponents(Scene* scene, Transform transform) override {

        light = scene->createComponent<BasicLightComponent>(transform);
        setRootComponent(light.raw());
        
        
    }

    void update(float deltaTime) override {

        SceneObject::update(deltaTime);
        
        getRootComponent()->rotateAround(Vector3::zero, { 0.0f, 1.0f, 0.0f }, rotation);
        
        rotation += { 50.0f * deltaTime, 0.0f, 0.0f};
        
    }

private:

    WeakPointer<BasicLightComponent> light;
    Vector3 rotation = Vector3::zero;
    
};

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
        createObject<LightObject>(Transform({ 0.0f, 5.0f, 0.0f }));

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

        setWorldColor(Color(10, 10, 10).value);

        this->planet1 = createObject<MeshObject>(Transform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(1.0f)), "Sphere");
        this->planet2 = createObject<MeshObject>(Transform({ 3.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(0.25f)), "Sphere");
        this->planet3 = createObject<MeshObject>(Transform({ 4.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, Vector3(0.125f)), "Sphere");
        createObject<LightObject>(Transform({ 0.0f, 5.0f, 0.0f }, {}, { 0.25f, 0.25f, 0.25f }));

        this->character = createObject<FlyingCharacter>({ { -5.0f, 5.0f, 0.0f }, { 0.0f, -45.0f, 0.0f } });

        planet2->attachTo(planet1.raw());
        planet3->attachTo(planet2.raw());

    }
    
    void update(const float deltaTime) override {

        Scene::update(deltaTime);

        if(planet1.valid()) {
            planet1->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        }

        if(planet2.valid()) {
            planet2->addWorldRotation({ 0.0f, 50.0f * deltaTime, 0.0f });
        }        
        
    }
        
    WeakPointer<FlyingCharacter> character = nullptr;
    
    WeakPointer<MeshObject> planet1 = nullptr;
    WeakPointer<MeshObject> planet2 = nullptr;
    WeakPointer<MeshObject> planet3 = nullptr;
    
};

class RenderScene : public Scene
{
public:
    
    void start() override {

        setWorldColor(Color(10, 10, 10).value);
        createObject<MeshObject>(Transform({ 0.0f, 0.0f, 0.0f }), "Cube");
        createObject<FlyingCharacter>({ { 2.5f, 2.5f, 2.5f }, { -135.0f, -40.0f, 0.0f } });
        createObject<LightObject>(Transform({ 0.0f, 10.0f, 0.0f }));
        
    }
    
    NODISCARD FORCEINLINE String getSceneName() const override
    {
        return "RenderScene";
    }
};

class MyEngine : public Engine {

    void load() {
        loadScene<MyScene>();
    }

    void load2() {
        loadScene<SpaceScene>();
    }

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
                    else if(sceneName == "MyScene")
                    {
                        loadScene<MyScene>();
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
        
        auto* node = addMeshNode->addArgument("meshName", ArgumentType::String);
        
        node->addExecutable([this](const auto* context) {

            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

            String meshName = context->getString("meshName");
                       
            getActiveScene()->createObject<MeshObject>(Transform(), meshName);                       
                                              
        });

        node->addArgument("x", ArgumentType::Double)
            ->addArgument("y", ArgumentType::Double)
            ->addArgument("z", ArgumentType::Double)
            ->addExecutable([this](const auto* context) {

                CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

                String meshName = context->getString("meshName");
                const auto x = static_cast<float>(context->getDouble("x"));
                const auto y = static_cast<float>(context->getDouble("y"));
                const auto z = static_cast<float>(context->getDouble("z"));
                       
                getActiveScene()->createObject<MeshObject>(Transform(Vector3(x, y, z)), meshName);                       
                                              
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
                         
                         getActiveScene()->destroyObject(getActiveScene()->getObjectByIndex(index));
                         
                     });

        posNode->addArgument("index", ArgumentType::Int64)->addExecutable([this](const auto* context)
        {

            CommandError::throwIf(!getActiveScene().valid(), "No scene is loaded.");

            const Int64 index = context->getInt64("index");

            const SceneObject* object = getActiveScene()->getObjectByIndex(index);

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
        
        Console::exec("scene.load SpaceScene");

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

    const auto entry = UniquePointer<Entry>::make();
    const auto engine = UniquePointer<MyEngine>::make();
    const auto pointer = engine.raw();
    
    entry->entry(argc, argv, [pointer]() {
        return pointer;
    });


    return 0;

}
