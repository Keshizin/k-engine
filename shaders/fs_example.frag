#version 420 core

/*
	PASS-THROUGH SHADER
*/

layout (location=0) in vec2 texCoord;
layout (location=1) in vec4 color;

layout (location=0) out vec4 fragmentColor;

layout (binding = 0) uniform BlobSettings {
	vec4 InnerColor;
	vec4 OuterColor;
	float RadiusInner;
	float RadiusOuter;
};

void main()
{
	float dx = texCoord.x - 0.5;
	float dy = texCoord.y - 0.5;
	float distance = sqrt(dx * dx + dy * dy);

	float smoothColor = smoothstep(RadiusInner, RadiusOuter, distance);
	fragmentColor = mix(InnerColor, OuterColor, smoothColor);
}