#include <Physics/PhysicsManager.h>

#include <Exception/Exception.h>

#include <PhysX/PxPhysicsVersion.h>
#include <PhysX/PxMaterial.h>
#include <PhysX/PxRigidDynamic.h>
#include <PhysX/PxScene.h>
#include <PhysX/extensions/PxDefaultSimulationFilterShader.h>
#include <PhysX/extensions/PxSimpleFactory.h>
#include <PhysX/PxSceneDesc.h>
#include <PhysX/pvd/PxPvdSceneClient.h>

#include "extensions/PxRigidBodyExt.h"

using namespace physx;

void PhysicsManager::init() {

    if (foundation) {
        expect(false, "PhysicsManager is already initialized!");
    }

    foundation = PxCreateFoundation(PX_PHYSICS_VERSION, allocator, errorCallback);

    expect(foundation, "Failed to create PhysX foundation");

    cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(scale));
    
    expect(cooking, "Failed to create PhysX cooking");
    
    scale.length = 100;
    scale.speed = 981;
    
    pvd = PxCreatePvd(*foundation);

    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);

    pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
    
    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, scale, true, pvd);

    expect(physics, "Failed to create PhysX physics");
    
    dispatcher = PxDefaultCpuDispatcherCreate(2);
    
    material = physics->createMaterial(0.5f, 0.5f, 0.5f);
    
}

void PhysicsManager::destroy() const {

    if(physics) {
        physics->release();
    }

    if(cooking) {
        cooking->release();
    }
    
    if(foundation) {
        foundation->release();
    }    
    
}

void PhysicsManager::updatePvdClient(PxScene* scene) {
    
    if (PxPvdSceneClient* pvdClient = scene->getScenePvdClient()) {

        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        
    }
    
}

PhysicsManager::~PhysicsManager() {
    
    destroy();
    
}
