$input v_color0, v_normal, v_texcoord0, v_pos

#include <common.sh>

struct Attenuation{
    
    float constant;
    float lin;
    float exponent;
    
};

struct PointLight{
    
    vec4 color;
    vec3 position;
    float intensity;
    Attenuation attenuation;
    
};

struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    bool hasTexture;
    float reflectance;
    
};

struct Data{
    
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    
};

uniform vec4 ambientLight;

uniform vec4 m_ambient;
uniform vec4 m_diffuse;
uniform vec4 m_specular;
uniform vec4 m_texRef;

uniform vec4 l_color;
uniform vec4 l_position;
uniform vec4 icle;

uniform sampler2D textureSampler;

Data setupColors(Material material, vec2 texCoord)
{
    Data d;

    if (material.hasTexture)
    {

/*        ambientC = texture(textureSampler, texCoord);
        diffuseC = ambientC;
        speculrC = ambientC;*/
    }
    else
    {
        d.ambient = material.ambient;
        d.diffuse = material.diffuse;
        d.specular = material.specular;
    }
    return d;

}

vec3 calcPointLight(Data data, Material material, PointLight light, vec3 position, vec3 normal)
{
    vec3 viewDir = vec3(u_view[0][3], u_view[1][3], u_view[2][3]) - position;
    vec3 lightDir = normalize(light.position - position);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.reflectance);
    float dist = length(light.position - position);
    float attenuation = 1.0 / (light.attenuation.constant + light.attenuation.lin * dist + light.attenuation.exponent * (dist * dist));
    vec3 ambient  = ambientLight * material.ambient;
    vec3 diffuse  = diff * material.diffuse;
    vec3 specular = spec * material.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{

    Material material = Material(m_ambient, m_diffuse, m_specular, m_texRef.x, m_texRef.y);
    
    Data data = setupColors(material, v_texcoord0);
    
    PointLight pointLight = PointLight(l_color, mul(u_view, l_position).xyz, icle.x, Attenuation(icle.y, icle.z, icle.w));
    
    vec3 diffuseSpecularComp = calcPointLight(data, material, pointLight, v_pos, v_normal);

    gl_FragColor = vec4(diffuseSpecularComp, 1.0f) * pointLight.color;
}
