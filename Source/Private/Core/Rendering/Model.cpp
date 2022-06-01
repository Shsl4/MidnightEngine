#include <Rendering/Model.h>

#include <Rendering/ShaderManager.h>

#include <Rendering/ShaderPrograms.h>
#include <Rendering/Uniforms.h>
#include <Scene/PointLightComponent.h>
#include <Scene/DirectionalLightComponent.h>

Model::Model(Array<SharedPointer<Mesh>> meshes, String name, Material const& mat) :
    modelName(std::move(name)), material(mat), meshes(std::move(meshes)) {

    if(modelName == "Skybox") {
        this->handle = ShaderPrograms::skyboxShader;
    }
    else {
        this->handle = ShaderPrograms::directionalLightShader;
    }   

    
}

void Model::render(UInt16 viewId, Vector4 const& view, Matrix4 const& transform, LightComponent* light) const {

    for (auto const& mesh : meshes) {
        
        setUniform(Uniforms::viewPosition, &view);
        bgfx::setTransform(&transform);

        if (light) {

            const auto lightPosition = light->getWorldPosition();
            const auto lightAmbient = LinearColors::black;
            const auto lightDiffuse = light->getDiffuseColor();
            const auto lightSpecular = light->getSpecularColor();

            setUniform(Uniforms::lightPosition, &lightPosition);
            setUniform(Uniforms::lightAmbient, &lightAmbient);
            setUniform(Uniforms::lightDiffuse, &lightDiffuse);
            setUniform(Uniforms::lightSpecular, &lightSpecular);

            if(const auto* directionalLight = light->cast<DirectionalLightComponent>()) {

                const auto lightDirection = directionalLight->getLightDirection();
                setUniform(Uniforms::lightDirection, &lightDirection);
                
            }

            if(const auto* directionalLight = light->cast<PointLightComponent>()) {

                const auto attenuation = directionalLight->getAttenuation();
                setUniform(Uniforms::lightAttenuation, &attenuation);
                
            }
            
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