#include <Rendering/Model.h>

#include <Rendering/ShaderManager.h>

#include <Rendering/ShaderPrograms.h>
#include <Rendering/Uniforms.h>
#include <Scene/PointLightComponent.h>
#include <Scene/DirectionalLightComponent.h>

Model::Model(Array<SharedPointer<Mesh>> meshes, Array<SharedPointer<Texture>> textures, String name) :
    modelName(std::move(name)), boundTextures(std::move(textures)), meshes(std::move(meshes)) {

    for (const auto& e : this->meshes) {
        defaultHandles += ShaderPrograms::directionalLightShader;
        defaultMaterials += Material();
    }
    
}

Array<WeakPointer<Texture>> Model::getTextures() const {
    
    auto textures = Array<WeakPointer<Texture>>(this->boundTextures.getSize());

    for (const auto& texture : boundTextures) {
            
        textures += texture.weak();
        
    }

    return textures;

}

void Model::render(UInt16 viewId,
                   Vector4 const& view,
                   Matrix4 const& transform,
                   Array<WeakPointer<Texture>> const& textures,
                   Array<Material> const& materials,
                   Array<bgfx::ProgramHandle> const& programs,
                   LightComponent* light) const {

    size_t count = 0;

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

            if (const auto* directionalLight = light->cast<DirectionalLightComponent>()) {
                const auto lightDirection = directionalLight->getLightDirection();
                setUniform(Uniforms::lightDirection, &lightDirection);
            }

            if (const auto* directionalLight = light->cast<PointLightComponent>()) {
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

        mesh->render(viewId, materials[count], textures[count].valid() ? textures[count].raw() : nullptr, programs[count]);
        ++count;
        
    }   
    
}
