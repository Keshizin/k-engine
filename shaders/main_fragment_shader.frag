#version 450 core

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texCoords;

uniform sampler2D tex;

layout (location = 0) out vec4 fColor;

void main()
{
	//fColor = color;
	fColor = texture(tex, texCoords);
}
