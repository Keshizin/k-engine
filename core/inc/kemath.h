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


#ifndef K_ENGINE_MATH_H
#define K_ENGINE_MATH_H

namespace kengine
{
	// ----------------------------------------------------------------------------
	//  (!) kengine::matrix class
	// 
	//  This class stores the matrix in column-major order
	// ----------------------------------------------------------------------------
	class matrix {
		friend kengine::matrix translate(float x, float y, float z);
		friend kengine::matrix rotate(float x, float y, float z);
		friend kengine::matrix scale(float x, float y, float z);
		friend kengine::matrix frustum(const float left, const float right, const float bottom, const float top, const float near, const float far);
		friend kengine::matrix ortho(const float left, const float right, const float bottom, const float top, const float near, const float far);

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

	private:
		float* m;
	};
}

#endif
