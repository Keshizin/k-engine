#version 450 core

layout (location = 0) in vec4 color;
layout (location = 1) in vec2 texCoords;


// "binding" specify the default value for texture unit within the shader
layout (binding = 0) uniform sampler2D tex2D;


uniform int isTexture = 0;


layout (location = 0) out vec4 fragmentColor;


void main()
{
	if(isTexture == 1)
		fragmentColor = texture(tex2D, texCoords);
	else
		fragmentColor = color;
}
