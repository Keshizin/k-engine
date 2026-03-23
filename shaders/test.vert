#version 460

layout (location=0) in vec3 vPosition;
layout (location=1) in vec4 vColor;
layout (location=12) in mat4 vModelTransform;

layout (location=0) out vec4 outColor;

uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main(void) {
	mat4 t = mat4(1.0);
	gl_Position = projectionTransform * viewTransform * vModelTransform * vec4(vPosition, 1.0);
	outColor = vColor;
}