#version 420 core

layout (location=0) in vec3 vPosition;
layout (location=3) in vec4 vColor;

layout (location=0) out vec4 outColor;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

void main() {
	gl_Position = projectionTransform * viewTransform * modelTransform  * vec4(vPosition, 1.0);
	outColor = vColor;
}