#pragma once

#include <Scene/SceneComponent.h>
#include <Rendering/Renderable.h>
#include <Rendering/LinearColor.h>
#include <Math/Vector4.h>

struct Attenuation{
    
    Attenuation(float constant, float linear, float exponent)
        : constant(constant), linear(linear), exponent(exponent){
        
    }
    
    float constant = 0.0f;
    float linear = 0.0f;
    float exponent = 0.0f;
    
};

struct PointLight{
    
    PointLight(LinearColor color, Vector3 position, float intensity, Attenuation attenuation)
        : color(color), position(position), intensity(intensity), attenuation(attenuation){
        
    }
    
    LinearColor color = LinearColors::white;
    Vector3 position = Vector3();
    float intensity = 0.0f;
    Attenuation attenuation;
    
};

struct DirectionalLight{
    
    DirectionalLight(Vector3 direction, Vector3 ambient, Vector3 diffuse, Vector3 specular)
        : direction(direction), ambient(ambient), diffuse(diffuse), specular(specular)
    {
        
        
    }
    
    DirectionalLight() = default;
    
    Vector3 direction = Vector3(-1.0f, -1.0f, 0.0f);
    Vector3 ambient = Vector3(0.0f);
    Vector3 diffuse = Vector3(0.05f);
    Vector3 specular = Vector3(0.5f);

};

struct Material
{
    
    Material(LinearColor ambient, LinearColor diffuse, LinearColor specular, bool hasTexture, float reflectance)
        :ambient(ambient), diffuse(diffuse), specular(specular), hasTexture(hasTexture), reflectance(reflectance){
        
        
    }
    
    LinearColor ambient = LinearColors::white;
    LinearColor diffuse = LinearColors::white;
    LinearColor specular = LinearColors::white;
    bool hasTexture = false;
    float reflectance = 0.0f;
};

struct PointLightHandles{
    
    bgfx::UniformHandle color;
    bgfx::UniformHandle position;
    // Intensity, constant, linear and exponent packed together
    bgfx::UniformHandle icle;
};

struct DirectionalLightHandles{
    
    bgfx::UniformHandle direction;
    bgfx::UniformHandle ambient;
    bgfx::UniformHandle diffuse;
    bgfx::UniformHandle specular;
};

struct MaterialHandles
{
    
    bgfx::UniformHandle ambient;
    bgfx::UniformHandle diffuse;
    bgfx::UniformHandle specular;
    // hasTexture and reflectance value packed
    bgfx::UniformHandle texRef;
};

class MeshComponent : public SceneComponent, public Renderable
{

public:
    
    explicit MeshComponent(std::string const& path);
    void update(float deltaTime) override;
    void construct(Transform const& relativeTransform) override;

    virtual ~MeshComponent();

    
private:
    
    using Super = SceneComponent;

    void setUniforms() const;
    void render() override;
    struct Mesh* mesh;
    
    bgfx::UniformHandle ambientLightHandle;
    PointLightHandles pointLightHandles;
    DirectionalLightHandles dirLightHandles;
    MaterialHandles materialHandles;
    Material mat;
    PointLight light;
    DirectionalLight dirLight;

};

