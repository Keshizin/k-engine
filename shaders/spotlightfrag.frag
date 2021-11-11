varying vec3 normal;
varying vec3 sup;
uniform float interna;

void main(void)
{
	// Calcula um vetor da superfície à luz
	vec3 luz = normalize(gl_LightSource[0].position.xyz - sup); 

	// O vetor da superfície ao observador é simplesmente o oposto de sup
	// pois já estamos em coordenadas de câmera: observador = (0,0,0)
	vec3 obs = normalize(-sup);
	
	// Calcula vetor médio
	vec3 medio = normalize(luz+obs);

	// Normaliza a normal interpolada
	normal = normalize(normal);

	// Calcula ângulo entre direção do spot e vetor da luz à superfície
	float spot = max(dot(gl_LightSource[0].spotDirection, -luz),0.0);

	// Calcula intensidade da luz considerando a variação da borda interna à externa	
	// Se estiver fora do cone de luz, "borda" recebe 0.0
	float borda = smoothstep(gl_LightSource[0].spotCosCutoff,interna,spot);

	// Aplica GL_SPOT_EXPONENT ao resultado
	spot = pow(spot, gl_LightSource[0].spotExponent) * borda;

	// Calcula componente difuso
	vec4 difuso = gl_FrontLightProduct[0].diffuse * max(dot(normal,luz), 0.0);
		
	// Calcula componente especular
	vec4 especular = gl_FrontLightProduct[0].specular 
		* pow(max(dot(normal,medio),0.0),gl_FrontMaterial.shininess);

	// Multiplica pela intensidade do spot e pela combinação difuso+especular e adiciona
	// à contribuição do componente ambiente do modelo de iluminação para gerar a cor final
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + gl_FrontLightProduct[0].ambient
		+ spot * (difuso+especular);
}