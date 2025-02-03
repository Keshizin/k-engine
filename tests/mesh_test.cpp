/*
	K-Engine Unit Test for Mesh
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

#include <mesh.hpp>

/*
	vattribute memory leak tests
*/
kengine::vattrib<float> vattrib_test_1();
void vattrib_test_2(kengine::vattrib<float> a);
void vattribute_memory_leak_test();

/*
	main
*/
int main()
{
	int result = 0;
	vattribute_memory_leak_test();
	return result;
}





kengine::vattrib<float> vattrib_test_1() {
	float data[] = {
		2.0f, 2.0f, 2.0f, 2.0f,
		2.0f, 2.0f, 2.0f, 2.0f,
		2.0f, 2.0f, 2.0f, 2.0f,
	};

	kengine::vattrib<float> d = {
		data,
		16,
		4
	};

	return d;
}

void vattrib_test_2(kengine::vattrib<float> a) {}

void vattribute_memory_leak_test()
{
	/*
		Memory leak test #1
			- constructor with arguments call (ok)
			- destructor (ok)
			- Move constructor (ok)
			- destructor (ok)
	*/

	{
		kengine::vattrib<float> a = vattrib_test_1();
	}

	/*
		Memory leak test #2
			- constructor with arguments call (ok)
			- constructor with arguments call (ok)
			- copy constructor call (ok)
			- destructor (ok)
			- destructor (ok)
			- destructor (ok)
	*/

	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		float data2[] = {
			1.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 1.0f,
		};

		kengine::vattrib<float> b = {
			data2,
			6,
			2
		};

		kengine::vattrib<float> c = b;
	}

	/*
		Memory leak test #3
			- constructor with arguments call (ok)
			- copy constructor call (ok)
			- destructor (ok)
			- destructor (ok)
	*/

	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		vattrib_test_2(a);
	}

	/*
		Memory leak test #4
			- constructor with arguments call (ok)
			- constructor with arguments call (ok)
			- copy assignment call (ok)
			- copy assignment call (ok)
			- destructor (ok)
			- destructor (ok)
			- destructor (ok)
	*/

	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		float data1[] = {
			1.0f, 1.0f,
			1.0f, 1.0f,
		};

		kengine::vattrib<float> b = {
			data1,
			4,
			2
		};

		kengine::vattrib<float> c;

		c = b;
		c = a;
	}

	/*
		Memory leak test #5
			- constructor with arguments call (ok)
			- constructor with arguments call (ok)
			- move assignment call (ok)
			- move assignment call (ok)
			- destructor (ok)
			- destructor (ok)
			- destructor (ok)
	*/

	{
		float data[] = {
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
			2.0f, 2.0f, 2.0f, 2.0f,
		};

		kengine::vattrib<float> a = {
			data,
			16,
			4
		};

		float data1[] = {
			1.0f, 1.0f,
			1.0f, 1.0f,
		};

		kengine::vattrib<float> b = {
			data1,
			4,
			2
		};

		kengine::vattrib<float> c;

		c = std::move(b);
		c = std::move(a);
	}
}