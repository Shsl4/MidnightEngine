//
// Created by isl4s on 6/6/2022.
//

#include <ActorFactory.h>

#include "Scene/SpotLightComponent.h"

class AutoDestroyComponent : public SceneComponent{

public:

    void start() override {

    }

    void update(float deltaTime) override {

        Actor* parent = getParentActor();

        if (parent->getWorldPosition().y < -200.0f){

            parent->getScene()->destroyActor(parent);

        }

    }

};

Actor *ActorFactory::makePointLightActor(Scene *scene, const Vector3 &position, const LinearColor &color) {

    auto* actor = scene->createActor<Actor>(Transform(position));
    auto* comp = actor->createComponent<PointLightComponent>("PointLight");
    comp->setDiffuseColor(color);
    comp->attachTo(actor);

    return actor;

}

Actor* ActorFactory::makeSpotLightActor(Scene* scene, Vector3 const& position, Vector3 const& direction, LinearColor const& color) {
    
    auto* actor = scene->createActor<Actor>();
    auto* comp = actor->createComponent<SpotLightComponent>("SpotLight");
    actor->setRootComponent(comp);
    actor->setWorldPosition(position);
    comp->setLightDirection(direction);
    comp->setDiffuseColor(color);

    return actor;
    
}

Actor *ActorFactory::makeDirectionalLightActor(Scene *scene, const Vector3 &direction, const LinearColor &color) {

    auto* actor = scene->createActor<Actor>();
    auto* comp = actor->createComponent<DirectionalLightComponent>("DirectionalLight");
    comp->setLightDirection(direction);
    comp->setDiffuseColor(color);
    comp->attachTo(actor);

    return actor;

}

Actor *ActorFactory::makeBoxCollisionActor(Scene *scene, const String &modelName, String const& textureName, const Vector3 &halfExtents,
                                           ActorFactory::CollisionType type, const Transform &transform) {

    auto* actor = scene->createActor<Actor>();
    auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
    auto* model = actor->createComponent<ModelComponent>("Model");

    actor->setRootComponent(physicsComp);
    model->attachTo(physicsComp);

    actor->setWorldTransform(transform);

    model->setModel(modelName);

    if (type == CollisionType::Dynamic) {
        physicsComp->makeDynamicBoxCollider(halfExtents);
        auto* destroyer = actor->createComponent<AutoDestroyComponent>("Destroyer");
        destroyer->attachTo(actor);
    }
    else{
        physicsComp->makeStaticBoxCollider(halfExtents);
    }

    if(model->getModel().valid() && !textureName.isEmpty()) {
        model->setTexture(0, textureName);
        model->getMaterial(0).ambientColor = LinearColors::white;
    }

    return actor;

}

Actor *ActorFactory::makeSphereCollisionActor(Scene *scene, const String &modelName, String const& textureName, float sphereRadius,
                                              ActorFactory::CollisionType type, const Transform &transform) {

    auto* actor = scene->createActor<Actor>();
    auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
    auto* model = actor->createComponent<ModelComponent>("Model");

    actor->setRootComponent(physicsComp);
    model->attachTo(physicsComp);

    actor->setWorldTransform(transform);

    model->setModel(modelName);

    if (type == CollisionType::Dynamic) {
        physicsComp->makeDynamicSphereCollider(sphereRadius);
        auto* destroyer = actor->createComponent<AutoDestroyComponent>("Destroyer");
        destroyer->attachTo(actor);
    }
    else{
        physicsComp->makeStaticSphereCollider(sphereRadius);
    }

    if(model->getModel().valid() && !textureName.isEmpty()) {
        model->setTexture(0, textureName);
        model->getMaterial(0).ambientColor = LinearColors::white;
    }

    return actor;

}

Actor *ActorFactory::makeModelActor(Scene *scene, const String &modelName, String const& textureName, Transform const& transform) {

    auto* actor = scene->createActor<Actor>(transform);
    auto* model = actor->createComponent<ModelComponent>("Model");
    model->setModel(modelName);
    model->attachTo(actor);
    model->setWorldTransform(transform);

    if(model->getModel().valid() && !textureName.isEmpty()) {
        model->setTexture(0, textureName);
        model->getMaterial(0).ambientColor = LinearColors::white;
    }
    
    return actor;

}

Actor *ActorFactory::makeStaticMeshCollider(Scene *scene, const String &modelName, String const& textureName, Transform const& transform) {

    auto* actor = scene->createActor<Actor>();
    auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
    auto* model = actor->createComponent<ModelComponent>("Model");

    actor->setRootComponent(physicsComp);
    model->attachTo(physicsComp);

    actor->setWorldTransform(transform);

    bool result = model->setModel(modelName);

    if(result){
        physicsComp->makeModelCollider(model->getModel().raw());
    }

    if(model->getModel().valid() && !textureName.isEmpty()) {
        model->setTexture(0, textureName);
        model->getMaterial(0).ambientColor = LinearColors::white;
    }
    
    return actor;

}

Actor *ActorFactory::makeCameraActor(Scene *scene, const Transform &transform) {

    auto* actor = scene->createActor<Actor>();
    auto* camera = actor->createComponent<CameraComponent>("Camera");
    actor->setRootComponent(camera);

    actor->setWorldTransform(transform);

    return actor;

}
