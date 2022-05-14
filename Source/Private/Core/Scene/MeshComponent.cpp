#include <Scene/MeshComponent.h>
#include <Rendering/Mesh.h>
#include <Math/Matrix4.h>

#include <Core/Engine.h>
#include <Scene/Scene.h>

void MeshComponent::update(float deltaTime) {

    Super::update(deltaTime);
    
}

MeshComponent::MeshComponent(String const& name) {
        
    this->model = Engine::getInstance()->getResourceLoader()->getModel(name);

    if(name == "Cube")
    {
        this->texture = Engine::getInstance()->getResourceLoader()->getTexture("Missing");
    }
    
}

void MeshComponent::construct(Transform const &relativeTransform) {
    Super::construct(relativeTransform);
}

void MeshComponent::render() {

    // If a model is set
    if (!model.expired()){

        // Render it.
        const auto view = Vector4(getScene()->getCameraManager()->getActiveCamera()->getWorldPosition());
        const auto light = getScene()->getFirstComponentOfClass<BasicLightComponent>();
        
        // Render it.
        model->render(0, view, Matrix4::modelMatrix(getWorldTransform()), light);
        
    }

}
