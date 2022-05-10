#include <Scene/MeshComponent.h>
#include <Rendering/Mesh.h>
#include <Math/Matrix4.h>

#include <Core/Engine.h>
#include <Scene/Scene.h>

void MeshComponent::update(float deltaTime) {

    Super::update(deltaTime);
    
}

MeshComponent::MeshComponent(String const& name) {
        
    this->mesh = Engine::getInstance()->getResourceLoader()->getMesh(name);

    if(name == "Cube")
    {
        this->texture = Engine::getInstance()->getResourceLoader()->getTexture("Missing");
    }
    
    this->viewPos = createUniform("viewPos", bgfx::UniformType::Vec4);

}

void MeshComponent::construct(Transform const &relativeTransform) {
    Super::construct(relativeTransform);
}

MeshComponent::~MeshComponent() {
    destroy(viewPos);
}


void MeshComponent::render() {

    // If a mesh is set
    if (mesh){

        // Render it.
        mesh->use();
        
        const auto lightPos = Vector4(1.2f, 1.0f, 2.0f, 1);
        
        const auto lightAmbient = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
        const auto lightDiffuse = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
        const auto lightSpecular = Vector4(1.0f);
        
        const auto view = Vector4(getScene()->getCameraManager()->getActiveCamera()->getWorldPosition());

        const auto ambient = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        const auto diffuse = Vector4(1.0f, 1.0f,1.0f, 1.0f);
        const auto specular = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
        const auto shininess = Vector4(128.0f);
        
        setUniform(light.lightPos, &lightPos);
        setUniform(light.ambientColor, &lightAmbient);
        setUniform(light.diffuseColor, &lightDiffuse);
        setUniform(light.specularColor, &lightSpecular);

        setUniform(viewPos, &view);
        
        setUniform(material.ambient, &ambient);

        if(texture)
        {
            texture->use(0, material.textureDiffuse);
        }
        else
        {
            setUniform(material.diffuse, &diffuse);
        }

        setUniform(material.specular, &specular);
        setUniform(material.shininess, &shininess);

        model = Matrix4::modelMatrix(getWorldTransform());
        bgfx::setTransform(model.data);
        mesh->submit();
        
    }

}
