$input v_pos, v_texcoord0, v_normal, v_color0, v_color1

#include <common.sh>

struct Material{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform vec4 matAmbient;
uniform vec4 matDiffuse;
uniform vec4 matSpecular;
uniform vec4 matShininess;

uniform vec4 viewPos; 

uniform vec4 lightDirection; 
uniform vec4 lightAmbientColor;
uniform vec4 lightDiffuseColor;
uniform vec4 lightSpecularColor;

void main() {
    
	vec3 color = matAmbient.xyz;
	float opacity = 1.0;
	float thickness = 1.5;

	if (gl_FrontFacing) { opacity *= 0.5; }

	vec3 fw = abs(dFdx(v_color1)) + abs(dFdy(v_color1));
	vec3 val = smoothstep(vec3_splat(0.0), fw*thickness, v_color1);
	float edge = min(min(val.x, val.y), val.z); // Gets to 0.0 around the edges.

	vec4 rgba = vec4(color, (1.0-edge)*opacity);
	gl_FragColor = rgba;//vec4(v_color1, 1.0);
	
}
