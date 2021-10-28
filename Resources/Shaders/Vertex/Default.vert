#version 110

uniform mat4 viewMatrix;

attribute vec4 inColor;
attribute vec2 position;

varying vec4 color;

void main()
{
    gl_Position = viewMatrix * vec4(position, 0.0, 1.0);
    color = inColor;
}
