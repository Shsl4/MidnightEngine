$input v_color0, v_normal, v_texcoord0, v_pos

#include <common.sh>

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    bool hasTexture;
    float reflectance;
    
};

struct LightData{
    
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec4 l_direction;
uniform vec4 l_ambient;
uniform vec4 l_diffuse;
uniform vec4 l_specular;

uniform vec4 m_ambient;
uniform vec4 m_diffuse;
uniform vec4 m_specular;
uniform vec4 m_texRef;

uniform sampler2D textureSampler;

LightData makeLightData(Material material){
    
    LightData data;
    
    if(material.hasTexture){
        
        /*
        data.ambient = texture(material.ambient, v_texcoord0);
        data.diffuse = texture(material.diffuse, v_texcoord0);
        data.specular = texture(material.specular, v_texcoord0);
         */
    }
    else{
        
        data.ambient = material.ambient;
        data.diffuse = material.diffuse;
        data.specular = material.specular;
        
    }
    
    return data;
    
}

vec3 CalcDirLight(Material material, LightData lightData, DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.reflectance);
    // combine results
    vec3 ambient = light.ambient * lightData.ambient.xyz;
    vec3 diffuse = light.diffuse * diff * lightData.diffuse.xyz;
    vec3 specular = light.specular * spec * lightData.diffuse.xyz;
    return (ambient + diffuse + specular);
}

void main()
{

    Material material = Material(m_ambient, m_diffuse, m_specular, m_texRef.x, m_texRef.y);
    LightData data = makeLightData(material);
    DirectionalLight light = DirectionalLight(l_direction.xyz, l_ambient.xyz, l_diffuse.xyz, l_specular.xyz);
    vec3 norm = normalize(v_normal);
    vec3 viewDir = normalize(-v_pos);
    
    gl_FragColor = vec4(CalcDirLight(material, data, light, norm, viewDir), 1.0f);
    
}
