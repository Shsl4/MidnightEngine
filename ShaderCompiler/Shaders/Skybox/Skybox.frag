$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

SAMPLER2D(texDiffuse, 0);

void main(){

    gl_FragColor = texture2D(texDiffuse, vec3(v_texcoord0, 1.0));
	
}
