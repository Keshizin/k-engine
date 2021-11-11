varying vec3 normal;
varying vec3 sup;

void main()
{
	// attribute gl_Vertex - variável definida por GLSL que representa o vértice
	// attribute gl_Normal - 
	// attribute gl_Position - 
	// uniform mat3 gl_ModelViewMatrix - 
	// attribute gl_NormalMatrix -
	// uniform gl_ModelViewProjectionMatrix - 
	sup = vec3(gl_ModelViewMatrix * gl_Vertex);
	normal = gl_NormalMatrix * gl_Normal;
	gl_Position = ftransform();
}