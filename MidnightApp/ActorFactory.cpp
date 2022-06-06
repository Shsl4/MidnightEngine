//
// Created by isl4s on 6/6/2022.
//

#include <ActorFactory.h>

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

Actor *ActorFactory::makeDirectionalLightActor(Scene *scene, const Vector3 &direction, const LinearColor &color) {

    auto* actor = scene->createActor<Actor>();
    auto* comp = actor->createComponent<DirectionalLightComponent>("DirectionalLight");
    comp->setLightDirection(direction);
    comp->setDiffuseColor(color);
    comp->attachTo(actor);

    return actor;

}

Actor *ActorFactory::makeCapsuleCollisionActor(Scene *scene, const String &modelPath, float capsuleRadius,
                                               float capsuleHalfHeight, ActorFactory::CollisionType type,
                                               const Transform &transform) {

    auto* actor = scene->createActor<Actor>();
    auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
    auto* model = actor->createComponent<ModelComponent>("Model");

    actor->setRootComponent(physicsComp);
    model->attachTo(physicsComp);

    actor->setWorldTransform(transform);

    model->setModel(modelPath);

    if (type == CollisionType::Dynamic){
        physicsComp->makeDynamicCapsuleCollider(capsuleRadius, capsuleHalfHeight);
    }
    else{
        physicsComp->makeStaticCapsuleCollider(capsuleRadius, capsuleHalfHeight);
    }

    return actor;

}

Actor *ActorFactory::makeBoxCollisionActor(Scene *scene, const String &modelPath, const Vector3 &halfExtents,
                                           ActorFactory::CollisionType type, const Transform &transform) {

    auto* actor = scene->createActor<Actor>();
    auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
    auto* model = actor->createComponent<ModelComponent>("Model");

    actor->setRootComponent(physicsComp);
    model->attachTo(physicsComp);

    actor->setWorldTransform(transform);

    model->setModel(modelPath);

    if (type == CollisionType::Dynamic) {
        physicsComp->makeDynamicBoxCollider(halfExtents);
        auto* destroyer = actor->createComponent<AutoDestroyComponent>("Destroyer");
        destroyer->attachTo(actor);
    }
    else{
        physicsComp->makeStaticBoxCollider(halfExtents);
    }

    return actor;

}

Actor *ActorFactory::makeSphereCollisionActor(Scene *scene, const String &modelPath, float sphereRadius,
                                              ActorFactory::CollisionType type, const Transform &transform) {

    auto* actor = scene->createActor<Actor>();
    auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
    auto* model = actor->createComponent<ModelComponent>("Model");

    actor->setRootComponent(physicsComp);
    model->attachTo(physicsComp);

    actor->setWorldTransform(transform);

    model->setModel(modelPath);

    if (type == CollisionType::Dynamic) {
        physicsComp->makeDynamicSphereCollider(sphereRadius);
        auto* destroyer = actor->createComponent<AutoDestroyComponent>("Destroyer");
        destroyer->attachTo(actor);
    }
    else{
        physicsComp->makeStaticSphereCollider(sphereRadius);
    }

    return actor;

}

Actor *ActorFactory::makeModelActor(Scene *scene, const String &modelPath, const Vector3 &position,
                                    const Vector3 &rotation, const Vector3 &scale) {

    auto* actor = scene->createActor<Actor>(Transform(position, rotation, scale));
    auto* comp = actor->createComponent<ModelComponent>("Model");
    comp->setModel(modelPath);
    comp->attachTo(actor);
    comp->setWorldTransform({ position, rotation, scale });

    return actor;

}

Actor *ActorFactory::makeStaticMeshCollider(Scene *scene, const String &modelPath, Transform const& transform) {

    auto* actor = scene->createActor<Actor>();
    auto* physicsComp = actor->createComponent<PhysicsComponent>("Collision");
    auto* model = actor->createComponent<ModelComponent>("Model");

    actor->setRootComponent(physicsComp);
    model->attachTo(physicsComp);

    actor->setWorldTransform(transform);

    bool result = model->setModel(modelPath);

    if(result){
        physicsComp->makeModelCollider(model->getModel().raw());
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
