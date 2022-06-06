﻿#include <Scene/PhysicsComponent.h>

#include <Scene/Scene.h>
#include <PhysX/PxRigidBody.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/extensions/PxSimpleFactory.h>

#include "Engine.h"
#include "extensions/PxDefaultStreams.h"
#include "extensions/PxRigidActorExt.h"
#include "extensions/PxRigidBodyExt.h"
#include "foundation/PxMat44.h"

using namespace physx;

void PhysicsComponent::start() {
    
    SceneComponent::start();
    
}

void PhysicsComponent::update(float deltaTime) {

    SceneComponent::update(deltaTime);

    if(!rigidBody) { return; }
    
    const auto p = rigidBody->getGlobalPose().p;
    const auto m = PxMat44(rigidBody->getGlobalPose());
    
    const float r00 = m.column0[0];
    const float r10 = m.column0[1];
    const float r20 = m.column0[2];
    const float r21 = m.column1[2];
    const float r22 = m.column2[2];

    const float rx = Math::toDegrees(atan2(r21, r22));
    const float ry = Math::toDegrees(atan2(-r20, sqrt(r21 * r21 + r22 * r22)));
    const float rz = Math::toDegrees(atan2(r10, r00));
    
    const Vector3 position = { p.x ,p.y, p.z };
    const Vector3 rotation = { rx, ry, rz };
    
    this->setWorldPosition(position);
    this->setWorldRotation(rotation);
    
}

void PhysicsComponent::setup(Scene* scene) { 

    auto* physicsScene = scene->getPhysicsScene();
    auto& physics = physicsScene->getPhysics();

    if(!material) {
        PhysicsComponent::material = physics.createMaterial(2.0f, 0.5f, 0.5f);
    }    
    
}

void PhysicsComponent::addImpulse(Vector3 const& direction) const {

    if(!isSimulatingPhysics()) { return; }
    
    const PxVec3 pxVector = { direction.x, direction.y, direction.z };
    rigidBody->addForce(pxVector, PxForceMode::eIMPULSE);

}

void PhysicsComponent::setEnableGravity(bool value) const {

    rigidBody->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, !value);
    
}

void PhysicsComponent::setSimulatePhysics(bool value) const { 
    
    rigidBody->setActorFlag(PxActorFlag::eDISABLE_SIMULATION, !value);
    setEnableGravity(value);
    
}

bool PhysicsComponent::isSimulatingPhysics() const {
    
    return !rigidBody->getActorFlags().isSet(PxActorFlag::eDISABLE_SIMULATION);

}

bool PhysicsComponent::hasGravityEnabled() const {

    return !rigidBody->getActorFlags().isSet(PxActorFlag::eDISABLE_GRAVITY);
    
}

void PhysicsComponent::makeSphereCollider(float radius) {

    const auto* scene = getScene();
    auto* physicsScene = scene->getPhysicsScene();
    auto& physics = physicsScene->getPhysics();
    
    const auto geometry = physx::PxSphereGeometry(radius);

    Vector3 position = getWorldPosition();
    
    rigidBody = PxCreateDynamic(physics, PxTransform(*reinterpret_cast<PxVec3*>(&position)), geometry , *material, 10.0f);
        
    rigidBody->setAngularDamping(0.5f);
    rigidBody->setLinearVelocity(PxVec3(0, 0, 0));
    rigidBody->setMass(10.0f);
    PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
    physicsScene->addActor(*rigidBody);
        
}

void PhysicsComponent::makeDynamicBoxCollider(Vector3 const& halfExtents) {

    const auto* scene = getScene();
    auto* physicsScene = scene->getPhysicsScene();
    auto& physics = physicsScene->getPhysics();
    
    const Vector3 position = getWorldPosition();
    const auto tr = PxTransform(PxVec3(position.x, position.y, position.z));
    const auto geometry = PxBoxGeometry(halfExtents.x, halfExtents.y, halfExtents.z);
    rigidBody = PxCreateDynamic(physics, tr, geometry, *material, 10.0f);
    
    rigidBody->setAngularDamping(0.5f);
    rigidBody->setLinearVelocity(PxVec3(0, 0, 0));
    rigidBody->setMass(10.0f);
    
    PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
    
    physicsScene->addActor(*rigidBody);
    
}

bool PhysicsComponent::makeModelCollider(const Model* model) {

    const auto* scene = getScene();
    auto* physicsScene = scene->getPhysicsScene();
    auto& physics = physicsScene->getPhysics();
    
    const auto mesh = model->getMesh(0);

    auto positions = Array<Vector3>(mesh->vertexCount);
    auto indices = Array<UInt16>(mesh->indexCount);

    for (size_t i = 0; i < mesh->vertexCount; i++) {
        positions += mesh->data[i].position;
    }
    
    for (size_t i = 0; i < mesh->indexCount; i++) {
        indices += mesh->indices[i];
    }
    
    PxConvexMeshDesc convexDesc;
    convexDesc.points.count     = mesh->vertexCount;
    convexDesc.points.stride    = sizeof(Vector3);
    convexDesc.points.data      = positions.begin();
    convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX;
    
    PxDefaultMemoryOutputStream buf;
    PxConvexMeshCookingResult::Enum result;
    
    const auto cooking = Engine::getInstance()->getPhysicsManager()->getCooking();
    
    cooking->cookConvexMesh(convexDesc, buf, &result);

    if (result != PxConvexMeshCookingResult::eSUCCESS) {
        Console::getLogger()->error("Failed to cook convex mesh.");
        return false;
    }
    
    PxConvexMesh* convexMesh = cooking->createConvexMesh(convexDesc, physics.getPhysicsInsertionCallback());

    const auto convexGeometry = PxConvexMeshGeometry(convexMesh);

    const Vector3 position = getWorldPosition();
    const auto tr = PxTransform(PxVec3(position.x, position.y, position.z));

    rigidBody = PxCreateDynamic(physics, tr, convexGeometry, *material, 10.0f);
        
    rigidBody->setAngularDamping(0.5f);
    rigidBody->setLinearVelocity(PxVec3(0, 0, 0));
    rigidBody->setMass(10.0f);
    
    PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
    
    physicsScene->addActor(*rigidBody);

    return true;
   
}
