#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 baseColor;

uniform mat4 projectionMatrix;

layout (location = 0) out vec4 out_baseColor;

void main()
{
	gl_Position = projectionMatrix * vec4(position, 1.0);
	out_baseColor = baseColor;
}
