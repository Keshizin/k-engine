/*
	K-Engine Unit Test for Mathematics
	This file provide an test environment for K-Engine.

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

/*
	- test all vec4 instancing
	- test all matrix instancing
	- test vec4 multiplication (vec4 * vec4, vec4 * matrix, matrix * vec4)
	- test matrix multiplication (matrix * matrix)
	- testar os metodos como transposta, inverse, etc de matrix
	- testar translate,scale e rotate
	- testar todos os metodos de vec4 (normalize, dot, cross, etc)
*/

void vec4_instancing_test();
int linear_transformation_test();

/*
	main
*/
int main()
{
	int result = 0;
	return result;
}

void vec4_instancing_test()
{
	kengine::vec4<float> v = { 1.5f, 1.5f, 1.5f };
	kengine::vec4<float> v1;
	kengine::vec4<float> v2(3.5f, 3.5f, 3.5f);
	kengine::vec4<float> v4 = v2;
}

int linear_transformation_test()
{
	// test translate
	return 0;
}