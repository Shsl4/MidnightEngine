#pragma once

#include <Scene/Actor.h>
#include <Scene/ModelComponent.h>

/*!
 *  A simple Actor having a ModelComponent at its root
 */
class ModelActor : public Actor {

public:

    /*!
     * The ModelActor constructor.
     */
    ModelActor();

    void setModel(String const& name) const;
    void setTexture(size_t index, String const& name) const;
    void setShader(size_t index, bgfx::ProgramHandle handle) const;
    void setMaterial(size_t index, Material const& material) const;
    Material& getMaterial(size_t index) const;

    FORCEINLINE ModelComponent* getModelComponent() const{
        return model;
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

};
