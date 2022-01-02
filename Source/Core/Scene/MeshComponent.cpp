#include <Scene/MeshComponent.h>
#include <Rendering/Mesh.h>
#include <Math/Matrix4.h>

MaterialHandles createMaterialUniforms(){
    
    MaterialHandles handles;
    handles.ambient = bgfx::createUniform("m_ambient", bgfx::UniformType::Vec4);
    handles.diffuse = bgfx::createUniform("m_diffuse", bgfx::UniformType::Vec4);
    handles.specular = bgfx::createUniform("m_specular", bgfx::UniformType::Vec4);
    handles.texRef = bgfx::createUniform("m_texRef", bgfx::UniformType::Vec4);
    return handles;
    
}

PointLightHandles createPointLightUniforms(){
    
    PointLightHandles handles;
    handles.color = bgfx::createUniform("l_color", bgfx::UniformType::Vec4);
    handles.position = bgfx::createUniform("l_position", bgfx::UniformType::Vec4);
    handles.icle = bgfx::createUniform("icle", bgfx::UniformType::Vec4);
    return handles;
    
}


DirectionalLightHandles createDirectionalLightUniforms(){
    
    DirectionalLightHandles handles;
    handles.direction = bgfx::createUniform("l_direction", bgfx::UniformType::Vec4);
    handles.ambient = bgfx::createUniform("l_ambient", bgfx::UniformType::Vec4);
    handles.diffuse = bgfx::createUniform("l_diffuse", bgfx::UniformType::Vec4);
    handles.specular = bgfx::createUniform("l_specular", bgfx::UniformType::Vec4);
    return handles;
    
}

void MeshComponent::setUniforms(){

    float texRef[4] = { static_cast<float>(mat.hasTexture), mat.reflectance, 0.0f, 0.0f };
    float pos[4] = { light.position.x, light.position.y, light.position.z, 1.0f };
    float icle[4] = { light.intensity, light.attenuation.constant, light.attenuation.linear, light.attenuation.exponent };
    float ambient[4] = { 0.3f, 0.3f, 0.3f, 1.0f };

    bgfx::setUniform(materialHandles.ambient, &mat.ambient);
    bgfx::setUniform(materialHandles.diffuse, &mat.diffuse);
    bgfx::setUniform(materialHandles.specular, &mat.specular);
    bgfx::setUniform(materialHandles.texRef, texRef);
    /*
    bgfx::setUniform(dirLightHandles.direction, &dirLight.direction);
    bgfx::setUniform(dirLightHandles.ambient, &dirLight.ambient);
    bgfx::setUniform(dirLightHandles.diffuse, &dirLight.diffuse);
    bgfx::setUniform(dirLightHandles.specular, &dirLight.specular);
*/
    
    bgfx::setUniform(pointLightHandles.color, &light.color);
    bgfx::setUniform(pointLightHandles.position, pos);
    bgfx::setUniform(pointLightHandles.icle, icle);
    bgfx::setUniform(ambientLightHandle, ambient);
    
    
    bgfx::setTransform(Matrix4::modelMatrix(transform).data);
    
}


MeshComponent::MeshComponent(std::string const& path)
    : mat(LinearColors::white, LinearColors::white, LinearColors::white, false, 1.0f),
        light(LinearColor::fromRGB(155.0f, 39.0f, 227.0f), Vector3(0.0f, 2.0f, 0.0f), 5.0f, Attenuation(0.0f, 0.0f, 0.5f)){

    this->mesh = MeshLoader::loadOBJ(path);
    this->ambientLightHandle = bgfx::createUniform("ambientLight", bgfx::UniformType::Vec4);
    this->pointLightHandles = createPointLightUniforms();
    this->dirLightHandles = createDirectionalLightUniforms();
    this->materialHandles = createMaterialUniforms();
            
}

void MeshComponent::update(float deltaTime){
    
    SceneComponent::update(deltaTime);
    
    transform.rotation.y += 20.0f * deltaTime;
    
}

void MeshComponent::construct(Transform const& relativeTransform){
    
    Super::construct(relativeTransform);
    Vector3 lightPos = transform.position;
    lightPos.y += 3.0f;
    this->light = PointLight(LinearColor::fromRGB(155.0f, 39.0f, 227.0f), lightPos, 5.0f, Attenuation(0.0f, 0.0f, 0.5f));
    
}


void MeshComponent::render()
{
 
    setIndexBuffer(mesh->indexBuffer);
    setVertexBuffer(0, mesh->vertexBuffer);
    setUniforms();
    submit(0, mesh->programHandle);
    
}
