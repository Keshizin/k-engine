#version 420 core

layout (location=0) in vec2 texCoord;

layout (location=0) out vec4 fragmentColor;

layout (binding=0) uniform sampler2D tex;

void main()
{
	 fragmentColor = texture(tex, texCoord);
}