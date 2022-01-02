#pragma once

#include <Scene/SceneObject.h>
#include <Scene/MeshComponent.h>

class MeshObject : public SceneObject {
    
public:

    MeshObject(std::string const& meshName);
    
    void createComponents(class Scene *scene, Transform transform) override;
    
private:
    
    MeshComponent* mesh;
    const std::string meshName;
    
};
