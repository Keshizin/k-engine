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

#define _USE_MATH_DEFINES
#include <cmath>

#include <initializer_list>
#include <cstddef>
#include <string>


// (!) constant used to convert angle to radian (PI / 180º)
#define K_PI_TO_RADIAN 0.0174532925f

namespace kengine
{
	template <typename T> class matrix;

	/*
		This class represent a mathematic vector 4-tuple (x, y, z, w)
	*/
	template <typename T>
	class vec4
	{
	public:
		vec4() {}
		explicit vec4(const T& value) : x{ value }, y{ value }, z{ value }, w{ value } {}
		vec4(T a, T b, T c) : x{ a }, y{ b }, z{ c }, w{ 1 } {}
		vec4(T a, T b, T c, T d) : x{ a }, y{ b }, z{ c }, w{ d } {}

		vec4 operator-(const vec4& v); // only (x, y, and z)
		vec4 operator+(const vec4& v); // only (x, y, and z)
		vec4 operator*(const vec4& v); // only (x, y, and z)
		vec4 operator*(const T& v); // only (x, y, and z)

		/*
			This function consider that the matrix is in column-major order.
		*/
		vec4 operator*(const matrix<T>& m); // v * matrix

		/*
			Returns the norma or magnitude
		*/
		T length() const;

		/*
			The vector values is overrided
		*/
		vec4& normalize();

		T x = 0;
		T y = 0;
		T z = 0;
		T w = 1;
	};

	template <class T>
	vec4<T> vec4<T>::operator-(const vec4<T>& v)
	{
		return vec4<T>(x - v.x, y - v.y, z - v.z);
	}

	template <class T>
	vec4<T> vec4<T>::operator+(const vec4<T>& v)
	{
		return vec4<T>(x + v.x, y + v.y, z + v.z);
	}

	template <class T>
	vec4<T> vec4<T>::operator*(const vec4<T>& v)
	{
		return vec4<T>(x * v.x, y * v.y, z * v.z);
	}

	template <class T>
	vec4<T> vec4<T>::operator*(const T& v)
	{
		return vec4<T>(x * v, y * v, z * v);
	}

	template <class T>
	vec4<T> vec4<T>::operator*(const matrix<T>& m)
	{
		/*
			This function consider that the matrix is in COLUMN-MAJOR

				(v' = v * M)

			(x)   [a][e][i][m]   (a.x + e.y + i.z + m.w)
			(y) * [b][f][j][n] = (b.x + f.y + j.z + n.w)
			(z)   [c][g][k][o]   (c.x + g.y + k.z + o.w)
			(w)   [d][h][l][p]   (d.x + h.y + l.z + p.w)
		*/

		vec4<TYPE> r;
		r.x = (m[ 0] * x) + (m[ 1] * y) + (m[ 2] * z) + (m[ 3] * w);
		r.y = (m[ 4] * x) + (m[ 5] * y) + (m[ 6] * z) + (m[ 7] * w);
		r.z = (m[ 8] * x) + (m[ 9] * y) + (m[10] * z) + (m[11] * w);
		r.w = (m[12] * x) + (m[13] * y) + (m[14] * z) + (m[15] * w);
		return r;
	}

	template <class T>
	T vec4<T>::length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	template <class T>
	vec4<T>& vec4<T>::normalize()
	{
		T magnitude = length();

		if (magnitude > 0) {
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
		}
		
		// (!) must log if magnitude is 0 (!)

		return *this;
	}

	template <class T>
	vec4<T> operator*(const matrix<T>& m, const vec4<T>& v)
	{
		/*
			This function consider that the matrix is in COLUMN-MAJOR

				(v' = M * v)

			[a][e][i][m]   (x)   (a.x + b.y + c.z + d.w)
			[b][f][j][n] * (y) = (e.x + f.y + g.z + h.w)
			[c][g][k][o]   (z)   (i.x + j.y + k.z + l.w)
			[d][h][l][p]   (w)   (m.x + n.y + o.z + p.w)
		*/

		vec4<TYPE> r;
		r.x = (m[0] * v.x) + (m[4] * v.y) + (m[ 8] * v.z) + (m[12] * v.w);
		r.y = (m[1] * v.x) + (m[5] * v.y) + (m[ 9] * v.z) + (m[13] * v.w);
		r.z = (m[2] * v.x) + (m[6] * v.y) + (m[10] * v.z) + (m[14] * v.w);
		r.w = (m[3] * v.x) + (m[7] * v.y) + (m[11] * v.z) + (m[15] * v.w);
		return r;
	}

	template <typename T>
	vec4<T> normalize(const vec4<T>& v1)
	{
		vec4<T> r = v1;
		T magnitude = v1.length();

		if (magnitude > 0) {
			r.x /= magnitude;
			r.y /= magnitude;
			r.z /= magnitude;
		}

		// (!) must log if magnitude is 0 (!)

		return r;
	}

	template <typename T>
	vec4<T>	crossProduct(const vec4<T>& v1, const vec4<T>& v2)
	{
		return vec4<T>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x,
			1);
	}

	template <typename T>
	T dotProduct(const vec4<T>& v1, const vec4<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z
	}

	// ***************************************************************************

	/*
		This class stores A 4x4 matrix in column-major order.
	*/
	template <typename T>
	class matrix {
		template <class T> friend matrix<T> transpose(const matrix<T>& m);
		template <class T> friend matrix<T> translate(T x, T y, T z);
		template <class T> friend matrix<T> scale(T x, T y, T z);
		template <class T> friend matrix<T> rotate(T angle, T x, T y, T z);
		template <class T> friend matrix<T> rotate(T x, T y, T z);
		template <class T> friend matrix<T> frustum(const T left, const T right, const T bottom, const T top, const T near, const T far);
		template <class T> friend matrix<T> lookAt(kengine::vec4<T> eye, kengine::vec4<T> center, kengine::vec4<T> up);
		template <class T> friend matrix<T> perspective(T fovy, T aspectRatio, T near, T far);
		template <class T> friend matrix<T> ortho(const T left, const T right, const T bottom, const T top, const T near, const T far);

	public:
		static size_t size() { return 16 * sizeof(T); }

		matrix(); // zero values
		~matrix();

		explicit matrix(T identity);
		explicit matrix(std::initializer_list<T> data);

		matrix(const matrix& copy); // copy constructor
		matrix(matrix&& move) noexcept; // move constructor
		matrix& operator=(const matrix& right); // copy assignment
		matrix& operator=(matrix&&) noexcept; // move assigment
		
		T& operator[](size_t i) { return m[i]; } // (!) validar indice invalido
		T operator[](size_t i) const { return m[i]; } // (!) validar indice invalido
		matrix operator*(const matrix<T>& right);
		bool operator==(const matrix<T>& right);
		
		/*
			this function will override the values
		*/
		matrix& transpose();

		T* value() { return m; } // (!) dangerous
		
		std::string dump() const; // this method is used only for the K-Engine purpose

	private:
		T* m = nullptr;
	};

	template <class T>
	matrix<T>::matrix()
		: m{ new T[16] }
	{
		m[ 0] = 0; m[ 1] = 0; m[ 2] = 0; m[ 3] = 0;
		m[ 4] = 0; m[ 5] = 0; m[ 6] = 0; m[ 7] = 0;
		m[ 8] = 0; m[ 9] = 0; m[10] = 0; m[11] = 0;
		m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 0;
	}

	template <class T>
	matrix<T>::~matrix()
	{
		delete[] m;
	}

	template <class T>
	matrix<T>::matrix(T identity)
		: m{ new T[16] }
	{
		m[ 0] = identity; m[ 1] = 0;        m[ 2] = 0;        m[ 3] = 0;
		m[ 4] = 0;        m[ 5] = identity; m[ 6] = 0;        m[ 7] = 0;
		m[ 8] = 0;        m[ 9] = 0;        m[10] = identity; m[11] = 0;
		m[12] = 0;        m[13] = 0;        m[14] = 0;        m[15] = identity;
	}

	template <class T>
	matrix<T>::matrix(std::initializer_list<T> data)
		: m{ new T[16] }
	{
		auto first = data.begin();

		for (int i = 0; i < 16; i++) {
			m[i] = 0.0f;

			if (first != data.end()) {
				m[i] = *first;
				first++;
			}
		}
	}

	template <class T>
	matrix<T>::matrix(const matrix<T>& copy)
		: m{ new T[16] }
	{
		for (int i = 0; i < 16; i++) {
			m[i] = copy.m[i];
		}
	}

	template <class T>
	matrix<T>::matrix(matrix<T>&& move) noexcept
		: m{ move.m }
	{
		move.m = nullptr;
	}

	template <class T>
	matrix<T>& matrix<T>::operator=(const matrix<T>& right)
	{
		for (int i = 0; i < 16; i++) {
			m[i] = right.m[i];
		}

		return *this;
	}

	template <class T>
	matrix<T>& matrix<T>::operator=(matrix<T>&& right) noexcept
	{
		delete[] m;
		m = right.m;
		right.m = nullptr;
		return *this;
	}

	template <class T>
	matrix<T> matrix<T>::operator*(const matrix<T>& right)
	{
		// column-major multiplication
		matrix<T> r;
		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				for (size_t k = 0; k < 4; k++)
					r.m[j * 4 + i] += m[k * 4 + i] * right[j * 4 + k];
			}
		}

		return r;
	}

	template <class T>
	bool matrix<T>::operator==(const matrix<T>& right)
	{
		for (int i = 0; i < 16; i++)
			if (m[i] != right[i])
				return false;

		return true;
	}

	template <class T>
	matrix<T>& matrix<T>::transpose()
	{
		matrix<T> t = m;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[j * 4 + i] = t[i * 4 + j];
			}
		}

		return *this;
	}

	template <class T>
	std::string matrix<T>::dump() const
	{
		std::string d = std::string("\n> kengine::matrix object [0x") + std::to_string(reinterpret_cast<uintptr_t>(this)) + "]\n";

		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				d += "\t[" + std::to_string(m[row * 4 + column]) + "]";
			}

			d += "\n";
		}

		return d;
	}

	// ****************************************************************************

	template <class T>
	matrix<T> transpose(const matrix<T>& m)
	{
		matrix<T> t = m;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				t[j * 4 + i] = m[i * 4 + j];
			}
		}

		return t;
	}

	/*
		kengine::projection_info class

		This class define the properties for the following viewing models:
			- perspective viewing model
			- orthographic viewing model
	*/
	template <class T>
	class projection_info
	{
	public:
		T left = -2;
		T right = 2;
		T bottom = -2;
		T top = 2;
		T zNear = 2; // 2.0f;
		T zFar = 100;
		T fovy = 60; // field of view in Y axis for perspective use 39.6
	};

	/*
		global function for mathematics
	*/

	template <class T>
	matrix<T> translate(T x, T y, T z)
	{
		/*
			translate matrix in column-major order

				[ 1.0 ] [ 0.0 ] [ 0.0 ] [ 0.0 ]
				[ 0.0 ] [ 1.0 ] [ 0.0 ] [ 0.0 ]
				[ 0.0 ] [ 0.0 ] [ 1.0 ] [ 0.0 ]
				[  X  ] [  Y  ] [  Z  ] [ 1.0 ]
		*/

		kengine::matrix<T> t(1);
		t.m[12] = x;
		t.m[13] = y;
		t.m[14] = z;
		return t;
	}

	template <class T>
	matrix<T> scale(T x, T y, T z)
	{
		/*
			scale matrix in row-major/column-major order

				[  X  ] [ 0.0 ] [ 0.0 ] [ 0.0 ]
				[ 0.0 ] [  Y  ] [ 0.0 ] [ 0.0 ]
				[ 0.0 ] [ 0.0 ] [  Z  ] [ 0.0 ]
				[ 0.0 ] [ 0.0 ] [ 0.0 ] [ 1.0 ]
		*/

		matrix<T> s(1);
		s[0] = x;
		s[5] = y;
		s[10] = z;
		return s;
	}

	template <class T>
	matrix<T> rotate(T angle, T x, T y, T z)
	{
		matrix<T> r(1.0f);

		T x2 = x * x;
		T y2 = y * y;
		T z2 = z * z;
		T rad = angle * K_PI_TO_RADIAN;
		T cos_angle = std::cos(rad);
		T sin_angle = std::sin(rad);
		T omc = 1.0f - cos_angle;

		r[0] = (x2 * omc + cos_angle);
		r[1] = (y * x * omc + z * sin_angle);
		r[2] = (x * z * omc - y * sin_angle);
		r[3] = 0.0f;

		r[4] = (x * y * omc - z * sin_angle);
		r[5] = (y2 * omc + cos_angle);
		r[6] = (y * z * omc + x * sin_angle);
		r[7] = 0.0f;

		r[8] = (x * z * omc + y * sin_angle);
		r[9] = (y * z * omc - x * sin_angle);
		r[10] = (z2 * omc + cos_angle);
		r[11] = 0.0f;

		r[12] = 0.0f;
		r[13] = 0.0f;
		r[14] = 0.0f;
		r[15] = 1.0f;

		return r;
	}

	// (!) this function set the matrix in coloumn-major order
	template <class T>
	matrix<T> rotate(T x, T y, T z)
	{
		/*
			rotation matrix in column-major order in X axis

				[ 1.0 ] [   0.0   ] [  0.0   ] [ 0.0 ]
				[ 0.0 ] [  cos(x) ] [ sin(x) ] [ 0.0 ]
				[ 0.0 ] [ -sin(x) ] [ cos(x) ] [ 0.0 ]
				[ 0.0 ] [   0.0   ] [  0.0   ] [ 1.0 ]

			rotation matrix in column-major order in Y axis

				[ cos(y) ] [ 0.0 ] [ -sin(y) ] [ 0.0 ]
				[  0.0   ] [ 1.0 ] [   0.0   ] [ 0.0 ]
				[ sin(y) ] [ 0.0 ] [  cos(y) ] [ 0.0 ]
				[  0.0   ] [ 0.0 ] [   0.0   ] [ 1.0 ]

			rotation matrix in column-major order in Z axis

				[  cos(z) ] [ sin(z) ] [ 0.0 ] [ 0.0 ]
				[ -sin(z) ] [ cos(z) ] [ 0.0 ] [ 0.0 ]
				[   0.0   ] [  0.0   ] [ 1.0 ] [ 0.0 ]
				[   0.0   ] [  0.0   ] [ 0.0 ] [ 1.0 ]

			In all cases, the rotation is in the direction of the first axis
			toward the second axis — that is, from the row with the cos –sin
			pattern to the row with the sin cos pattern, for the positive
			axes corresponding to these rows.
		*/

		matrix<T> r(1);

		if (x > 0.0f)
		{
			matrix<T> rx(1);
			x *= K_PI_TO_RADIAN; // convert to radians
			rx[5] = std::cos(x);
			rx[6] = std::sin(x);
			rx[9] = -std::sin(x);
			rx[10] = std::cos(x);
			r = rx * r;
		}

		if (y > 0.0f)
		{
			matrix<T> ry(1);
			y *= K_PI_TO_RADIAN; // convert to radians
			ry[0] = std::cos(y);
			ry[2] = -std::sin(y);
			ry[8] = std::sin(y);
			ry[10] = std::cos(y);
			r = ry * r;
		}

		if (z > 0.0f)
		{
			matrix<T> rz(1);
			z *= K_PI_TO_RADIAN; // convert to radians
			r[0] = std::cos(z);
			r[1] = std::sin(z);
			r[4] = -std::sin(z);
			r[5] = std::cos(z);
			r = rz * r;
		}

		return  r;
	}

	template <typename T>
	matrix<T> frustum(const T left, const T right, const T bottom, const T top, const T near, const T far)
	{
		/*
			(!) frustum matrix in column-major order
				- using right-hand matrix
				- forward to z negative axis
				- z coordinate normalized to [0, 1]

			[ 2n/(r-l) ][    0     ][     0     ][  0 ]
			[    0     ][ 2n/(t-b) ][     0     ][  0 ]
			[ r+l/r-l  ][ t+b/t-b  ][ -(f/f-n)  ][ -1 ]
			[    0     ][    0     ][ -(fn/f-n) ][  0 ]
		*/
		kengine::matrix<T> p;
		p.m[0] = 2 * near / (right - left);
		p.m[5] = 2 * near / (top - bottom);
		p.m[8] = (right + left) / (right - left);
		p.m[9] = (top + bottom) / (top - bottom);
		p.m[10] = -(far / (far - near));
		p.m[11] = -1.0f;
		p.m[14] = -((far * near) / (far - near));
		p.m[15] = 0.0f;
		return p;
	}

	template <class T>
	matrix<T> lookAt(vec4<T> eye, vec4<T> center, vec4<T> up)
	{
		/*
			(!) look-at matrix in column-major matrix
				- using right-hand matrix
		*/
		vec4<T> forward = normalize(center - eye);
		vec4<T> right = normalize(crossProduct(forward, up));
		vec4<T> up_ = crossProduct(right, forward);

		matrix<T> l(1.0f);
		l[0] = right.x; 
		l[4] = right.y;
		l[8] = right.z;

		l[1] = up_.x;
		l[5] = up_.y;
		l[9] = up_.z;

		l[2] = -forward.x;
		l[6] = -forward.y;
		l[10] = -forward.z;

		return translate(-eye.x, -eye.y, -eye.z) * l;
	}

	template <typename T>
	matrix<T> perspective(T fovy, T aspectRatio, T near, T far)
	{
		float xmax, ymax;
		ymax = near * tanf(fovy * static_cast<T>(3.14159265358979323846) / 360);
		xmax = ymax * aspectRatio;
		return frustum(-xmax, xmax, -ymax, ymax, near, far);
	}

	template <typename T>
	matrix<T> ortho(const T left, const T right, const T bottom, const T top, const T near, const T far)
	{
		/*
			(!) ortho matrix is in colum-major order

			[   2/(r-l)  ][     0      ][     0      ][ 0 ]
			[      0     ][  2/(t-b)   ][     0      ][ 0 ]
			[      0     ][     0      ][  -2/(f-n)  ][ 0 ]
			[ -(r+l/r-l) ][ -(t+b/t-b) ][ -(f+n/f-n) ][ 1 ]
		*/
		kengine::matrix<T> p(1);
		p.m[0] = 2 / (right - left);
		p.m[5] = 2 / (top - bottom);
		p.m[10] = -2 / (far - near);
		p.m[12] = -((right + left) / (right - left));
		p.m[13] = -((top + bottom) / (top - bottom));
		p.m[14] = -((far + near) / (far - near));
		return p;
	}

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