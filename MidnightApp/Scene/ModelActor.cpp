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
