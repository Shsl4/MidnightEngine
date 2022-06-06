#pragma once

#include <Scene/Actor.h>
#include <Scene/Scene.h>
#include <Scene/SceneComponent.h>
#include <Scene/PointLightComponent.h>
#include <Scene/DirectionalLightComponent.h>
#include <Scene/ModelComponent.h>
#include <Scene/PhysicsComponent.h>
#include <Rendering/LinearColor.h>

class ActorFactory {

public:

    enum class CollisionType {

        Static = 0,
        Dynamic = 1

    };

    static Actor* makeCameraActor(Scene* scene, Transform const& transform = Transform());

    static Actor* makePointLightActor(Scene* scene, Vector3 const& position, LinearColor const& color);

    static Actor* makeDirectionalLightActor(Scene* scene, Vector3 const& direction, LinearColor const& color);

    static Actor* makeModelActor(Scene* scene, String const& modelPath, Vector3 const& position = Vector3::zero,
                                 Vector3 const& rotation = Vector3::zero, Vector3 const& scale = Vector3::one);

    static Actor* makeSphereCollisionActor(Scene* scene, String const& modelPath, float sphereRadius = 1.0f,
                                           CollisionType type = CollisionType::Dynamic,
                                           Transform const& transform = Transform());

    static Actor* makeBoxCollisionActor(Scene* scene, String const& modelPath,
                                        Vector3 const& halfExtents = Vector3(1.0f),
                                        CollisionType type = CollisionType::Dynamic,
                                        Transform const& transform = Transform());

    static Actor* makeCapsuleCollisionActor(Scene* scene, String const& modelPath, float capsuleRadius = 1.0f,
                                            float capsuleHalfHeight = 1.0f,
                                            CollisionType type = CollisionType::Dynamic,
                                            Transform const& transform = Transform());

    static Actor* makeStaticMeshCollider(Scene* scene, String const& modelPath, Transform const& transform = Transform());

};