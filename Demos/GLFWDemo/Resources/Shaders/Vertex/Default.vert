#version 410 core

uniform mat4 viewMatrix;

layout (location = 0) in vec4 inColor;
layout (location = 1) in vec3 position;

out vec4 color;

void main()
{
    gl_Position = viewMatrix * vec4(position, 1.0);
    color = inColor;
}
