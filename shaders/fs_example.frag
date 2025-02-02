#version 420 core

/*
	PASS-THROUGH SHADER
*/

layout (location=0) in vec2 texCoord;
layout (location=1) in vec4 color;

layout (location=0) out vec4 fragmentColor;

void main()
{
	fragmentColor = color;
}