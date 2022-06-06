#pragma once

#include <Scene/Actor.h>
#include <Scene/PhysicsComponent.h>
#include <Scene/ModelComponent.h>

class PhysicsModelActor : public Actor {
    
public:

    PhysicsModelActor();
    
    void setModel(String const& name) const;
    void setTexture(size_t index, String const& name) const;
    void setShader(size_t index, bgfx::ProgramHandle handle) const;
    void setMaterial(size_t index, Material const& material) const;
    Material& getMaterial(size_t index) const;

    FORCEINLINE ModelComponent* getModelComponent() const{
        return model;
    }

    FORCEINLINE PhysicsComponent* getPhysicsComponent() const{
        return physicsComponent;
    }

private:

    /*!
     * The model component.
     */
    ModelComponent* model = nullptr;
    
    /*!
     * The mesh name to load.
     */
    const String meshName;
    
    PhysicsComponent* physicsComponent = nullptr;
    
};
