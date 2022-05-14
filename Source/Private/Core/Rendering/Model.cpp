#include <Rendering/Model.h>

#include <Rendering/ShaderManager.h>

#include "Rendering/ShaderPrograms.h"
#include "Rendering/Uniforms.h"
#include "Scene/BasicLightComponent.h"

Model::Model(Array<SharedPointer<Mesh>> meshes, String name, Material const& mat) :
    modelName(std::move(name)), material(mat), meshes(std::move(meshes)) {

    this->handle = ShaderPrograms::materialShader;
    
}

void Model::render(UInt16 viewId, Vector4 const& view, Matrix4 const& transform, WeakPointer<BasicLightComponent> light) const {

    for (auto const& mesh : meshes) {
        
        setUniform(Uniforms::viewPosition, &view);
        bgfx::setTransform(&transform);

        if (light.valid()) {

            const auto lightPosition = light->getWorldPosition();
            const auto lightAmbient = light->getAmbientColor();
            const auto lightDiffuse = light->getDiffuseColor();
            const auto lightSpecular = light->getSpecularColor();

            setUniform(Uniforms::lightPosition, &lightPosition);
            setUniform(Uniforms::lightAmbient, &lightAmbient);
            setUniform(Uniforms::lightDiffuse, &lightDiffuse);
            setUniform(Uniforms::lightSpecular, &lightSpecular);
            
        }
        else {

            setUniform(Uniforms::lightPosition, &Vector4::zero);
            setUniform(Uniforms::lightAmbient, &Vector4::zero);
            setUniform(Uniforms::lightDiffuse, &Vector4::zero);
            setUniform(Uniforms::lightSpecular, &Vector4::zero);
            
        }
        
        mesh->render(viewId, material, handle);
    }
    
}