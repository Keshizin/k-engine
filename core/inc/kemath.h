/*
	K-Engine Mathematic Library
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


#ifndef K_ENGINE_MATH_H
#define K_ENGINE_MATH_H

#define _USE_MATH_DEFINES
#include <cmath>


// (!) constant used to convert angle to radian (PI / 180º)
#define K_PI_TO_RADIAN 0.0174532925f

namespace kengine
{
	/*
	* 
	*  kengine::vec3 class
	* 
	*  This class represent a mathematic vector (analytic geometry).
	*
	*		- length (norm or magnitude) - ||V|| = sqrt( x^2 + y^2 + z^2 )
	*		- dot
	*		- dot product (scalar product) - <A, B>
	*		- cross product - C = A x B
	*		- normalize (a unit vector) - V^ = V / ||V||
	*
	*/
	template <typename TYPE>
	class vec3
	{
	public:
		vec3()
			: x{ 0 }, y{ 0 }, z{ 0 } {}
		
		vec3(const TYPE& value)
			: x{ value }, y{ value }, z{ value } {}
		
		vec3(const TYPE& xParam, const TYPE& yParam, const TYPE& zParam)
			: x{ xParam }, y{ yParam }, z{ zParam } {}

		~vec3() {}


		// copy constructor
		vec3(const vec3& copy)
		{
			x = copy.x;
			y = copy.y;
			z = copy.z;
		}


		// move constructor
		vec3(vec3&& move) noexcept
		{
			x = move.x;
			y = move.y;
			z = move.z;
		}


		// copy assignment
		vec3& operator=(const vec3& copy)
		{
			x = copy.x;
			y = copy.y;
			z = copy.z;
			return *this;
		}

		// sobrecarregar aqui +=. -=, +, -
		vec3 operator+(const vec3& op)
		{
			return vec3(
				x + op.x,
				y + op.y,
				z + op.z);
		}


		vec3 operator-(const vec3& op)
		{
			return vec3(
				x - op.x,
				y - op.y,
				z - op.z);
		}


		vec3 operator*(const float& op)
		{
			return vec3(
				x * op,
				y * op,
				z * op);
		}


		float length() {
			return std::sqrt(x * x + y * y + z * z);
		}

		float dot(const vec3& otherVec3)
		{
			return x * otherVec3.x + y * otherVec3.y + z * otherVec3.z;
		}

		void dotProduct(const vec3& otherVec3)
		{
			float len = dot(otherVec3);

			if (len > 0.0f)
			{
				float invert = 1.0f / std::sqrt(len);
				x *= invert;
				y *= invert;
				z *= invert;
			}
		}

		vec3 crossProduct(const vec3& otherVec3)
		{
			return vec3(
				y * otherVec3.z - z * otherVec3.y,
				z * otherVec3.x - x * otherVec3.z,
				x * otherVec3.y - y * otherVec3.x);
		}

		void normalize()
		{
			float len = length();

			if (len > 0.0f)
			{
				float invert = 1.0f / len;
				x *= invert;
				y *= invert;
				z *= invert;
			}
		}
		
		TYPE x;
		TYPE y;
		TYPE z;
	};


	/*
	* 
	*  kengine::matrix class
	* 
	*  This class stores the 4x4 matrix in column-major order.
	* 
	*/
	class matrix {
		friend kengine::matrix translate(float x, float y, float z);
		friend kengine::matrix rotate(float angle, float x, float y, float z);
		friend kengine::matrix rotate(float x, float y, float z);
		friend kengine::matrix scale(float x, float y, float z);
		friend kengine::matrix frustum(const float left, const float right, const float bottom, const float top, const float near, const float far);
		friend kengine::matrix ortho(const float left, const float right, const float bottom, const float top, const float near, const float far);
		friend kengine::matrix perspective(float fovy, float aspectRatio, float near, float far);
		friend kengine::matrix lookAt(kengine::vec3<float> eye, kengine::vec3<float> center, kengine::vec3<float> up);

	public:
		static size_t size() { return 16 * sizeof(float); }

		matrix(); // default constructor
		explicit matrix(float identity); // explicit constructor
		matrix(const matrix& copy); // copy constructor
		matrix(matrix&& move) noexcept; // move constructor
		~matrix(); // destructor

		matrix operator*(const matrix& right);
		matrix& operator=(const matrix& right);
		
		const float * const value() { return m; }

		//void inverse(float* mat);
		//void transpose(float* mat);
		//void inverseTranspose(float* matrix);

	private:
		float* m;
	};


	/*
	* 
	*  struct for rectangle
	* 
	*/
	typedef struct {
		float left;
		float right;
		float top;
		float bottom;
	} rect;


	/*
	* 
	*  function to generate circle points (x, y).
	* 
	*  params:
	*	- count: the number of circle points.
	*	- points: the buffer where the points will be stored.
	*	- offset: the offset to x and y coordinates.
	* 
	*/
	void generateCirclePoints(int count, float *points, float offset);
}

#endif
