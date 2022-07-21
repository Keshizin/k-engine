/*
	K-Engine Geometric Mesh
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

#include <kemesh.h>
#include <keaux.h>
#include <iostream>


// ----------------------------------------------------------------------------
//  k-engine mesh class constructors & destructor
// ----------------------------------------------------------------------------
kengine::mesh::mesh(vattrib<float>& v, vattrib<float>& c, vattrib<float>& t, vattrib<unsigned int>& i)
	: position{ std::move(v) }, colors{ std::move(c) }, texCoords{ std::move(t) }, indices{ std::move(i) }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "> (!) kengine::mesh constructor with arguments - [" << this << "]")
}


kengine::mesh::mesh(const mesh& m)
	: position{ m.position }, colors{ m.colors }, texCoords{ m.texCoords }, indices{ m.indices }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "> (!) kengine::mesh copy constructor - [" << this << "]")
}


kengine::mesh::mesh(mesh&& m) noexcept
	: position{ std::move(m.position) }, colors{ std::move(m.colors) }, texCoords{ std::move(m.texCoords) }, indices{ std::move(m.indices) }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "> (!) kengine::mesh move constructor - [" << this << "]")
}


kengine::mesh::~mesh()
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "> (!) kengine::mesh destructor - [" << this << "]")
}


kengine::mesh& kengine::mesh::operator=(const mesh& m)
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "> kengine::mesh = operator overload - [" << this << "]")

	position = m.position;
	colors = m.colors;
	texCoords = m.texCoords;
	indices = m.indices;
 	return *this;
}


void kengine::mesh::dump() const
{
	std::cout << "\n> kengine::mesh object [0x" << this << "]" << std::endl;
	
	std::cout
		<< "   - position.attributeArray memory address [0x" << position.attributeArray << "]\n"
		<< "   - position.arraySize: " << position.arraySize << "\n"
		<< "   - position.count: " << position.count << "\n" << std::endl;
	
	for (size_t i = 0; i < position.arraySize; i++)
		std::cout << "     position.attributeArray[" << i << "]: " << position.attributeArray[i] << std::endl;
	
	std::cout << std::endl;

	std::cout
		<< "   - colors.attributeArray memory address [0x" << colors.attributeArray << "]\n"
		<< "   - colors.arraySize: " << colors.arraySize << "\n"
		<< "   - colors.count: " << colors.count << "\n" << std::endl;

	for (size_t i = 0; i < colors.arraySize; i++)
		std::cout << "     colors.attributeArray[" << i << "]: " << colors.attributeArray[i] << std::endl;

	std::cout << std::endl;

	std::cout
		<< "   - texCoords.attributeArray memory address [0x" << texCoords.attributeArray << "]\n"
		<< "   - texCoords.arraySize: " << texCoords.arraySize << "\n"
		<< "   - texCoords.count: " << texCoords.count << "\n" << std::endl;

	for (size_t i = 0; i < texCoords.arraySize; i++)
		std::cout << "     texCoords.attributeArray[" << i << "]: " << texCoords.attributeArray[i] << std::endl;

	std::cout << std::endl;

	std::cout
		<< "   - indices.attributeArray memory address [0x" << indices.attributeArray << "]\n"
		<< "   - indices.arraySize: " << indices.arraySize
		<< "   - indices.count: " << indices.count << "\n" << std::endl;

	for (size_t i = 0; i < indices.arraySize; i++)
		std::cout << "     indices.attributeArray[" << i << "]: " << indices.attributeArray[i] << std::endl;

	std::cout << std::endl;
}


void kengine::mesh::clear()
{
	position.clear();
	colors.clear();
	texCoords.clear();
	indices.clear();
}


// ------------------------------------------------------------------------
//  functions to create basic geometric mesh
// ------------------------------------------------------------------------
kengine::mesh kengine::triangle(float size)
{
	float vertex_positions[] =
	{
		-size, -size,  0.0f, 1.0f,
		 size, -size,  0.0f, 1.0f,
		-size,  size,  0.0f, 1.0f
	};

	float vertex_colors[] =
	{
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	float texture_coordinates[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};

	unsigned int vertex_indices[] =
	{
		0, 1, 2
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		12,
		4
	};

	kengine::vattrib<float> c = {
		vertex_colors,
		12,
		4
	};

	kengine::vattrib<float> t = {
		texture_coordinates,
		6,
		2
	};

	kengine::vattrib<unsigned int> i = {
		vertex_indices,
		3,
		1
	};

	kengine::mesh q(v, c, t, i);
	return q;
}


kengine::mesh kengine::quad(float size)
{
	float half = size / 2.0f;

	float vertex_positions[] =
	{
		-half,  half,  0.0f, 1.0f,
		-half, -half,  0.0f, 1.0f,
		 half, -half,  0.0f, 1.0f,
		 half,  half,  0.0f, 1.0f
	};

	float vertex_colors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.8f, 0.2f, 0.5f, 1.0f,
	};

	float texture_coordinates[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	unsigned int vertex_indices[] =
	{
		0, 1, 2, 3, 0, 2
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		16,
		4
	};

	kengine::vattrib<float> c = {
		vertex_colors,
		16,
		4
	};

	kengine::vattrib<float> t = {
		texture_coordinates,
		8,
		2
	};

	kengine::vattrib<unsigned int> i = {
		vertex_indices,
		6,
		1
	};

	kengine::mesh q(v, c, t, i);
	return q;
}


kengine::mesh kengine::cube(float size)
{
	float half = size / 2.0f;

	float vertex_positions[] =
	{
		-half,  half,  half, 1.0f,
		-half, -half,  half, 1.0f,
		 half, -half,  half, 1.0f,
		 half,  half,  half, 1.0f,
		 half,  half, -half, 1.0f,
		 half, -half, -half, 1.0f,
		-half, -half, -half, 1.0f,
		-half,  half, -half, 1.0f
	};

	float vertex_colors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
	};

	float texture_coordinates[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f
	};
	
	unsigned int vertex_indices[] =
	{
		0, 1, 2, 3, 0, 2, // front
		2, 5, 4, 3, 2, 4, // right
		2, 1, 6, 5, 2, 6, // bottom
		6, 1, 0, 7, 6, 0, // left
		0, 3, 4, 7, 0, 4, // top
		4, 5, 6, 7, 4, 6  // back
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		32,
		4
	};

	kengine::vattrib<float> c = {
		vertex_colors,
		32,
		4
	};

	kengine::vattrib<float> t = {
		texture_coordinates,
		8,
		2
	};

	kengine::vattrib<unsigned int> i = {
		vertex_indices,
		36,
		1
	};

	kengine::mesh cube3D(v, c, t, i);
	return cube3D;
}