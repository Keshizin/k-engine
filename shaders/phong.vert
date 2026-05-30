#version 420 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec2 vTexture;
layout (location=2) in vec3 vNormal;

layout (location=0) out vec2 outTexture;
layout (location=1) out vec4 outFrontLight;
layout (location=2) out vec4 outBackLight;

uniform mat4 projectionTransform;
uniform mat4 viewTransform;
uniform mat4 modelTransform;

uniform struct PhongLight {
	vec3 position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
} Light;

uniform struct PhongMaterial {
	float shininess;
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
} Material;

vec3 phongModel(vec3 s, vec3 n, vec4 pos) {
	float sn = max(dot(s, n), 0.0);

	vec3 ambientLight = Light.La * Material.Ka;
	vec3 diffuseLight = Light.Ld * Material.Kd * sn;
	vec3 specularLight = vec3(0.0);

	if(sn > 0.0) {
		// vec3 v = vec3(0.0, 0.0, 1.0);
		vec3 v = normalize(-pos.xyz);
		vec3 r = reflect(-s, n);
		specularLight = Light.Ls * Material.Ks * pow(max(dot(r, v), 0.0), Material.shininess);
	}

	return ambientLight + diffuseLight + specularLight;
}

void main() {
	mat4 modelViewTransform = viewTransform * modelTransform;
	mat3 normalTransform = transpose(inverse(mat3(modelViewTransform)));
	vec3 n = normalize(normalTransform * vNormal);

	// calculate the direction from vertex to light source
	vec4 lightPosition = viewTransform * vec4(Light.position, 1.0);
	vec4 position = modelViewTransform * vec4(vPosition, 1.0);
	vec3 s = normalize(vec3(lightPosition.xyz - position.xyz));

	outFrontLight = vec4(phongModel(s, n, position), 1.0);
	outBackLight = vec4(phongModel(s, -n, position), 1.0);

	gl_Position = projectionTransform * position;
	outTexture = vTexture;
}