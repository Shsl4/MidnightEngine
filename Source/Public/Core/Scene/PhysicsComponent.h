#pragma once

#include <Scene/SceneComponent.h>
#include <Physics/PhysicsManager.h>

class ENGINE_API PhysicsComponent : public SceneComponent {
    
public:
    
    PhysicsComponent() = default;

    void start() override;

    void update(float deltaTime) override;

    void setup(Scene* scene) override;

    void addImpulse(Vector3 const& direction) const;

    void setEnableGravity(bool value) const;

    void setSimulatePhysics(bool value) const;

    NODISCARD bool isSimulatingPhysics() const;

    NODISCARD bool hasGravityEnabled() const;

    NODISCARD FORCEINLINE physx::PxRigidBody* getRigidBody() const { return rigidBody; }
    
private:

    physx::PxRigidBody* rigidBody = nullptr;
    
};
