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

    static Actor* makeDirectionalLightActor(Scene* scene, Vector3 const& direction, LinearColor const& color);

    static Actor* makePointLightActor(Scene* scene, Vector3 const& position, LinearColor const& color);

    static Actor* makeSpotLightActor(Scene* scene,
                                     Vector3 const& position = {},
                                     Vector3 const& direction = {0.0f, -1.0f, 0.0f},
                                     LinearColor const& color = LinearColors::white);


    static Actor* makeModelActor(Scene* scene,
                                 String const& modelName,
                                 String const& textureName = "",
                                 Transform const& transform = Transform());

    static Actor* makeSphereCollisionActor(Scene* scene,
                                           String const& modelName,
                                           String const& textureName = "",
                                           float sphereRadius = 1.0f,
                                           CollisionType type = CollisionType::Dynamic,
                                           Transform const& transform = Transform());

    static Actor* makeBoxCollisionActor(Scene* scene,
                                        String const& modelName,
                                        String const& textureName = "",
                                        Vector3 const& halfExtents = Vector3(1.0f),
                                        CollisionType type = CollisionType::Dynamic,
                                        Transform const& transform = Transform());

    static Actor* makeStaticMeshCollider(Scene* scene,
                                         String const& modelName,
                                         String const& textureName = "",
                                         Transform const& transform = Transform());

};
