#version 450 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_color;

uniform mat4 projectionView;

layout (location = 0) out vec4 out_color;

void main()
{
	gl_Position = projectionView * in_position;
	out_color = in_color;
}
