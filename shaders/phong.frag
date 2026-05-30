#version 460

layout (location=1) in vec4 inFrontLight;
layout (location=2) in vec4 inBackLight;
layout (location=3) in vec4 inColor;

layout (location=0) out vec4 fragmentColor;

void main(void) {
	if(gl_FrontFacing) {
		fragmentColor = inFrontLight;
	} else {
		fragmentColor = mix(inBackLight, vec4(0.5, 0.5, 0.5, 1.0), 0.3);
	}
}