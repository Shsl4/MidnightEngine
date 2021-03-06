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

        modMutex.lock();
        
        textures.clear();
        handles.clear();
        materials.clear();

        this->model = loadedModel;
        
        this->textures = this->model->getTextures();
        this->handles = this->model->getPrograms();
        this->materials = this->model->getMaterials();

        modMutex.unlock();
        
        return true;

    }

    return false;

}

bool ModelComponent::setTexture(size_t index, String const& name) {

    if (index >= textures.getSize() ) {
        return false;
    }
        
    if(model.valid()) {

        modMutex.lock();
        
        if(name.isEmpty()) {
            textures[index] = nullptr;
            modMutex.unlock();
            return true;
        }

        const auto texture = Engine::getInstance()->getResourceLoader()->getTexture(name);

        if(texture.valid()) {
            textures[index] = texture;
            modMutex.unlock();
            return true;
        }

        modMutex.unlock();
        
    }

    return false;
    
}

bool ModelComponent::setShader(size_t index, bgfx::ProgramHandle handle) {

    if (index >= textures.getSize() ) {
        return false;
    }

    if(model.valid()) {
        
        modMutex.lock();
        handles[index] = handle;
        modMutex.unlock();
        return true;
        
    }

    return false;
    
}

void ModelComponent::setMaterial(size_t index, Material const& material) {
    
    if (index >= materials.getSize()) { return; }
    
    modMutex.lock();
    materials[index] = material;
    modMutex.unlock();

}


Material& ModelComponent::getMaterial(size_t index) const {
    return materials[index];
}

void ModelComponent::render() {
    
    if(!visible) return;
    
    // If a model is set
    if (!model.expired()){

        modMutex.lock();
        
        // Render it.
        const auto view = Vector4(getScene()->getCameraManager()->getActiveCamera()->getWorldPosition());
        
        // Render it.
        model->render(viewId, view, Matrix4::modelMatrix(getWorldTransform()), textures, materials, handles);

        modMutex.unlock();
        
    }
    
}
