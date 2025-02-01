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

#ifndef K_ENGINE_MATH_HPP
#define K_ENGINE_MATH_HPP

#include <initializer_list>
#include <cstddef>

// (!) constant used to convert angle to radian (PI / 180º)
#define K_PI_TO_RADIAN 0.0174532925f

namespace kengine
{
	/*
		kengine::vec3 class

		This class represent a mathematic vector (analytic geometry).

			- length (norm or magnitude) - ||V|| = sqrt( x^2 + y^2 + z^2 )
			- dot
			- dot product (scalar product) - <A, B>
			- cross product - C = A x B
			- normalize (a unit vector) - V^ = V / ||V||

	*/
	//template <typename TYPE>
	//class vec3
	//{
	//public:
	//	vec3()
	//		: x{ 0 }, y{ 0 }, z{ 0 } {}

	//	explicit vec3(const TYPE& value)
	//		: x{ value }, y{ value }, z{ value } {}

	//	vec3(const TYPE& xParam, const TYPE& yParam, const TYPE& zParam)
	//		: x{ xParam }, y{ yParam }, z{ zParam } {}

	//	~vec3() {}


	//	vec3 operator+(const vec3& op)
	//	{
	//		return vec3(
	//			x + op.x,
	//			y + op.y,
	//			z + op.z);
	//	}


	//	//vec3 operator-(const vec3& op)
	//	//{
	//	//	return vec3(
	//	//		x - op.x,
	//	//		y - op.y,
	//	//		z - op.z);
	//	//}


	//	vec3 operator*(const TYPE& op)
	//	{
	//		return vec3(
	//			this->x * op,
	//			this->y * op,
	//			this->z * op);
	//	}


	//	vec3 operator*(const vec3& op)
	//	{
	//		return vec3(
	//			this->x * op.x,
	//			this->y * op.y,
	//			this->z * op.z);
	//	}

	//	//vec3 operator*(const matrix& m)
	//	//{
	//		//vec3<TYPE> result;

	//		//result.x = (m.m[0] * x) + (m.m[1] * y) + (m.m[ 2] * z) + (m.m[ 3] * 1);
	//		//result.y = (m.m[4] * x) + (m.m[5] * y) + (m.m[ 6] * z) + (m.m[ 7] * 1);
	//		//result.z = (m.m[8] * x) + (m.m[9] * y) + (m.m[10] * z) + (m.m[11] * 1);

	//		//return result;
	//	//}


	//	//float length() {
	//	//	return std::sqrt(x * x + y * y + z * z);
	//	//}

	//	//float dot(const vec3& otherVec3)
	//	//{
	//	//	return x * otherVec3.x + y * otherVec3.y + z * otherVec3.z;
	//	//}

	//	//void dotProduct(const vec3& otherVec3)
	//	//{
	//	//	float len = dot(otherVec3);

	//	//	if (len > 0.0f)
	//	//	{
	//	//		float invert = 1.0f / std::sqrt(len);
	//	//		x *= invert;
	//	//		y *= invert;
	//	//		z *= invert;
	//	//	}
	//	//}

	//	//vec3 crossProduct(const vec3& otherVec3)
	//	//{
	//	//	return vec3(
	//	//		y * otherVec3.z - z * otherVec3.y,
	//	//		z * otherVec3.x - x * otherVec3.z,
	//	//		x * otherVec3.y - y * otherVec3.x);
	//	//}

	//	//void normalize()
	//	//{
	//	//	float len = length();

	//	//	if (len > 0.0f)
	//	//	{
	//	//		float invert = 1.0f / len;
	//	//		x *= invert;
	//	//		y *= invert;
	//	//		z *= invert;
	//	//	}
	//	//}

	//	TYPE x;
	//	TYPE y;
	//	TYPE z;
	//};

	/*
		kengine::matrix class
		This class stores a float 4x4 matrix in row-major order.

		Reference: https://stackoverflow.com/questions/22145037/matrix-multiplication-as-column-major
	*/
	class matrix {
		//friend kengine::matrix translate(float x, float y, float z);
		//friend kengine::matrix scale(float x, float y, float z);
		friend kengine::matrix rotate(float angle, float x, float y, float z);
		//friend kengine::matrix rotate(float x, float y, float z);
		//friend kengine::matrix ortho(const float left, const float right, const float bottom, const float top, const float near, const float far);
		//friend kengine::matrix frustum(const float left, const float right, const float bottom, const float top, const float near, const float far);
		//friend kengine::matrix perspective(float fovy, float aspectRatio, float near, float far);
		//friend kengine::matrix lookAt(kengine::vec3<float> eye, kengine::vec3<float> center, kengine::vec3<float> up);

	public:
		static size_t size() { return 16 * sizeof(float); }

		matrix();
		explicit matrix(float identity);
		explicit matrix(std::initializer_list<float> data);

		~matrix();

		matrix(const matrix& copy); // copy constructor
		matrix(matrix&& move) noexcept; // move constructor

		matrix& operator=(const matrix& right); // copy assignment
		matrix& operator=(matrix&&) noexcept; // move assigment
		
		float* value() { return m; }

		//matrix operator*(const matrix& right);
		//float& operator[](int i) { return m[i]; }
		//void inverse(float* mat);
		//void transpose(float* mat);
		//void inverseTranspose(float* matrix);

		void print() const;

	private:
		float* m;
	};

	/*
		min returns the minimum of the two parameters. It returns y if y is less than x, otherwise it returns x.
	*/
	template<typename TYPE>
	TYPE min(TYPE x, TYPE y) {
		return (y < x ? y : x);
	}

	/*
		max returns the maximum of the two parameters. It returns y if y is greater than x, otherwise it returns x.
	*/
	template<typename TYPE>
	TYPE max(TYPE x, TYPE y) {
		return (y > x ? y : x);
	}

	/*
		clamp returns the value of x constrained to the range minVal to maxVal.
	*/
	template<typename TYPE>
	TYPE clamp(TYPE x, TYPE minValue, TYPE maxValue) {
		return min<TYPE>(max<TYPE>(x, minValue), maxValue);
	}

	/*
		smoothstep performs smooth Hermite interpolation between 0 and 1 when edge0 < x < edge1
	*/
	template <typename TYPE>
	TYPE smoothstep(TYPE edge0, TYPE edge1, TYPE x) {
		TYPE t = clamp<TYPE>((x - edge0) / (edge1 - edge0), 0.0, 1.0);
		return t * t * (3.0 - 2.0 * t);
	}

	/*
		mix performs a linear interpolation between x and y using a to weight between them.
	*/
	template <typename TYPE>
	TYPE mix(TYPE x, TYPE y, TYPE a) {
		return x * (1 - a) + y * a;
	}
}

#endif