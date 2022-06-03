#include <Scene/ModelActor.h>
#include <Scene/Scene.h>
#include <utility>

ModelActor::ModelActor() {

    this->model = createComponent<ModelComponent>("RootModel");
    this->setRootComponent(model);

}

void ModelActor::setModel(String const& name) const {
    this->model->setModel(name);
}

void ModelActor::setTexture(size_t index, String const& name) const {
    model->setTexture(index, name);
}

void ModelActor::setShader(size_t index, bgfx::ProgramHandle handle) const {
    model->setShader(index, handle);
}

void ModelActor::setMaterial(size_t index, Material const& material) const {
    model->setMaterial(index, material);
}

Material& ModelActor::getMaterial(size_t index) const {
    return model->getMaterial(index);
}
