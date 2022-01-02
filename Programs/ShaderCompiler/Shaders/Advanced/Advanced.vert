$input a_position, a_normal, a_texcoord0, a_color0
$output v_color0, v_normal, v_texcoord0, v_pos

uniform mat4 modelMatrix;

#include <common.sh>

void main()
{
    
    vec4 modelViewPos = mul(u_modelView, vec4(a_position, 1.0));
	gl_Position = mul(u_proj, modelViewPos);
    v_texcoord0 = a_texcoord0;
	v_normal = normalize(mul(u_modelView, vec4(a_normal, 0.0))).xyz;
    v_pos = modelViewPos.xyz;
    
    v_color0 = a_color0;

}
