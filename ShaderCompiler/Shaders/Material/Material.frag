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

SAMPLER2D(texDiffuse, 0);

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

// x = lightType, y = SL Cutoff, z = SL OuterCutoff
uniform vec4 additionalLightData[MAX_LIGHTS];

// x = lightCount, y = hasTexturem z = has normalTexture
uniform vec4 additionalData;

#define lightCount additionalData[0]
#define hasTexture additionalData[1]

vec4 computeDirectionalLight(vec2 texCoords, vec3 position, vec3 normal, vec3 viewp, vec4 lAmbient, vec4 lDiffuse, vec4 lSpecular, vec4 lDir){

    // Ambient
    vec4 ambient = vec4(0.0, 0.0, 0.0, 0.0);
    
    if(additionalData[1]){
        ambient = matAmbient * texture2D(texDiffuse, texCoords) * lAmbient;
    }
    else{
        ambient = matAmbient * lAmbient;
    }

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(-lDir.xyz);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);

    if(additionalData[1]){
        diffuse = matDiffuse * lDiffuse * diff * texture2D(texDiffuse, texCoords);
    }
    else{
        diffuse = lDiffuse * (diff * matDiffuse);
    }

    // Specular
    vec3 viewDir = normalize(viewp - position);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), matShininess.x);
    vec4 specular = lSpecular * (matSpecular * spec);
    
    return ambient + diffuse + specular;

}

vec4 computePointLight(vec2 texCoords, vec3 position, vec3 normal, vec3 viewp, vec4 lAmbient, vec4 lDiffuse, vec4 lSpecular, vec4 lPos, vec4 lAtt){

    float constant = lAtt.x;
    float lin = lAtt.y;
    float quadratic = lAtt.z;

    float dist = length(lPos.xyz - position);

    float attenuation = 1.0;

    if(quadratic == 0 && lin == 0){
        attenuation = 1.0 / (constant);
    }
    else{
        attenuation = 1.0 / (constant + lin * dist + quadratic * (dist * dist));
    }

    // Ambient
    vec4 ambient = vec4(0.0, 0.0, 0.0, 0.0);
    
    if(additionalData[1]){
        ambient = texture2D(texDiffuse, texCoords) * lAmbient * attenuation;
    }
    else{
        ambient = matAmbient * lAmbient * attenuation;
    }

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lPos.xyz - position);
    float diff = max(dot(norm, lightDir), 0.0);

    vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);

    if(additionalData[1]){
        diffuse = lDiffuse * (diff * texture2D(texDiffuse, texCoords)) * attenuation;
    }
    else{
        diffuse = lDiffuse * (diff * matDiffuse) * attenuation;
    }

    // Specular
    vec3 viewDir = normalize(viewp - position);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), matShininess.x);
    vec4 specular = lSpecular * (matSpecular * spec) * attenuation;
    
    return ambient + diffuse + specular;

}

vec4 computeSpotLight(vec2 texCoords, vec3 position, vec3 normal, vec3 viewp, vec4 lAmbient, vec4 lDiffuse, vec4 lSpecular, vec4 lPos, vec4 lDir, vec4 lAtt, float cutoff, float outerCutoff){

    float constant = lAtt.x;
    float lin = lAtt.y;
    float quadratic = lAtt.z;

    float dist = length(lPos.xyz - position);
    float attenuation = 1.0 / (constant + lin * dist + quadratic * (dist * dist));

    // Ambient
    vec4 ambient = vec4(0.0, 0.0, 0.0, 0.0);
    
    if(additionalData[1]){
        ambient = texture2D(texDiffuse, texCoords) * lAmbient * attenuation;
    }
    else{
        ambient = matAmbient * lAmbient * attenuation;
    }

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lPos.xyz - position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);

    if(additionalData[1]){
        diffuse = lDiffuse * (diff * texture2D(texDiffuse, texCoords)) * attenuation;
    }
    else{
        diffuse = lDiffuse * (diff * matDiffuse) * attenuation;
    }

    // Specular
    vec3 viewDir = normalize(viewp - position);
    vec3 reflectionDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), matShininess.x);
    vec4 specular = lSpecular * (matSpecular * spec) * attenuation;

    float theta = dot(lightDir, normalize(-lDir.xyz)); 
    float epsilon = (cutoff - outerCutoff);
    float intensity = clamp((theta - outerCutoff) / epsilon, 0.0, 1.0);

    return ambient + diffuse * intensity + specular * intensity;

}

void main() {

    vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i < lightCount; ++i){

        int lightType = additionalLightData[i].x;

        if(lightType == 0.0){

            finalColor += computeDirectionalLight(v_texcoord0,
                v_pos, 
                v_normal,
                viewPos.xyz, 
                lightAmbientColor[i],
                lightDiffuseColor[i],
                lightSpecularColor[i],
                lightDirection[i]);

        }
        else if(lightType == 1.0){

            finalColor += computePointLight(v_texcoord0,
                v_pos, 
                v_normal, 
                viewPos.xyz, 
                lightAmbientColor[i],
                lightDiffuseColor[i],
                lightSpecularColor[i],
                lightPos[i],
                lightAttenuation[i]);
        }
        else if(lightType == 2.0){

            float cutoff = additionalLightData[i].y;
            float outerCutoff = additionalLightData[i].z;

            finalColor += computeSpotLight(v_texcoord0,
                v_pos, 
                v_normal, 
                viewPos.xyz, 
                lightAmbientColor[i],
                lightDiffuseColor[i],
                lightSpecularColor[i],
                lightPos[i],
                lightDirection[i],
                lightAttenuation[i],
                cutoff,
                outerCutoff);

        }

    }

    gl_FragColor = finalColor;

}
