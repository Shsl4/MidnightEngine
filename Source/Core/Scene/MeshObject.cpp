#include <Scene/MeshObject.h>

MeshObject::MeshObject(std::string const &meshName) : meshName(meshName) {


}

void MeshObject::createComponents(Scene *scene, Transform transform) {

    // Create the mesh component
    this->mesh = scene->createComponent<MeshComponent>(transform, meshName);
    this->setRootComponent(mesh);

}

