/*
	K-Engine Mathematic Library
	This file is part of the K-Engine.

	Copyright (C) 2020-2025 Fabio Takeshi Ishikawa

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

#include <k_math.hpp>

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

/*
	kengine::matrix class - member class definition
*/

kengine::matrix::matrix()
	: m{ new float[16] }
{
	//std::cout << "> kengine::matrix default constructor [" << this << "]" << std::endl;

	m[ 0] = 0.0f; m[ 1] = 0.0f; m[ 2] = 0.0f; m[ 3] = 0.0f;
	m[ 4] = 0.0f; m[ 5] = 0.0f; m[ 6] = 0.0f; m[ 7] = 0.0f;
	m[ 8] = 0.0f; m[ 9] = 0.0f; m[10] = 0.0f; m[11] = 0.0f;
	m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 0.0f;
}

kengine::matrix::matrix(float identity)
	: m{ new float[16] }
{
	//std::cout << "> kengine::matrix constructor with 1 argument [" << this << "]" << std::endl;

	m[ 0] = identity; m[ 1] = 0.0f;     m[ 2] = 0.0f;     m[ 3] = 0.0f;
	m[ 4] = 0.0f;     m[ 5] = identity; m[ 6] = 0.0f;     m[ 7] = 0.0f;
	m[ 8] = 0.0f;     m[ 9] = 0.0f;     m[10] = identity; m[11] = 0.0f;
	m[12] = 0.0f;     m[13] = 0.0f;     m[14] = 0.0f;     m[15] = identity;
}

kengine::matrix::matrix(std::initializer_list<float> data)
	: m{ new float[16] }
{
	//std::cout << "> kengine::matrix initializer list constructor [" << this << "]" << std::endl;

	auto first = data.begin();

	for (int i = 0; i < 16; i++) {
		m[i] = 0.0f;

		if (first != data.end()) {
			m[i] = *first;
			first++;
		}
	}
}

kengine::matrix::~matrix()
{
	//std::cout << "> kengine::matrix destructor [" << this << "]" << std::endl;
	delete[] m;
}

kengine::matrix::matrix(const matrix& copy)
	: m{ new float[16] }
{
	//std::cout << "> kengine::matrix copy constructor [" << this << "]" << std::endl;

	for (int i = 0; i < 16; i++) {
		m[i] = copy.m[i];
	}
}

kengine::matrix::matrix(matrix&& move) noexcept
	: m{ move.m }
{
	//std::cout << "> kengine::matrix move constructor [" << this << "]" << std::endl;
	move.m = nullptr;
}

kengine::matrix& kengine::matrix::operator=(const kengine::matrix& right)
{
	//std::cout << "> kengine::matrix overload operator= (copy) [" << this << "]" << std::endl;

	for (int i = 0; i < 16; i++) {
		m[i] = right.m[i];
	}

	return *this;
}

kengine::matrix& kengine::matrix::operator=(matrix&& right) noexcept
{
	//std::cout << "> kengine::matrix overload operator= (move) [" << this << "]" << std::endl;

	delete[] m;
	m = right.m;
	right.m = nullptr;
	return *this;
}

void kengine::matrix::print() const
{
	std::cout << "> kengine::matrix [" << this << "]\n" << std::endl;

	for (int i = 0; i < 4; i++) {
		std::cout << "\t";

		for (int j = 0; j < 4; j++) {
			std::cout << "[" << m[j + (i * 4)] << "]";
		}

		std::cout << std::endl;
	}

	std::cout << std::endl;
}


/*
	kengine::matrix friend functions
*/

// (!) this function set the matrix in coloumn-major order
kengine::matrix kengine::rotate(float angle, float x, float y, float z)
{
	kengine::matrix r(1.0f);

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

	r.m[8] = (x * z * omc + y * sin_angle);
	r.m[9] = (y * z * omc - x * sin_angle);
	r.m[10] = (z2 * omc + cos_angle);
	r.m[11] = 0.0f;

	r.m[12] = 0.0f;
	r.m[13] = 0.0f;
	r.m[14] = 0.0f;
	r.m[15] = 1.0f;

	return r;
}

//kengine::matrix kengine::lookAt(kengine::vec3<float> eye, kengine::vec3<float> center, kengine::vec3<float> up)
//{
//	eye.z = -eye.z;
//
//	// computing forward vector
//	kengine::vec3<float> forward = center - eye;
//	forward.normalize();
//
//	// computing right vector
//	kengine::vec3<float> right = forward.crossProduct(up);
//	right.normalize();
//
//	// computing the true up vector
//	up = right.crossProduct(forward);
//	up.normalize();
//
//	kengine::matrix lookMatrix(1);
//
//	// look at matrix in column-major order and right-hand coordinate system
//	// 
//	// [ rightX ] [ upX ] [ forwardX ] [ 0.0 ]
//	// [ rightY ] [ upY ] [ forwardY ] [ 0.0 ]
//	// [ rightZ ] [ upZ ] [ forwardZ ] [ 0.0 ]
//	// [  0.0   ] [ 0.0 ] [   0.0    ] [ 1.0 ]
//
//	lookMatrix.m[0] = right.x;
//	lookMatrix.m[1] = up.x;
//	lookMatrix.m[2] = -forward.x;
//	lookMatrix.m[3] = 0.0f;
//
//	lookMatrix.m[4] = right.y;
//	lookMatrix.m[5] = up.y;
//	lookMatrix.m[6] = -forward.y;
//	lookMatrix.m[7] = 0.0f;
//
//	lookMatrix.m[8] = right.z;
//	lookMatrix.m[9] = up.z;
//	lookMatrix.m[10] = -forward.z;
//	lookMatrix.m[11] = 0.0f;
//
//	lookMatrix.m[12] = 0.0f;
//	lookMatrix.m[13] = 0.0f;
//	lookMatrix.m[14] = 0.0f;
//	lookMatrix.m[15] = 1.0f;
//
//	kengine::matrix t = kengine::translate(-eye.x, -eye.y, -eye.z);
//
//	return lookMatrix * t;
//}