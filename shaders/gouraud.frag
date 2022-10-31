#version 450 core

layout (location = 3) in vec3 lightColor;
layout (Location = 4) flat in vec3 flatLightColor;


// lighting model info
uniform struct LightingModelInfo {
	int non_local_viewer;
	int is_two_sided;
	int flat_shading;
	int model_type;
} lightingModel;


layout (location = 0) out vec4 fragmentColor;


void main()
{
	if(lightingModel.flat_shading == 1)
		fragmentColor = vec4(flatLightColor, 1.0);
	else
		fragmentColor = vec4(lightColor, 1.0);
}
