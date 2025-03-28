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

#ifndef K_ENGINE_MESH_HPP
#define K_ENGINE_MESH_HPP

#include <unordered_map>
#include <vector>
#include <cstddef>
#include <string>

namespace kengine {
	/*
		Class to store an vertex attribute
		Note: "vertex attribute" is a term used in OpenGL to represent an array of vertex data (i.e. array of positions, array of colors, etc).
	*/
	template <typename TYPE>
	class vattrib
	{
	public:
		// array of vertex data
		TYPE* attributeArray;

		// array size (number of elements)
		size_t arraySize;

		// count of elements per vertex attribute (i.e. must be 4 for RGBA)
		size_t count;

		// default constructor
		vattrib()
			: attributeArray{ nullptr }, arraySize{ 0 }, count{ 0 }
		{
		}

		// destrcutor
		~vattrib()
		{
			clear();
		}

		// constructor with arguments
		vattrib(const TYPE* array, size_t size, size_t c)
			: attributeArray{ nullptr }, arraySize{ 0 }, count{ 0 }
		{
			if (size > 0) {
				arraySize = size;
				count = c;
				attributeArray = new TYPE[size];

				for (size_t i = 0; i < size; i++) {
					attributeArray[i] = array[i];
				}
			}

		}

		// copy constructor
		vattrib(const vattrib& va)
			: attributeArray{ nullptr }, arraySize{ 0 }, count{ 0 }
		{
			if (va.arraySize) {
				arraySize = va.arraySize;
				count = va.count;
				attributeArray = new TYPE[va.arraySize];

				for (size_t i = 0; i < va.arraySize; i++) {
					attributeArray[i] = va.attributeArray[i];
				}
			}
		}

		// copy assignment = operator
		vattrib& operator=(const vattrib& va)
		{
			delete[] attributeArray;

			if (va.arraySize) {
				arraySize = va.arraySize;
				count = va.count;
				attributeArray = new TYPE[va.arraySize];

				for (size_t i = 0; i < va.arraySize; i++) {
					attributeArray[i] = va.attributeArray[i];
				}
			} else {
				arraySize = 0;
				count = 0;
				attributeArray = nullptr;
			}

			/* testar caso de copia invalida */

			return *this;
		}

		// move constructor
		vattrib(vattrib&& move) noexcept
			: attributeArray{ move.attributeArray }, arraySize{ move.arraySize }, count{ move.count }
		{
			move.attributeArray = nullptr;
			move.arraySize = 0;
			move.count = 0;
		}

		// move assignment = operator
		vattrib& operator=(vattrib&& move)
		{
			delete[] attributeArray;
			arraySize = std::move(move.arraySize);
			count = std::move(move.count);
			attributeArray = std::move(move.attributeArray);

			move.attributeArray = nullptr;
			move.arraySize = 0;
			move.count = 0;

			return *this;
		}

		// == operator overload
		bool operator==(const vattrib& op) const
		{
			if (arraySize != op.arraySize || count != op.count)
				return false;

			for (int i = 0; i < arraySize; i++) {
				if (attributeArray != op.attributeArray)
					return false;
			}
			
			return true;
		}

		/*
			Total size in bytes
		*/
		size_t getSizeInBytes() const
		{
			return arraySize * sizeof(TYPE);
		}

		/*
			Total of vertex data elements (.i.e. rgba) in the array
		*/
		size_t getSize() const
		{
			return arraySize / count;
		}

		void clear()
		{
			delete[] attributeArray;
			attributeArray = nullptr;
			arraySize = 0;
			count = 0;
		}
	};

	/*
		Class to store geometric models made by vertices.
	*/
	class mesh
	{
		friend class mesh_node; // for perfomance reason
		static const int MAX_VERTEX_ATTRIBUTES = 16; // this value can be obtained by GL_MAX_VERTEX_ATTRIBS

	public:
		mesh();
		~mesh() { clear(); }

		mesh(const mesh& m) = delete; // copy constructor
		mesh& operator=(const mesh& m) = delete; // copy assigment
		mesh(mesh&& m) noexcept; // move constructor
		mesh& operator=(mesh&&) = delete; // move assigment

		/*
			Set the vertex attribute data and its shader input location.
			Note: If an vertex attribute already exists at the location, it will be deleted.
		*/
		void setVertexAttribute(vattrib<float>& vertexAttribute, size_t location);

		/*
			Remove the vertex attribute data
		*/
		void removeVertexAttribute(size_t location);
		
		void clear();

		float* getInterleavedData();

		size_t getSizeInBytes() const {
			return m_sizeInBytes;
		}

		/*
			Return the sum of array elements including all attribute arrays
		*/
		size_t getSize() const {
			return m_size;
		}

		std::string dump() const;
		
		//void setIndices(vattrib<unsigned int>& indices)
		//{
		//	this->indices = indices;
		//}

		//void setMaxVertexAttributes(int value);

	private:
		size_t m_size = 0; // total of array elements
		size_t m_sizeInBytes = 0;
		size_t m_interleavedStride = 0;
		std::unordered_map<size_t, vattrib<float>> m_vattributesMap = {}; // [location, vattribute array]
		std::vector<float> m_interleavedData = {};

		//vattrib<unsigned int> m_indices;
	};

	mesh point(float x, float y, float z);
	mesh triangle(float size);
	mesh quad(float size);
	mesh cube(float size);
	mesh grid();
}

#endif