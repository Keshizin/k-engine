#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 uv;
layout (location = 3) in mat4 modelView; // one modelview matrix per instance

uniform mat4 projectionView;

layout (location = 0) out vec4 fColor;
layout (location = 1) out vec2 texCoords;

void main()
{
	gl_Position = projectionView * modelView * vec4(position, 1.0);
	fColor = color;
	texCoords = uv;
}
