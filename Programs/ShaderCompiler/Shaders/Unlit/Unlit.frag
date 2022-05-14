$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

uniform vec4 matAmbient;

void main()
{
    
    gl_FragColor = matAmbient;
	
}
