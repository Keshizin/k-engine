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


/*
*
*  kengine::mesh class - member class definition
*
*/

kengine::mesh::mesh()
	: position{}, colors{}, texCoords{}, normals{}, indices{}
{
}


kengine::mesh::mesh(vattrib<float>& v, vattrib<float>& c, vattrib<float>& t, vattrib<float>& n, vattrib<unsigned int>& i)
	: position{ std::move(v) }, colors{ std::move(c) }, texCoords{ std::move(t) }, normals{ std::move(n) }, indices{ std::move(i) }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "> (!) kengine::mesh constructor with arguments - [" << this << "]")
}


kengine::mesh::mesh(const mesh& m)
	: position{ m.position }, colors{ m.colors }, texCoords{ m.texCoords }, normals{ m.normals }, indices{ m.indices }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "> (!) kengine::mesh copy constructor - [" << this << "]")
}


kengine::mesh::mesh(mesh&& m) noexcept
	: position{ std::move(m.position) }, colors{ std::move(m.colors) }, texCoords{ std::move(m.texCoords) }, normals{ m.normals }, indices{ std::move(m.indices) }
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
	normals = m.normals;
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
		<< "   - normals.attributeArray memory address [0x" << normals.attributeArray << "]\n"
		<< "   - normals.arraySize: " << normals.arraySize << "\n"
		<< "   - normals.count: " << normals.count << "\n" << std::endl;

	for (size_t i = 0; i < normals.arraySize; i++)
		std::cout << "     normals.attributeArray[" << i << "]: " << normals.attributeArray[i] << std::endl;

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
	normals.clear();
	indices.clear();
}


/*
*
*  functions to create basic geometric models
*
*/

kengine::mesh kengine::triangle(float size)
{
	float half = size / 2.0f;

	float vertex_positions[] =
	{
		-half, -half,  0.0f,
		 half, -half,  0.0f,
		-half,  half,  0.0f
	};

	float vertex_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
	};

	float texture_coordinates[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f
	};

	float vertex_normal[] =
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	unsigned int vertex_indices[] =
	{
		1, 2, 0
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		9,
		3
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

	kengine::vattrib<float> n = {
		vertex_normal,
		9,
		3
	};

	kengine::vattrib<unsigned int> i = {
		vertex_indices,
		3,
		1
	};

	kengine::mesh tris(v, c, t, n, i);
	return tris;
}


kengine::mesh kengine::quad(float size)
{
	float half = size / 2.0f;

	float vertex_positions[] =
	{
		-half, -half, 0.0f,
		 half, -half, 0.0f,
		-half,  half, 0.0f,
		 half,  half, 0.0f
	};

	float vertex_colors[] =
	{
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
	};

	// (!) attention here! Opengl store the texture image "upside down"
	float texture_coordinates[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f
	};

	float vertex_normal[] =
	{
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	unsigned int vertex_indices[] =
	{
		1, 2, 0, 1, 3, 2
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		12,
		3
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

	kengine::vattrib<float> n = {
		vertex_normal,
		12,
		3
	};

	kengine::vattrib<unsigned int> i = {
		vertex_indices,
		6,
		1
	};

	kengine::mesh q(v, c, t, n, i);
	return q;
}