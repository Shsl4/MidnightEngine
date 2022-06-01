#include <Scene/ModelComponent.h>
#include <Rendering/Mesh.h>
#include <Math/Matrix4.h>

#include <Core/Engine.h>
#include <Scene/Scene.h>

void ModelComponent::update(float deltaTime) {

    Super::update(deltaTime);
    
}

bool ModelComponent::setModel(String const& name) {
    
    const auto loadedModel = Engine::getInstance()->getResourceLoader()->getModel(name);

    if(loadedModel.valid()) {

        textures.clear();
        handles.clear();
        materials.clear();

        this->model = loadedModel;
        
        this->textures = this->model->getTextures();
        this->handles = this->model->getPrograms();
        this->materials = this->model->getMaterials();
        
        return true;

    }

    return false;

}

bool ModelComponent::setTexture(size_t index, String const& name) {

    if (index >= textures.getSize() ) {
        return false;
    }
        
    if(model.valid()) {

        const auto texture = Engine::getInstance()->getResourceLoader()->getTexture(name);

        if(texture.valid()) {
            textures[index] = texture;
            return true;
        }        
        
    }

    return false;
    
}

bool ModelComponent::setShader(size_t index, bgfx::ProgramHandle handle) {

    if (index >= textures.getSize() ) {
        return false;
    }

    if(model.valid()) {

        handles[index] = handle;
        return true;
        
    }

    return false;
    
}

void ModelComponent::render(UInt64 state) {

    // If a model is set
    if (!model.expired()){

        // Render it.
        const auto view = Vector4(getScene()->getCameraManager()->getActiveCamera()->getWorldPosition());
        const auto light = getScene()->getFirstComponentOf<LightComponent>();

        bgfx::setState(state);
        
        // Render it.
        model->render(0, view, Matrix4::modelMatrix(getWorldTransform()), textures, materials, handles, light);
        
    }
    
}
