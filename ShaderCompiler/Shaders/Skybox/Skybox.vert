$input a_position, a_texcoord0, a_normal, a_color0
$output v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

void main() {
    v_texcoord0 = a_texcoord0;
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0f));    
}