#pragma once

#include <PhysX/PxFoundation.h>
#include <PhysX/PxPhysics.h>

#include <Physics/PhysXAllocator.h>
#include <Physics/PhysXErrorCallback.h>
#include <PhysX/common/PxTolerancesScale.h>
#include <PhysX/extensions/PxDefaultCpuDispatcher.h>
#include <PhysX/pvd/PxPvd.h>
#include <PhysX/pvd/PxPvdTransport.h>
#include <PhysX/cooking/PxCooking.h>

#include <Core/Object.h>


class ENGINE_API PhysicsManager : public Object {
      
public:

    void init();
    void destroy() const;

    FORCEINLINE physx::PxFoundation* getFoundation() const { return foundation; }
    FORCEINLINE physx::PxPhysics* getPhysics() const { return physics; }
    FORCEINLINE physx::PxCpuDispatcher* getCPUDispatcher() const { return dispatcher; }
    FORCEINLINE physx::PxMaterial* getPhysicsMaterial() const { return material; }
    FORCEINLINE physx::PxCooking* getCooking() const { return cooking; }

    static void updatePvdClient(physx::PxScene* scene);

    ~PhysicsManager() override;

private:
    /*
    PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity=PxVec3(0));
    void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);
*/
    physx::PxFoundation* foundation = nullptr;
    physx::PxPhysics* physics = nullptr;
    physx::PxDefaultCpuDispatcher* dispatcher = nullptr;
    physx::PxPvd* pvd = nullptr;
    physx::PxMaterial* material = nullptr;
    physx::PxCooking* cooking = nullptr;
    physx::PxTolerancesScale scale;

    PhysXAllocator allocator;
    PhysXErrorCallback errorCallback;
    
};
