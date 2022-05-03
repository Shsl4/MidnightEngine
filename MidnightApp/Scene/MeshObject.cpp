#include <Scene/MeshObject.h>
#include <Scene/Scene.h>
#include <utility>

MeshObject::MeshObject(String meshName) : meshName(std::move(meshName)) {



}

void MeshObject::createComponents(Scene *scene, Transform transform) {

    // Create the mesh component
    this->mesh = scene->createComponent<MeshComponent>(transform, meshName);
    this->setRootComponent(mesh);

}

