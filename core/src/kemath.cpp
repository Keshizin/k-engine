/*
	K-Engine Mathematics Library
	This file is part of the K-Engine.

	Copyright (C) 2022 Fabio Takeshi Ishikawa

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/


#include <kemath.h>
#include <cmath>


// matrix memory layout
// [  0 ] [  1 ] [  2 ] [  3 ]
// [  4 ] [  5 ] [  6 ] [  7 ]
// [  8 ] [  9 ] [ 10 ] [ 11 ]
// [ 12 ] [ 13 ] [ 14 ] [ 15 ]


// (!) this function set the matrix in coloumn-major order
kengine::matrix kengine::translate(float x, float y, float z)
{
	// translate matrix in column-major order
	// [ 1.0 ] [ 0.0 ] [ 0.0 ] [ 0.0 ]
	// [ 0.0 ] [ 1.0 ] [ 0.0 ] [ 0.0 ]
	// [ 0.0 ] [ 0.0 ] [ 1.0 ] [ 0.0 ]
	// [  x  ] [  y  ] [  z  ] [ 1.0 ]

	kengine::matrix t(1);
	t.m[12] = x;
	t.m[13] = y;
	t.m[14] = z;
	return t;
}


// (!) this function set the matrix in coloumn-major order
kengine::matrix kengine::rotate(float angle, float x, float y, float z)
{
	kengine::matrix r(1);

	float x2 = x * x;
	float y2 = y * y;
	float z2 = z * z;
	float rad = angle * K_PI_TO_RADIAN;
	float cos_angle = std::cos(rad);
	float sin_angle = std::sin(rad);
	float omc = 1.0f - cos_angle;

	r.m[0] = (x2 * omc + cos_angle);
	r.m[1] = (y * x * omc + z * sin_angle);
	r.m[2] = (x * z * omc - y * sin_angle);
	r.m[3] = 0.0f;

	r.m[4] = (x * y * omc - z * sin_angle);
	r.m[5] = (y2 * omc + cos_angle);
	r.m[6] = (y * z * omc + x * sin_angle);
	r.m[7] = 0.0f;

	r.m[ 8] = (x * z * omc + y * sin_angle);
	r.m[ 9] = (y * z * omc - x * sin_angle);
	r.m[10] = (z2 * omc + cos_angle);
	r.m[11] = 0.0f;

	r.m[12] = 0.0f;
	r.m[13] = 0.0f;
	r.m[14] = 0.0f;
	r.m[15] = 1.0f;

	return r;
}


// (!) this function set the matrix in coloumn-major order
kengine::matrix kengine::rotate(float x, float y, float z)
{
	kengine::matrix r(1);

	if (x > 0.0f)
	{
		// convert to radians
		x *= K_PI_TO_RADIAN;

		// rotate in x axis (column-major order)
		// [ 1.0 ] [   0.0   ] [  0.0   ] [ 0.0 ]
		// [ 0.0 ] [  cos(x) ] [ sin(x) ] [ 0.0 ]
		// [ 0.0 ] [ -sin(x) ] [ cos(x) ] [ 0.0 ]
		// [ 0.0 ] [   0.0   ] [  0.0   ] [ 1.0 ]

		r.m[5] = std::cos(x);
		r.m[6] = std::sin(x);
		r.m[9] = -std::sin(x);
		r.m[10] = std::cos(x);
	}

	if (y > 0.0f)
	{
		// convert to radians
		y *= K_PI_TO_RADIAN;

		// rotate in y axis (column-major order)
		// [ cos(y) ] [ 0.0 ] [ -sin(y) ] [ 0.0 ]
		// [  0.0   ] [ 1.0 ] [   0.0   ] [ 0.0 ]
		// [ sin(y) ] [ 0.0 ] [  cos(y) ] [ 0.0 ]
		// [  0.0   ] [ 0.0 ] [   0.0   ] [ 1.0 ]
	
		r.m[0] = std::cos(y);
		r.m[2] = -std::sin(y);
		r.m[8] = std::sin(y);
		r.m[10] = std::cos(y);
	}

	if (z > 0.0f)
	{
		// convert to radians
		z *= K_PI_TO_RADIAN;
		
		// rotate in z axis (column-major order)
		// [  cos(z) ] [ sin(z) ] [ 0.0 ] [ 0.0 ]
		// [ -sin(z) ] [ cos(z) ] [ 0.0 ] [ 0.0 ]
		// [   0.0   ] [  0.0   ] [ 1.0 ] [ 0.0 ]
		// [   0.0   ] [  0.0   ] [ 0.0 ] [ 1.0 ]

		r.m[0] = std::cos(z);
		r.m[1] = std::sin(z);
		r.m[4] = -std::sin(z);
		r.m[5] = std::cos(z);
	}

	return r;
}


// (!) this function set the matrix in coloumn-major order
kengine::matrix kengine::scale(float x, float y, float z)
{
	// scale matrix in column-major order
	// [  x  ] [ 0.0 ] [ 0.0 ] [ 0.0 ]
	// [ 0.0 ] [  y  ] [ 0.0 ] [ 0.0 ]
	// [ 0.0 ] [ 0.0 ] [  z  ] [ 0.0 ]
	// [ 0.0 ] [ 0.0 ] [ 0.0 ] [ 1.0 ]

	kengine::matrix s(1);
	s.m[0] = x;
	s.m[5] = y;
	s.m[10] = z;
	return s;
}


// (!) this function set the matrix in coloumn-major order
kengine::matrix kengine::frustum(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	kengine::matrix p(1);

	// frustum matrix in column-major order
	// [ (2.0f * n) / (right - left)     ] [ 0.0                             ] [ 0.0                       ] [  0.0 ]
	// [ 0.0                             ] [ (2.0f * n) / (top - bottom)     ] [ 0.0                       ] [  0.0 ]
	// [ (right + left) / (right - left) ] [ (top + bottom) / (top - bottom) ] [ -(f + n) / (f - n)        ] [ -1.0 ]
	// [ 0.0                             ] [  0.0                            ] [ -(2.0f * f * n) / (f - n) ] [  0.0 ]

	p.m[0] = (2.0f * near) / (right - left);
	p.m[5] = (2.0f * near) / (top - bottom);
	p.m[8] = (right + left) / (right - left);
	p.m[9] = (top + bottom) / (top - bottom);
	p.m[10] = -(far + near) / (far - near);
	p.m[11] = -1.0f;
	p.m[14] = -(2.0f * far * near) / (far - near);
	p.m[15] = 0.0f;

	return p;
}


// (!) this function set the matrix in coloumn-major order
kengine::matrix kengine::ortho(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
	kengine::matrix p;
		
	p.m[0] = 2.0f / (right - left);
	p.m[5] = 2.0f / (top - bottom);
	p.m[10] = -2.0f / (far - near);
	p.m[12] = -(right + left) / (right - left);
	p.m[13] = -(top + bottom) / (top - bottom);
	p.m[14] = -(far + near) / (far - near);
	p.m[15] = 1.0f;

	return p;
}


kengine::matrix kengine::lookAt(const vec3<float>& right, const vec3<float>& up, const vec3<float>& forward, const vec3<float>& from)
{
	kengine::matrix l(1);

	// look at matrix in column-major order
	// [ rightX   ] [ rightY   ] [ rightZ   ] [  0.0 ]
	// [ upX      ] [ upY      ] [ upZ      ] [  0.0 ]
	// [ forwardX ] [ forwardY ] [ forwardZ ] [  0.0 ]
	// [ fromX    ] [ fromY    ] [ fromZ    ] [  1.0 ]

	l.m[0] = right.x;
	l.m[1] = right.y;
	l.m[2] = right.z;

	l.m[4] = up.x;
	l.m[5] = up.y;
	l.m[6] = up.z;

	l.m[ 8] = forward.x;
	l.m[ 9] = forward.y;
	l.m[10] = forward.z;

	l.m[12] = from.x;
	l.m[13] = from.y;
	l.m[14] = from.z;

	return l;
}


// ----------------------------------------------------------------------------
//  (!) kengine::matrix class - member class definition
// ----------------------------------------------------------------------------
kengine::matrix::matrix()
	: m{ new float[16] }
{
	m[ 0] = 0.0f; m[ 1] = 0.0f; m[ 2] = 0.0f; m[ 3] = 0.0f;
	m[ 4] = 0.0f; m[ 5] = 0.0f; m[ 6] = 0.0f; m[ 7] = 0.0f;
	m[ 8] = 0.0f; m[ 9] = 0.0f; m[10] = 0.0f; m[11] = 0.0f;
	m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 0.0f;
}


kengine::matrix::matrix(float identity)
	: m{ new float[16] }
{
	m[0] = identity; m[1] = 0.0f; m[2] = 0.0f; m[3] = 0.0f;
	m[4] = 0.0f; m[5] = identity; m[6] = 0.0f; m[7] = 0.0f;
	m[8] = 0.0f; m[9] = 0.0f; m[10] = identity; m[11] = 0.0f;
	m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = identity;
}


kengine::matrix::matrix(const matrix& copy)
	: m{ new float[16] }
{
	for (int i = 0; i < 16; i++)
	{
		m[i] = copy.m[i];
	}
}


// move constructor
kengine::matrix::matrix(matrix&& move) noexcept
	: m{ move.m }
{
	move.m = nullptr;
}


kengine::matrix::~matrix()
{
	delete m;
}


kengine::matrix kengine::matrix::operator*(const matrix& right)
{
	kengine::matrix result;
	int indices = 0;

	// column major multiplication
	for (int z = 0; z < 4; z++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int i = 0; i < 4; i++)
			{
				result.m[indices] += this->m[(i * 4) + j] * right.m[i + (z * 4)];
			}

			indices++;
		}
	}

	return result;
}


kengine::matrix& kengine::matrix::operator=(const kengine::matrix& right)
{
	delete m;
	m = new float[16];

	for (int i = 0; i < 16; i++)
	{
		m[i] = right.m[i];
	}

	return *this;
}