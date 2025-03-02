#version 420 core

/*
	PASS-THROUGH SHADER
*/

layout (location=0) in vec3 vPosition;
layout (location=1) in vec4 vColor;
layout (location=2) in vec2 vTexCoord;

layout (location=0) out vec2 texCoord;
layout (location=1) out vec4 color;

uniform mat4 projection;
uniform mat4 eye;
uniform mat4 model;

void main() {
	gl_Position = projection * eye * model * vec4(vPosition, 1.0);
	texCoord = vTexCoord;
	color = vColor;
}