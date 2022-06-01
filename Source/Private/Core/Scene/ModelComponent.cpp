#include <Scene/ModelComponent.h>
#include <Rendering/Mesh.h>
#include <Math/Matrix4.h>

#include <Core/Engine.h>
#include <Scene/Scene.h>

void ModelComponent::update(float deltaTime) {

    Super::update(deltaTime);
    
}

void ModelComponent::setModel(String const& name) {

    this->model = Engine::getInstance()->getResourceLoader()->getModel(name);

    if(name == "Cube") {
        this->texture = Engine::getInstance()->getResourceLoader()->getTexture("Missing");
    }
    
}

void ModelComponent::render(UInt64 state) {

    // If a model is set
    if (!model.expired()){

        // Render it.
        const auto view = Vector4(getScene()->getCameraManager()->getActiveCamera()->getWorldPosition());
        const auto light = getScene()->getFirstComponentOf<LightComponent>();

        bgfx::setState(state);
        
        // Render it.
        model->render(0, view, Matrix4::modelMatrix(getWorldTransform()), light);
        
    }
    
}
