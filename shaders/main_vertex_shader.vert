#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in mat4 modelView;

uniform mat4 projectionView;

layout (location = 0) out vec4 fColor;
layout (location = 1) out vec2 texCoords;

void main()
{
	gl_Position = projectionView * modelView * position;
	fColor = color;
	texCoords = uv;
}
