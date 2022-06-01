$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

struct Material{
    vec4 ambient;
    sampler2D diffuse;
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
SAMPLER2D(texDiffuse, 0);
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform vec4 matShininess;

uniform vec4 viewPos; 

uniform vec4 lightPos; 
uniform vec4 lightAmbientColor;
uniform vec4 lightDiffuseColor;
uniform vec4 lightSpecularColor;

void main()
{
    
    // Ambient
    vec4 ambient = matAmbient * texture2D(texDiffuse, v_texcoord0) * lightAmbientColor;

    // Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(lightPos.xyz - v_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = matDiffuse * lightDiffuseColor * diff * texture2D(texDiffuse, v_texcoord0);

    // Specular
    vec3 viewDir = normalize(viewPos - v_pos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), matShininess.x);
    vec4 specular = lightSpecularColor * (matSpecular * spec);
    
    vec4 result = ambient + diffuse + specular;
    gl_FragColor = result;
	
}
