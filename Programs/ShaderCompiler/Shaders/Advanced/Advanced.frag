$input v_color0, v_normal, v_texcoord0, v_pos

#include <common.sh>

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
	float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(lightPos - v_pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    //float specularStrength = 0.5;
    //vec3 viewDir = normalize(viewPos - v_pos);
    //vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse) * objectColor;
    gl_FragColor = vec4(result, 1.0);
	
}
