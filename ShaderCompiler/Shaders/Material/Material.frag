$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

#define MAX_LIGHTS 10

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

uniform vec4 lightAttenuation[MAX_LIGHTS];
uniform vec4 lightPos[MAX_LIGHTS]; 

uniform vec4 lightDirection[MAX_LIGHTS]; 

uniform vec4 lightAmbientColor[MAX_LIGHTS];
uniform vec4 lightDiffuseColor[MAX_LIGHTS];
uniform vec4 lightSpecularColor[MAX_LIGHTS];

uniform vec4 lightData;

#define lightCount lightData[0]

vec4 computeDirectionalLight(vec3 position, vec3 normal, vec3 viewp, vec4 lAmbient, vec4 lDiffuse, vec4 lSpecular, vec4 lDir){

    // Ambient
    vec4 ambient = matAmbient * lAmbient;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-lDir.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = lDiffuse * (diff * matDiffuse);

    // Specular
    vec3 viewDir = normalize(viewp - position);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), matShininess.x);
    vec4 specular = lSpecular * (matSpecular * spec);
    
    return ambient + diffuse + specular;

}

vec4 computePointLight(vec3 position, vec3 normal, vec3 viewp, vec4 lAmbient, vec4 lDiffuse, vec4 lSpecular, vec4 lPos, vec4 lAtt){

    float constant = lAtt.x;
    float lin = lAtt.y;
    float quadratic = lAtt.z;

    float dist = length(lPos.xyz - position);
    float attenuation = 1.0f / (constant + lin * dist + quadratic * (dist * dist));

    // Ambient
    vec4 ambient = matAmbient * lAmbient * attenuation;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lPos.xyz - position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = lDiffuse * (diff * matDiffuse) * attenuation;

    // Specular
    vec3 viewDir = normalize(viewp - position);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), matShininess.x);
    vec4 specular = lSpecular * (matSpecular * spec) * attenuation;
    
    return ambient + diffuse + specular;

}

bool v4eqz(vec4 a){

    return a.x == 0.0 && a.y == 0.0 && a.z == 0.0 && a.w == 0.0;

}

void main() {

    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i < lightCount; ++i){

        if(!v4eqz(lightDirection[i])){
            finalColor += computeDirectionalLight(v_pos, 
                v_normal,
                viewPos.xyz, 
                lightAmbientColor[i],
                lightDiffuseColor[i],
                lightSpecularColor[i],
                lightDirection[i]);
        }
        else{
            finalColor += computePointLight(v_pos, 
                v_normal, 
                viewPos.xyz, 
                lightAmbientColor[i],
                lightDiffuseColor[i],
                lightSpecularColor[i],
                lightPos[i],
                lightAttenuation[i]);
        }

    }

	gl_FragColor = finalColor;

}
