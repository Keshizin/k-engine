#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 baseColor;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 modelMatrix; // one modelview matrix per instance

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

layout (location = 0) out vec4 out_baseColor;
layout (location = 1) out vec2 out_texCoord;

void main()
{
	out_baseColor = baseColor;
	out_texCoord = uv;

	// column-major order -> proj * view * model * position
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}
