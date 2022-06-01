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
