/*
	K-Engine Mesh
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

#include <mesh.hpp>
#include <string>

/*
	kengine::mesh class - member class definition
*/

kengine::mesh::mesh()
	//:
	//m_indices{}
{
	m_vattributesMap.reserve(MAX_VERTEX_ATTRIBUTES); // avoid dynamically resizing
}

/*
	move constructor
*/
kengine::mesh::mesh(mesh&& m) noexcept
	:
	m_size{ m.m_size },
	m_sizeInBytes{ m.m_sizeInBytes },
	m_interleavedStride{ m.m_interleavedStride },
	m_vattributesMap{ std::move(m.m_vattributesMap) },
	m_interleavedData{ std::move(m.m_interleavedData) }
	//m_indices{ std::move(m.m_indices) }
{
}

/*
	Set the vertex attribute data and its shader input location
	(!) attention: this method is copying the data - try to use std::move
*/
void kengine::mesh::setVertexAttribute(vattrib<float>& vertexAttribute, size_t location)
{

	if (m_vattributesMap.find(location) != m_vattributesMap.end()) {
		removeVertexAttribute(location);
	}

	m_vattributesMap[location] = vertexAttribute;
	m_size += vertexAttribute.getSize() * vertexAttribute.count;
	m_sizeInBytes += vertexAttribute.getSizeInBytes();
}

/*
	Remove the vertex attribute data
*/
void kengine::mesh::removeVertexAttribute(size_t location) {
	if (m_vattributesMap.find(location) != m_vattributesMap.end()) {
		m_vattributesMap.erase(location);
	}
}

void kengine::mesh::clear()
{
	m_size = 0;
	m_sizeInBytes = 0;
	m_interleavedStride = 0;
	m_vattributesMap.clear(); // (!) checar se todos os destrutores estão sendo chamados (!)
	m_interleavedData.clear(); // (!) checar se todos os destrutores estão sendo chamados (!)
}

float* kengine::mesh::getInterleavedData()
{
	if (!m_interleavedData.empty())
		return m_interleavedData.data();

	size_t vattributeCurrentIndex[MAX_VERTEX_ATTRIBUTES] = { 0 }; // control the indices of the vattribute arrays
	
	while (m_interleavedData.size() < getSize()) {
		for (auto it : m_vattributesMap) {
			for (size_t i = 0; i < it.second.count; i++) {
				m_interleavedData.push_back(it.second.attributeArray[vattributeCurrentIndex[it.first]++]);
			}
		}
	}

	/*
		Calculating the interleaved array stride
	*/
	for (auto it : m_vattributesMap) {
		m_interleavedStride += it.second.count * sizeof(float);
	}

	if (m_vattributesMap.size() <= 1)
		m_interleavedStride = 0;

	return m_interleavedData.data();
}

std::string kengine::mesh::dump() const
{
	std::string msg = std::string("\n> kengine::mesh object [0x") + std::to_string(reinterpret_cast<uintptr_t>(this)) + "]";
	
	for(auto it : m_vattributesMap) {

		auto location = std::to_string(it.first);

		msg += "   - vattributes[" + location + "].attributeArray memory address: [0x" + std::to_string(reinterpret_cast<uintptr_t>(it.second.attributeArray)) + "]";
		msg += "   - vattributes[" + location + "].arraySize: " + std::to_string(it.second.arraySize);
		msg += "   - vattributes[" + location + "].count: " + std::to_string(it.second.count) + "\n";
	
		for (size_t j = 0; j < it.second.arraySize; j++) {
			msg += "      vattributes[" + location + "].attributeArray[" + std::to_string(j) + "]: " + std::to_string(it.second.attributeArray[j]);
		}

		msg + "\n";
	}
	
	return msg;
}

kengine::mesh kengine::triangle(float size)
{
	float half = size / 2.0f;

	float vertex_positions[] = {
		-half, -half,  0.0f,
		 half, -half,  0.0f,
		-half,  half,  0.0f
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		9,
		3
	};

	float vertex_colors[] = {
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};

	kengine::vattrib<float> c = {
		vertex_colors,
		12,
		4
	};

	float texture_coordinates[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f
	};

	kengine::vattrib<float> t = {
		texture_coordinates,
		6,
		2
	};

	kengine::mesh tris;
	tris.setVertexAttribute(v, 0);
	tris.setVertexAttribute(c, 1);
	tris.setVertexAttribute(t, 2);
	tris.dump();
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

		-half,  half, 0.0f,
		 half, -half, 0.0f,
		 half,  half, 0.0f,
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		18,
		3
	};

	float vertex_colors[] =
	{
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
	};

	kengine::vattrib<float> c = {
		vertex_colors,
		24,
		4
	};

	
	float vertex_uv[] =
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	kengine::vattrib<float> t = {
		vertex_uv,
		12,
		2
	};

	kengine::mesh q;
	q.setVertexAttribute(v, 0);
	q.setVertexAttribute(c, 1);
	q.setVertexAttribute(t, 2);
	q.dump();
	return q;
}

kengine::mesh kengine::cube(float size)
{
	float half = size / 2.0f;

	float vertex_positions[] =
	{
		-half,  half, -half,
		-half, -half, -half,
		 half, -half, -half,
		 half,  half, -half,
		-half,  half,  half,
		-half, -half,  half,
		 half, -half,  half,
		 half,  half,  half,
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		24,
		3
	};

	float vertex_colors[] =
	{
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f,
		1.0f, 0.5f, 0.5f, 1.0f
	};

	kengine::vattrib<float> c = {
		vertex_colors,
		32,
		4
	};

	unsigned int vertex_indices[] = {
		0, 1, 2,
		0, 2, 3,
		4, 0, 3,
		4, 3, 7,
		5, 4, 7,
		5, 7, 6,
		1, 5, 6,
		1, 6, 2,
		4, 5, 1,
		4, 1, 0,
		3, 2, 6,
		3, 6, 7
	};

	kengine::vattrib<unsigned int> i = {
		vertex_indices,
		36,
		1
	};

	kengine::mesh q;
	q.setVertexAttribute(v, 0);
	q.setVertexAttribute(c, 1);
	//q.setIndices(i);

	return q;
}

kengine::mesh kengine::grid()
{
	float vertex_positions[] =
	{
		-5000.0f, 0.0f, 0.0f,
		 5000.0f, 0.0f, 0.0f,
		 0.0f, -5000.0f, 0.0f,
		 0.0f, 5000.0f, 0.0f,
		 0.0f, 0.0f, -5000.0f,
		 0.0f, 0.0f, 5000.0f
	};

	kengine::vattrib<float> v = {
		vertex_positions,
		18,
		3
	};

	float vertex_colors[] =
	{
		0.7f, 0.5f, 0.5f, 1.0f,
		0.7f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.7f, 0.5f, 1.0f,
		0.5f, 0.7f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.7f, 1.0f,
		0.5f, 0.5f, 0.7f, 1.0f
	};

	kengine::vattrib<float> col = {
		vertex_colors,
		24,
		4
	};

	kengine::mesh g;
	g.setVertexAttribute(v, 0);
	g.setVertexAttribute(col, 1);

	return g;
}