#version 450 core

// total of lights
const int TOTAL_OF_LIGHTS = 10;
const float PI = 3.14159265358979323846;

layout (location = 2) in vec3 transfomedNormal;
layout (location = 3) in vec3 eyePosition;


// PBR Light Info
uniform struct PBRLightInfo {
	vec3 position;
	vec3 L;
} lights[TOTAL_OF_LIGHTS];


// PBR Material Info
uniform struct PBRMaterial {
	float rough; // roughness
	int metal; // mettalic (true) or dieletric (false)
	vec3 color; // diffuse color for dielectrics, f0 for mettalic
} pbrMaterial;


uniform mat4 viewMatrix;


layout (location = 0) out vec4 fragmentColor;


// function for the Fresnel term using the Schlick approximation
vec3 schlickFresnel(float lDotH) {
	vec3 f0 = vec3(0.04); // dieletrics

	if(pbrMaterial.metal == 1)
	{
		f0 = pbrMaterial.color;
	}

	return f0 + (1.0 - f0) * pow(1.0 - lDotH, 5.0);
}


// function for the geometry term G
float geoSmith(float dotProd) {
	float k = (pbrMaterial.rough + 1.0) * (pbrMaterial.rough + 1.0) / 8.0;
	float denom = dotProd * (1 - k) + k;
	return 1.0 / denom;
}


// the normal distribution function D, based on GGX/Trowbridge-Reitz
float ggxDistribution(float nDotH) {
	float alpha2 = pbrMaterial.rough * pbrMaterial.rough * pbrMaterial.rough * pbrMaterial.rough;
	float d = (nDotH * nDotH) * (alpha2 - 1) + 1;
	return alpha2 / (3.141592653 * d * d);
}


// function that computes the entire model for a single light source
vec3 microfacedModel(int light, vec3 position, vec3 n) {
	vec3 diffuseBRDF = vec3(0.0); // metallic

	if(pbrMaterial.metal == 0) {
		diffuseBRDF = pbrMaterial.color;
	}

	vec3 l = vec3(0.0);
	vec3 lightI = lights[light].L;

	vec3 lightPosition = (viewMatrix * vec4(lights[light].position, 1.0)).xyz;
	l = lightPosition - position;

	float dist = length(l);
	l = normalize(l);
	lightI /= (dist * dist);

	vec3 v = normalize(-position);
	vec3 h = normalize(v + l);
	float nDotH = dot(n, h);
	float lDotH = dot(l, h);
	float nDotL = max(dot(n, l), 0.0);
	float nDotV = dot(n, v);

	vec3 specBRDF = 0.25 * ggxDistribution(nDotH) * schlickFresnel(lDotH) * geoSmith(nDotL) * geoSmith(nDotV);

	return (diffuseBRDF + PI * specBRDF) * lightI * nDotL;
}


void main()
{	
	vec3 sum = vec3(0.0);

	for(int i = 0; i < 2; i++)
	{
		sum += microfacedModel(i, eyePosition, transfomedNormal);
	}

	// gamma correction
	sum = pow(sum, vec3(1.0 / 2.2));

	fragmentColor = vec4(sum, 1.0);
}
