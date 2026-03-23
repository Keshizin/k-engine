#version 420 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec4 vColor;
layout (location=2) in vec2 vTexCoord;
layout (location=3) in vec3 vNormal;
layout (location=12) in mat4 vModelMatrix;

layout (location=0) out vec2 texCoord;
layout (location=1) out vec4 color;

uniform mat4 projection;
uniform mat4 view;

layout (binding = 0) uniform PhongLightInfo {
	vec3 position;
	vec3 La; // ambient light intensity
	vec3 Ld; // diffuse light intensity
	vec3 Ls; // specular light intensity
} Light;

layout (binding = 1) uniform PhongMaterialInfo {
	vec3 Ka; // ambient reflectivity
	vec3 Kd; // diffuse reflectivity
	vec3 Ks; // specular reflectivity
	float shininess; // specular shininess factor
} Material;

void main() {
	mat4 modelView = view * vModelMatrix;
	mat3 normalMatrix = transpose(inverse(mat3(modelView)));

	vec3 tnorm = normalize(normalMatrix * vNormal);
	vec4 eyeCoords = modelView * vec4(vPosition, 1.0);

	vec3 s = normalize(Light.position - eyeCoords.xyz);

	vec3 ambient = Light.La * Material.Ka;

	float sdotn =  max(dot(s, tnorm), 0.0);
	vec3 diffuse = Light.Ld * Material.Kd * sdotn;

	vec3 specular = vec3(0.0);

	if(sdotn > 0.0) {
		vec3 v = normalize(-eyeCoords.xyz);
		vec3 r = reflect(-s, tnorm);
		specular = Light.Ls * Material.Ks * pow(max(dot(r, v), 0.0), Material.shininess);
	}

	vec3 light = ambient + diffuse + specular;

	gl_Position = projection * eyeCoords;
	texCoord = vTexCoord;
	color = vec4(light, 1.0) * vColor;
}