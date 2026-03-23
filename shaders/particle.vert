#version 460

layout (location=0) in vec3 vPosition;
layout (location=1) in vec4 vColor;

layout (location=0) out vec4 outColor;

uniform float currentTime = 0.0;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

mat4 translate(float x, float y, float z);
mat4 rotate(float x, float y, float z);

void main(void) {
	float x = sin(0.02 * (currentTime + gl_InstanceID)) * 40.0f;
	float y = cos(0.07 * (currentTime + gl_InstanceID)) * 20.0f;
	float z = sin(0.10 * (currentTime + gl_InstanceID)) * 10.0f;
	mat4 t = translate(x, y, z);

	float speed = 25.0;
	mat4 r = rotate(0.0, speed * currentTime, 0.0) * rotate(speed * currentTime, 0.0, 0.0) * rotate(0.0, 0.0, speed * currentTime);
	mat4 mTransform = t * r;

	gl_Position = projectionTransform * viewTransform * mTransform * vec4(vPosition, 1.0);
	outColor = vColor;
}

mat4 translate(float x, float y, float z)
{
	mat4 t = mat4(1.0);
	t[3][0] = x;
	t[3][1] = y;
	t[3][2] = z;
	return t;
}

mat4 rotate(float x, float y, float z)
{
	mat4 r = mat4(1.0f);

	if(x > 0.0f)
	{
		mat4 rx = mat4(1.0f);
		x *= 0.0174532925f;
		rx[1][1] = cos(x);
		rx[1][2] = sin(x);
		rx[2][1] = -sin(x);
		rx[2][2] = cos(x);
		r = rx * r;
	}

	if(y > 0.0f)
	{
		mat4 ry = mat4(1.0f);
		y *= 0.0174532925f;
		ry[0][0] = cos(y);
		ry[0][2] = -sin(y);
		ry[2][0] = sin(y);
		ry[2][2] = cos(y);
		r = ry * r;
	}

	if(z > 0.0f)
	{
		mat4 rz = mat4(1.0f);
		z *= 0.0174532925f;
		rz[0][0] = cos(z);
		rz[0][1] = sin(z);
		rz[1][0] = -sin(z);
		rz[1][1] = cos(z);
		r = rz * r;
	}

	return r;
}