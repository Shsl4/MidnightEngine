#include <Scene/PhysicsComponent.h>

#include <Scene/Scene.h>
#include <PhysX/PxRigidBody.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/extensions/PxSimpleFactory.h>

#include "Engine.h"
#include "extensions/PxDefaultStreams.h"
#include "extensions/PxRigidBodyExt.h"
#include "foundation/PxMat44.h"

using namespace physx;

void PhysicsComponent::start() {
    
    SceneComponent::start();
    
}

void PhysicsComponent::update(float deltaTime) {

    SceneComponent::update(deltaTime);

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
    auto* material = physics.createMaterial(2.0f, 0.5f, 0.5f);
    
    const auto geometry = physx::PxSphereGeometry(1.0f);
        
    rigidBody = PxCreateDynamic(physics, PxTransform(PxVec3(0, 0, 0)), geometry , *material, 10.0f);
    
/*
    const auto model = Engine::getInstance()->getResourceLoader()->getModel("Sphere");
    const auto mesh = model->getMesh(0);

    Vector3* a = new Vector3[mesh->vertexCount];

    for (size_t i = 0; i < mesh->vertexCount; i++) {
        a[i] = mesh->data[i].position;
    }
    
    PxConvexMeshDesc convexDesc;
    convexDesc.points.count     = mesh->vertexCount;
    convexDesc.points.stride    = sizeof(Vector3);
    convexDesc.points.data      = a;
    convexDesc.flags            = PxConvexFlag::eCOMPUTE_CONVEX;

    PxDefaultMemoryOutputStream buf;
    PxConvexMeshCookingResult::Enum result;
    const auto cooking = Engine::getInstance()->getPhysicsManager()->getCooking();
    
    expect(cooking->cookConvexMesh(convexDesc, buf, &result), "Failed to cook convex mesh.");

    PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    PxConvexMesh* convexMesh = physics.createConvexMesh(input);

    Engine::getInstance()->getPhysicsManager()->getCooking()->createConvexMesh(convexDesc, *material, &convexMesh);
    PxConvexMesh* aConvexMesh = scene->getPhysicsScene()->getPhysics().getcreateConvexMesh(convexDesc,
    scene->getPhysicsScene()->getPhysics().getPhysicsInsertionCallback());
*/
    
    rigidBody->setAngularDamping(0.5f);
    rigidBody->setLinearVelocity(PxVec3(0, 0, 0));
    rigidBody->setMass(10.0f);
    PxRigidBodyExt::updateMassAndInertia(*rigidBody, 10.0f);
    physicsScene->addActor(*rigidBody);
    

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
