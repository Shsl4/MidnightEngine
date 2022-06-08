$input v_pos, v_texcoord0, v_normal, v_color0

#include <common.sh>

SAMPLER2D(texDiffuse, 0);

uniform vec4 matAmbient;

// x = lightCount, y = hasTexturem z = has normalTexture
uniform vec4 additionalData;

#define lightCount additionalData[0]
#define hasTexture additionalData[1]

void main() {
    
    if(hasTexture){
        gl_FragColor = texture2D(texDiffuse, v_texcoord0);
    }
    else{
        gl_FragColor = matAmbient;
    }
	
}
