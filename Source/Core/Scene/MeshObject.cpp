#include <Scene/MeshObject.h>
#include <Scene/Scene.h>

MeshObject::MeshObject(std::string const& meshName) : meshName(meshName){
    
    
}

void MeshObject::createComponents(Scene *scene, Transform transform) {
    
    this->mesh = scene->createComponent<MeshComponent>(transform, meshName);
    this->setRootComponent(mesh);
    
}

