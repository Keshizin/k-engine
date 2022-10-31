#version 450 core

layout (location = 0) in vec4 in_baseColor;

layout (location = 0) out vec4 out_fragmentColor;

void main()
{
	out_fragmentColor = in_baseColor;
}
