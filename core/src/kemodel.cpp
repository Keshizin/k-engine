/*
	K-Engine Geometric Model
	This file is part of the K-Engine.

	Copyright (C) 2021 Fabio Takeshi Ishikawa

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

#include <kemodel.h>
#include <iostream>

// ----------------------------------------------------------------------------
//  k-engine model class constructors & destructor
// ----------------------------------------------------------------------------
kengine::model::model()
	: vertexArray{nullptr}, size{0}
{
}

kengine::model::model(const model &m)
	: vertexArray{ new float[m.size] }, size{ m.size }
{
	for(size_t i = 0; i < size; i++)
		vertexArray[i] = m.vertexArray[i];
}

kengine::model::model(model&& m)
	: vertexArray { m.vertexArray }, size { m.size }
{
	m.vertexArray = nullptr;
	m.size = 0;
}

kengine::model::~model()
{
	delete vertexArray;
}

// ----------------------------------------------------------------------------
//  k-engine model class operators overload
// ----------------------------------------------------------------------------
kengine::model& kengine::model::operator=(const model& m)
{
	delete vertexArray;

	size = m.size;
	vertexArray = new float[size];

	for(size_t i = 0; i < size; i++)
		vertexArray[i] = m.vertexArray[i];

	return *this;
}

// ----------------------------------------------------------------------------
//  k-engine model class public methods
// ----------------------------------------------------------------------------
void kengine::model::load(const float *v, const size_t n)
{
	if(vertexArray != nullptr)
	{
		delete vertexArray;
	}

	size = n;
	vertexArray = new float[size];

	for(size_t i = 0; i < size; i++)
	{
		vertexArray[i] = v[i];
	}
}

void kengine::model::dump() const
{
	if (vertexArray == nullptr)
		return;

	std::cout
		<< "\n> kengine::model object [0x" << this << "]\n\n"
		<< "   - vertexArray memory address [0x" << vertexArray << "]\n"
		<< "   - array size: " << size << "\n"
		<< std::endl;

	for (size_t i = 0; i < size; i++)
		std::cout << "     vertexArray[" << i << "]: " << vertexArray[i] << std::endl;
	
	std::cout << std::endl;
}