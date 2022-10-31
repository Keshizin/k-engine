#version 450 core

// attribute variables
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 baseColor;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normal;
layout (location = 4) in mat4 modelMatrix; // one modelview matrix per instance


// uniform variable data
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


// out variables
layout (location = 2) out vec3 out_transfomedNormal; // interpolate the normal vector across the polygon
layout (location = 3) out vec3 out_eyePosition; // interpolate the position across the polygon


void main()
{
	mat4 modelViewMatrix = viewMatrix * modelMatrix;

	// getting the normal matrix transformation from modelview matrix transformation
	mat3 eyeMatrix = transpose(inverse(mat3(modelViewMatrix)));

	out_transfomedNormal = normalize(eyeMatrix * normal); // must be in eye space
	out_eyePosition = (modelViewMatrix * vec4(position, 1.0)).xyz; // must be in eye space

	// column-major order -> proj * view * model * position
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}
