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
//  k-engine model class constructor
// ----------------------------------------------------------------------------
kengine::model::model()
	: vertexArray{nullptr}, size{0}
{
}

// ----------------------------------------------------------------------------
//  k-engine model class copy constructor
// ----------------------------------------------------------------------------
kengine::model::model(const model &m)
{
	size = m.size;
	vertexArray = new float[size];

	for(size_t i = 0; i < size; i++)
		vertexArray[i] = m.vertexArray[i];
}

// ----------------------------------------------------------------------------
//  k-engine model class destructor
// ----------------------------------------------------------------------------
kengine::model::~model()
{
	delete vertexArray;
}

// ----------------------------------------------------------------------------
//  k-engine model class copy data
// ----------------------------------------------------------------------------
void kengine::model::copyData(const float *v, const size_t n)
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

// ----------------------------------------------------------------------------
//  k-engine model class print dump
// ----------------------------------------------------------------------------
void kengine::model::printDump() const
{
	if(vertexArray == nullptr)
		return;

	for(size_t i = 0; i < size; i++)
		std::cout << "element[" << i << "]: " << vertexArray[i] << std::endl;
}

// ----------------------------------------------------------------------------
//  k-engine model class get size in bytes
// ----------------------------------------------------------------------------
size_t kengine::model::getSizeInBytes() const
{
	return size * sizeof(float);
}

size_t kengine::model::getSize() const
{
	return size / 3;
}

// ----------------------------------------------------------------------------
//  k-engine model class vertex array
// ----------------------------------------------------------------------------
const float * const kengine::model::getVertexArray() const
{
	return vertexArray;
}