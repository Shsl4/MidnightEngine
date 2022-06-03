$input a_position, a_texcoord0, a_normal, a_color0, a_color1
$output v_pos, v_texcoord0, v_normal, v_color0, v_color1

#include <common.sh>

void main()
{
    
    v_pos = mul(u_model[0], vec4(a_position, 1.0f)).xyz;
    v_normal = mul(transpose(inverse(u_model[0])), a_normal);
    v_color1 = a_color1;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));

    
}