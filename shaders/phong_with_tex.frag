#version 420 core

layout (location=0) in vec2 inTexture;
layout (location=1) in vec4 inFrontLight;
layout (location=2) in vec4 inBackLight;

layout (location=0) out vec4 fragmentColor;

layout (binding=0) uniform sampler2D tex;

void main()
{
	vec4 baseColor = texture(tex, inTexture);

	if(gl_FrontFacing) {
		fragmentColor = baseColor * inFrontLight;
	} else {
		fragmentColor = baseColor * mix(inBackLight, vec4(1.0, 0.0, 0.0, 1.0), 0.7);
	}
}