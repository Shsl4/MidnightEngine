$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

SAMPLERCUBE(skybox, 0);
uniform mat4 u_mtx;
void main(){

    gl_FragColor = textureCube(skybox, vec3(v_texcoord0, 1.0));
	
}
