$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

struct Material{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

struct Light{
    vec4 position;
    vec4 ambientColor;
    vec4 diffuseColor;
    vec4 specularColor;
};

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform vec4 matShininess;

uniform vec4 viewPos; 

uniform vec4 lightAttenuation;
uniform vec4 lightPos; 
uniform vec4 lightAmbientColor;
uniform vec4 lightDiffuseColor;
uniform vec4 lightSpecularColor;

#define constant lightAttenuation.x
#define linear lightAttenuation.y
#define quadratic lightAttenuation.z

void main()
{
    
    float distance = length(lightPos.xyz - v_pos);
    float attenuation = 1.0f / (constant + linear * distance + quadratic * (distance * distance));

    // Ambient
    vec4 ambient = matAmbient * lightAmbientColor * attenuation;

    // Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(lightPos.xyz - v_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = lightDiffuseColor * (diff * matDiffuse) * attenuation;

    // Specular
    vec3 viewDir = normalize(viewPos - v_pos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), matShininess.x);
    vec4 specular = lightSpecularColor * (matSpecular * spec) * attenuation;
    
    vec4 result = ambient + diffuse + specular;
    gl_FragColor = result;
	
}
