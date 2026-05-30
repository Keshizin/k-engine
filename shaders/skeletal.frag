#version 420 core

layout (location=0) in vec4 inColor;

layout (location=0) out vec4 fragmentColor;

void main()
{
	fragmentColor = vec4(0.0, 0.0, 0.0, 1.0);
}