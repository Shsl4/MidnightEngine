#pragma once

#include <Scene/SceneObject.h>
#include <Scene/MeshComponent.h>

/*!
 *  A simple SceneObject having a MeshComponent at its root
 */
class MeshObject : public SceneObject {

public:

    /*!
     * The MeshObject constructor.
     *
     *  \param[in] meshName The file name of the mesh to load. (In the Resources/Models folder)
     */
    MeshObject(std::string const &meshName);

    void createComponents(class Scene *scene, Transform transform) override;

private:

    /*!
     * The mesh component.
     */
    MeshComponent *mesh;
    
    /*!
     * The mesh name to load.
     */
    const std::string meshName;

};
