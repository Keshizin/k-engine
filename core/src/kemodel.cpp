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

#include "kemodel.h"
#include <iostream>

// ----------------------------------------------------------------------------
//  k-engine model class constructors & destructor
// ----------------------------------------------------------------------------
//kengine::model::model()
//	: coords{}
//{
//	std::cout << "> kengine::model default constructor - [" << this << "]" << std::endl;
//}

kengine::model::model(struct vattrib<float>& v, struct vattrib<float>& c, struct vattrib<int>& i)
	: coords { std::move(v) }, colors { std::move(c) }, indices { std::move(i) }
{
	// std::cout << "> kengine::model constructor with arguments - [" << this << "]" << std::endl;
}

kengine::model::model(const model &m)
	: coords{ m.coords }, colors{ m.colors }, indices{ m.indices }
{
	// std::cout << "> kengine::model copy constructor - [" << this << "]" << std::endl;
}

kengine::model::model(model&& m) noexcept
	: coords{ std::move(m.coords) }, colors{ std::move(m.colors) }, indices{ std::move(m.indices) }
{
	// std::cout << "> kengine::model move constructor - [" << this << "]" << std::endl;
}

kengine::model::~model()
{
	// std::cout << "> kengine::model destructor - [" << this << "]" << std::endl;
}

// ----------------------------------------------------------------------------
//  k-engine model class operators overload
// ----------------------------------------------------------------------------
kengine::model& kengine::model::operator=(const model& m)
{
	// std::cout << "> kengine::model = operator overload - [" << this << "]" << std::endl;

	coords = m.coords;
	colors = m.colors;
	indices = m.indices;
	return *this;
}

// ----------------------------------------------------------------------------
//  k-engine model class public methods
// ----------------------------------------------------------------------------
//void kengine::model::load(const struct vattrib<float>& v)
//{
//	coords = v;
//}

void kengine::model::dump() const
{
	std::cout << "\n> kengine::model object [0x" << this << "]" << std::endl;

	std::cout
		<< "   - coords.attributeArray memory address [0x" << coords.attributeArray << "]\n"
		<< "   - coords.arraySize: " << coords.arraySize << "\n" << std::endl;

	for (size_t i = 0; i < coords.arraySize; i++)
		std::cout << "     coords.attributeArray[" << i << "]: " << coords.attributeArray[i] << std::endl;

	std::cout << std::endl;

	std::cout
		<< "   - colors.attributeArray memory address [0x" << colors.attributeArray << "]\n"
		<< "   - colors.arraySize: " << colors.arraySize << "\n" << std::endl;

	for (size_t i = 0; i < colors.arraySize; i++)
		std::cout << "     colors.attributeArray[" << i << "]: " << colors.attributeArray[i] << std::endl;

	std::cout << std::endl;

	std::cout
		<< "   - indices.attributeArray memory address [0x" << indices.attributeArray << "]\n"
		<< "   - indices.arraySize: " << indices.arraySize << "\n" << std::endl;

	for (size_t i = 0; i < indices.arraySize; i++)
		std::cout << "     indices.attributeArray[" << i << "]: " << indices.attributeArray[i] << std::endl;

	std::cout << std::endl;
}