$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

uniform vec4 lightPos; 
uniform vec4 viewPos; 
uniform vec4 lightDiffuseColor;
uniform vec4 matDiffuse;

void main()
{
    
    // Ambient
    float ambientStrengh = 0.1f;
    vec4 ambient = ambientStrengh * lightDiffuseColor;

    // Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(lightPos.xyz - v_pos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec4 diffuse = diff * lightDiffuseColor;

    // Specular
    float specularStrengh = 0.5f;
    vec3 viewDir = normalize(viewPos - v_pos);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0f), 32);
    vec4 specular = specularStrengh * spec * lightDiffuseColor;
    
    vec4 result = (ambient + diffuse + specular) * matDiffuse;
    gl_FragColor = result;
	
}
