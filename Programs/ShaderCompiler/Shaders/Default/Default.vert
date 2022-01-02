$input a_position, a_color0
$output v_color0

uniform mat4 worldMatrix;

#include <common.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, mul(worldMatrix, vec4(a_position, 1.0)));
	v_color0 = a_color0;
}
