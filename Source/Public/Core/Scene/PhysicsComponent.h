#pragma once

#include <Scene/SceneComponent.h>
#include <Physics/PhysicsManager.h>

class ENGINE_API PhysicsComponent : public SceneComponent {
    
public:
    
    PhysicsComponent() = default;

    ~PhysicsComponent() override;

    void start() override;

    void update(float deltaTime) override;

    void setup(Scene* scene) override;

    void addImpulse(Vector3 const& direction) const;

    void setEnableGravity(bool value) const;

    void setSimulatePhysics(bool value) const;

    NODISCARD bool isSimulatingPhysics() const;

    NODISCARD bool hasGravityEnabled() const;

    NODISCARD FORCEINLINE physx::PxRigidActor* getRigidBody() const { return rigidActor; }

    void makeDynamicSphereCollider(float radius = 1.0f);
    void makeStaticSphereCollider(float radius = 1.0f);
    void makeDynamicBoxCollider(Vector3 const& halfExtents = { 1.0f, 1.0f, 1.0f });
    void makeStaticBoxCollider(Vector3 const& halfExtents = { 1.0f, 1.0f, 1.0f });
    void makeDynamicCapsuleCollider(float radius = 1.0f, float halfHeight = 1.0f);
    void makeStaticCapsuleCollider(float radius = 1.0f, float halfHeight = 1.0f);

    bool makeModelCollider(struct Model* model);

private:

    physx::PxRigidActor* rigidActor = nullptr;
    static inline physx::PxMaterial* material = nullptr;

};
