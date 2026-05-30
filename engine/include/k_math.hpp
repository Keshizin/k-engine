/*
	K-Engine Mathematic Library
	This file is part of the K-Engine.

	Copyright (C) 2020-2026 Fabio Takeshi Ishikawa

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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <initializer_list>
#include <cstddef>
#include <vector>
#include <array>
#include <string>

// (!) constant used to convert angle to radian (PI / 180º)
#define K_ANGLE_TO_RADIAN 0.0174532925f

namespace kengine
{
	/*
		kengine::projection_info class
	*/
	template <class T>
	class projection_info
	{
	public:
		T left = 0;
		T right = 0;
		T bottom = 0;
		T top = 0;
		T zNear = 0;
		T zFar = 0;
		T fovy = 0;
		T width = 0.0f;
		T height = 0.0f;
	};

	template <typename T> class vec3; // forward declaration

	/*
		This is a static matrix 4x4 (column-major order)
	*/
	template <typename TYPE>
	class matrix
	{
	public:
		template <class T> friend matrix<T> transpose(const matrix<T>&m);
		template <class T> friend matrix<T> translate(T x, T y, T z);
		template <class T> friend matrix<T> scale(T x, T y, T z);
		template <class T> friend matrix<T> rotate(T angle, T x, T y, T z);
		template <class T> friend matrix<T> rotate(T x, T y, T z);

		template <class T> friend matrix<T> quaternion(T x, T y, T z, T w);

		template <class T> friend matrix<T> frustum(const T left, const T right, const T bottom, const T top, const T near, const T far);
		template <class T> friend matrix<T> frustum(const projection_info<T>& projInfo);
		template <class T> friend matrix<T> perspective(T fovy, T aspectRatio, T near, T far);
		template <class T> friend matrix<T> ortho(const T left, const T right, const T bottom, const T top, const T near, const T far);
		template <class T> friend matrix<T> lookAt(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up);

		matrix() {}
		explicit matrix(TYPE identity);
		explicit matrix(std::initializer_list<TYPE> data);

		TYPE& operator[](size_t i) { return m[i]; } // (!) chech invalid index
		TYPE operator[](size_t i) const { return m[i]; } // (!) chech invalid index
		matrix operator*(const matrix& right);

		matrix& transpose();
		std::string dump() const;

		TYPE* data() { return m.data(); }

		void copy(TYPE* copy) {
			for (int i = 0; i < 16; i++) {
				copy[i] = m[i];
			}
		}

	private:
		std::array<TYPE, 16> m{};
	};

	template <class T>
	matrix<T>::matrix(T identity)
	{
		m[ 0] = identity; m[ 1] = 0;        m[ 2] = 0;        m[3] = 0;
		m[ 4] = 0;        m[ 5] = identity; m[ 6] = 0;        m[7] = 0;
		m[ 8] = 0;        m[ 9] = 0;        m[10] = identity; m[11] = 0;
		m[12] = 0;        m[13] = 0;        m[14] = 0;        m[15] = identity;
	}

	template <class T>
	matrix<T>::matrix(std::initializer_list<T> data)
	{
		auto first = data.begin();

		for (int i = 0; i < 16; i++) {
			if (first != data.end()) {
				m[i] = *first;
				first++;
			}
		}
	}

	template <class T>
	matrix<T> matrix<T>::operator*(const matrix<T>& right)
	{
		/*
			column-major multiplication
		*/

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
	matrix<T>& matrix<T>::transpose()
	{
		matrix<T> t = *this;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[j * 4 + i] = t[i * 4 + j];
			}
		}

		return *this;
	}

	template <class T>
	matrix<T> transpose(const matrix<T>& m)
	{
		matrix<T> t = *this;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				t[j * 4 + i] = m[i * 4 + j];
			}
		}

		return t;
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
		T rad = angle * K_ANGLE_TO_RADIAN;
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
			x *= K_ANGLE_TO_RADIAN; // convert to radians
			rx[5] = std::cos(x);
			rx[6] = std::sin(x);
			rx[9] = -std::sin(x);
			rx[10] = std::cos(x);
			r = rx * r;
		}

		if (y > 0.0f)
		{
			matrix<T> ry(1);
			y *= K_ANGLE_TO_RADIAN; // convert to radians
			ry[0] = std::cos(y);
			ry[2] = -std::sin(y);
			ry[8] = std::sin(y);
			ry[10] = std::cos(y);
			r = ry * r;
		}

		if (z > 0.0f)
		{
			matrix<T> rz(1);
			z *= K_ANGLE_TO_RADIAN; // convert to radians
			r[0] = std::cos(z);
			r[1] = std::sin(z);
			r[4] = -std::sin(z);
			r[5] = std::cos(z);
			r = rz * r;
		}

		return  r;
	}

	template <typename T>
	matrix<T> quaternion(T x, T y, T z, T w)
	{
		T xx = x * x;
		T yy = y * y;
		T zz = z * z;
		T xy = x * y;
		T xz = x * z;
		T yz = y * z;
		T wx = w * x;
		T wy = w * y;
		T wz = w * z;

		matrix<T> q;

		q[0] = 1 - 2 * y * y - 2 * z * z;
		q[1] = 2 * x * y + 2 * z * w;
		q[2] = 2 * x * z - 2 * y * w;
		q[3] = 0;

		q[4] = 2 * x * y - 2 * z * w;
		q[5] = 1 - 2 * x * x - 2 * z * z;
		q[6] = 2 * y * z + 2 * x * w;
		q[7] = 0;

		q[8] = 2 * x * z + 2 * y * w;
		q[9] = 2 * y * z - 2 * x * w;
		q[10] = 1 - 2 * x * x - 2 * y * y;
		q[11] = 0;

		q[12] = 0;
		q[13] = 0;
		q[14] = 0;
		q[15] = 1;

		/*
			return matrix<T>(
				// Coluna 0
				T(1) - T(2) * (yy + zz),
				T(2) * (xy + wz),
				T(2) * (xz - wy),
				T(0),
			// Coluna 1
			T(2) * (xy - wz),
			T(1) - T(2) * (xx + zz),
			T(2) * (yz + wx),
			T(0),
			// Coluna 2
			T(2) * (xz + wy),
			T(2) * (yz - wx),
			T(1) - T(2) * (xx + yy),
			T(0),
			// Coluna 3
			T(0), T(0), T(0), T(1)
    );
		*/

		return q;
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
		p.m[10] = -far / (far - near);
		p.m[11] = -1;
		p.m[14] = -far * near / (far - near);
		return p;
	}

	template <class T>
	matrix<T> frustum(const projection_info<T>& projInfo)
	{
		return frustum<T>(projInfo.left, projInfo.right, projInfo.bottom, projInfo.top, projInfo.zNear, projInfo.zFar);
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
		kengine::matrix<T> p;
		p.m[0] = 2 / (right - left);
		p.m[5] = 2 / (top - bottom);
		p.m[10] = -2 / (far - near);
		p.m[12] = -(right + left) / (right - left);
		p.m[13] = -(top + bottom) / (top - bottom);
		p.m[14] = -(far + near) / (far - near);
		p.m[15] = 1;
		return p;
	}

	template <class T>
	matrix<T> lookAt(const vec3<T>& eye, const vec3<T>& center, const vec3<T>& up)
	{
		/*
			(!) The look-at matrix is:
				- column-major order
				- right-hand matrix
		*/

		vec3<T> forward = normalize(center - eye);
		vec3<T> right = normalize(crossProduct(forward, up));
		vec3<T> up_ = crossProduct(right, forward);

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

		l[12] = -dotProduct(right, eye);
		l[13] = -dotProduct(up_, eye);
		l[14] = dotProduct(forward, eye);

		return l;
	}


	/*
		This class represent a mathematic vector 3-tuple (x, y, z) with implicit coordinate w
	*/
	template <typename T>
	class vec3
	{
	public:
		vec3() {}
		explicit vec3(const T& value) : x{ value }, y{ value }, z{ value }, w{ 1 } {}
		vec3(T a, T b, T c) : x{ a }, y{ b }, z{ c }, w{ 1 } {}
		vec3(T a, T b, T c, T d) : x{ a }, y{ b }, z{ c }, w{ d } {}

		vec3 operator+(const vec3& v) const;
		vec3 operator-(const vec3& v) const;
		constexpr vec3 operator-() const noexcept;
		vec3 operator*(const vec3& v);
		vec3 operator*(const T& scalar);

		/*
		*/
		void invert();

		/*
			Returns the norma or magnitude
		*/
		T length() const;

		/*
		*/
		T sqrtMagnitude() const;

		/*
		*/
		vec3& normalize();

		/*
		*/
		vec3& normalizeAll();

		/*
		*/
		void addAndScale(const vec3& v, T scale);

		/*
		*/
		T dotProduct(const vec3& v);

		/*
		*/
		vec3& crossProduct(const vec3& v);

		/*
		*/
		void copy(T c[4]);

		/*
		*/
		void set(size_t index, T value);

		T x = 0;
		T y = 0;
		T z = 0;
		T w = 1;
	};

	template <class T>
	vec3<T> vec3<T>::operator+(const vec3<T>& v) const
	{
		return vec3<T>(x + v.x, y + v.y, z + v.z);
	}

	template <class T>
	vec3<T> vec3<T>::operator-(const vec3<T>& v) const
	{
		return vec3<T>(x - v.x, y - v.y, z - v.z);
	}

	template <class T>
	constexpr vec3<T> vec3<T>::operator-() const noexcept
	{
		return vec3<T>(-x, -y, -z);
	}

	template <class T>
	vec3<T> vec3<T>::operator*(const vec3& v)
	{
		return vec3<T>(x * v.x, y * v.y, z * v.z);
	}

	template <class T>
	vec3<T> vec3<T>::operator*(const T& scalar)
	{
		return vec3<T>(x * scalar, y * scalar, z * scalar);
	}

	template<typename T>
	vec3<T> operator*(T scalar, const vec3<T>& v)
	{
		return vec3<T>(v.x * scalar, v.y * scalar, v.z * scalar);
	}

	template <class T>
	vec3<T> operator*(const matrix<T>& m, const vec3<T>& v)
	{
		/*
			This function consider that the matrix is in column-major

				(v' = M * v)

			[a][e][i][m]   (x)   (a.x + b.y + c.z + d.w)
			[b][f][j][n] * (y) = (e.x + f.y + g.z + h.w)
			[c][g][k][o]   (z)   (i.x + j.y + k.z + l.w)
			[d][h][l][p]   (w)   (m.x + n.y + o.z + p.w)
		*/

		vec3<T> r;
		r.x = (m[0] * v.x) + (m[4] * v.y) + (m[8] * v.z) + (m[12] * v.w);
		r.y = (m[1] * v.x) + (m[5] * v.y) + (m[9] * v.z) + (m[13] * v.w);
		r.z = (m[2] * v.x) + (m[6] * v.y) + (m[10] * v.z) + (m[14] * v.w);
		r.w = (m[3] * v.x) + (m[7] * v.y) + (m[11] * v.z) + (m[15] * v.w);
		return r;
	}

	template <class T>
	void vec3<T>::invert()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	template <class T>
	T vec3<T>::length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}

	template<class T>
	T vec3<T>::sqrtMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	template <class T>
	vec3<T>& vec3<T>::normalize()
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
	vec3<T>& vec3<T>::normalizeAll()
	{
		T magnitude = std::sqrt(x * x + y * y + z * z + w * w);

		if (magnitude > 0) {
			x /= magnitude;
			y /= magnitude;
			z /= magnitude;
			w /= magnitude;

		}
		
		return *this;
	}

	template <class T>
	void vec3<T>::addAndScale(const vec3<T>& v, T scale)
	{
		x += v.x * scale;
		y += v.y * scale;
		z += v.z * scale;
	}

	template <class T>
	T vec3<T>::dotProduct(const vec3& v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	template <typename T>
	T dotProduct(const vec3<T>& v1, const vec3<T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template <typename T>
	vec3<T> normalize(const vec3<T>& v1)
	{
		vec3<T> r = v1;
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
	vec3<T> normalizeAll(const vec3<T>& v1)
	{
		vec3<T> r = v1;
		T magnitude = std::sqrt(r.x * r.x + r.y * r.y + r.z * r.z + r.w * r.w);

		if (magnitude > 0) {
			r.x /= magnitude;
			r.y /= magnitude;
			r.z /= magnitude;
			r.w /= magnitude;
		}

		// (!) must log if magnitude is 0 (!)
		return r;
	}

	template <class T>
	vec3<T>& vec3<T>::crossProduct(const vec3& v)
	{
		x = y * v.z - z * v.y;
		y = z * v.x - x * v.z;
		z = x * v.y - y * v.x;
		return this;
	}

	template <typename T>
	vec3<T>	crossProduct(const vec3<T>& v1, const vec3<T>& v2)
	{
		return vec3<T>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	template <typename T>
	void vec3<T>::copy(T c[3])
	{
		c[0] = x;
		c[1] = y;
		c[2] = z;
	}

	template <typename T>
	void vec3<T>::set(size_t index, T value)
	{
		switch (index) {
		case 0:
			x = value;
			break;

		case 1:
			y = value;
			break;

		case 2:
			z = value;
			break;

		case 3:
			w = value;
			break;
		}
	}

	// ----------------------------------------------------------------------------------------------------------------

	template <typename TYPEA, typename TYPEB>
	TYPEA lerp(TYPEA a, TYPEA b, TYPEB t)
	{
		return a + t * (b - a);
	}

	template <typename TYPE>
	vec3<TYPE> slerp(vec3<TYPE> a, vec3<TYPE> b, TYPE t)
	{
		TYPE dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

		if (dot < TYPE(0)) {
			b.x = -b.x; b.y = -b.y; b.z = -b.z; b.w = -b.w;
			dot = -dot;
		}

		if (dot > TYPE(0.9995)) {
			vec3<TYPE> result = {
				a.x + t * (b.x - a.x),
				a.y + t * (b.y - a.y),
				a.z + t * (b.z - a.z),
				a.w + t * (b.w - a.w)
			};
			return normalizeAll(result);
		}

		TYPE theta0 = std::acos(dot);
		TYPE theta = theta0 * t;
		TYPE sinTheta0 = std::sin(theta0);

		TYPE sa = std::sin(theta0 - theta) / sinTheta0;
		TYPE sb = std::sin(theta) / sinTheta0;

		return {
			sa * a.x + sb * b.x,
			sa * a.y + sb * b.y,
			sa * a.z + sb * b.z,
			sa * a.w + sb * b.w
		};
	}

	//template <typename TYPE>
	//vec3<TYPE> slerp(vec3<TYPE> a, vec3<TYPE> b, TYPE t)
	//{
	//	// Dot product = cosseno do ângulo entre os quaternions
	//	TYPE dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

	//	// Se dot negativo, os quaternions estão em hemisférios opostos.
	//	// Nega b para garantir o caminho mais curto na esfera.
	//	if (dot < TYPE(0)) {
	//		b.x = -b.x; b.y = -b.y;
	//		b.z = -b.z; b.w = -b.w;
	//		dot = -dot;
	//	}

	//	// Se muito próximos, usa LERP para evitar divisão por zero no sin(theta)
	//	if (dot > TYPE(0.9995)) {
	//		vec3<TYPE> result = {
	//			a.x + t * (b.x - a.x),
	//			a.y + t * (b.y - a.y),
	//			a.z + t * (b.z - a.z),
	//			a.w + t * (b.w - a.w)
	//		};
	//		return normalize(result); // LERP não preserva magnitude — normaliza
	//	}

	//	TYPE theta0 = std::acos(dot);        // ângulo total entre a e b
	//	TYPE theta = theta0 * t;            // ângulo para o instante t
	//	TYPE sinTheta0 = std::sin(theta0);
	//	TYPE sinTheta = std::sin(theta);

	//	TYPE sa = std::cos(theta) - dot * sinTheta / sinTheta0; // peso de a
	//	TYPE sb = sinTheta / sinTheta0;                          // peso de b

	//	return {
	//		sa * a.x + sb * b.x,
	//		sa * a.y + sb * b.y,
	//		sa * a.z + sb * b.z,
	//		sa * a.w + sb * b.w
	//	};
	//}

	/*
		min returns the minimum of the two parameters. It returns y if y is less than x, otherwise it returns x.
	*/
	//template<typename TYPE>
	//TYPE min(TYPE x, TYPE y) {
	//	return (y < x ? y : x);
	//}

	// ***************************************************************************

	/*
		max returns the maximum of the two parameters. It returns y if y is greater than x, otherwise it returns x.
	*/
	//template<typename TYPE>
	//TYPE max(TYPE x, TYPE y) {
	//	return (y > x ? y : x);
	//}

	// ***************************************************************************

	/*
		clamp returns the value of x constrained to the range minVal to maxVal.
	*/
	//template<typename TYPE>
	//TYPE clamp(TYPE x, TYPE minValue, TYPE maxValue) {
	//	return min<TYPE>(max<TYPE>(x, minValue), maxValue);
	//}

	// ***************************************************************************

	/*
		smoothstep performs smooth Hermite interpolation between 0 and 1 when edge0 < x < edge1
	*/
	//template <typename TYPE>
	//TYPE smoothstep(TYPE edge0, TYPE edge1, TYPE x) {
	//	TYPE t = clamp<TYPE>((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	//	return t * t * (3.0 - 2.0 * t);
	//}

	// ***************************************************************************

	/*
		mix performs a linear interpolation between x and y using a to weight between them.
	*/
	//template <typename TYPE>
	//TYPE mix(TYPE x, TYPE y, TYPE a) {
	//	return x * (1 - a) + y * a;
	//}
}

#endif 