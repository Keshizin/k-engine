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

#ifndef K_ENGINE_MESH_H
#define K_ENGINE_MESH_H

namespace kengine
{
	/*
	* 
	*  kengine::vattrib class
	* 
	*  Class for store vertex attributes.
	* 
	*  Note: "vertex attributes" is a term used in OpenGL to represent array of
	*  vertex data (i.e. positions, colors, uv, etc).
	* 
	*/
	template <typename TYPE>
	class vattrib
	{
	public:
		// array of vertex data (heap allocation)
		TYPE*  attributeArray;

		// array size (number of elements)
		size_t arraySize;

		// count of elements per attribute (i.e. must be 4 for RGBA)
		size_t count;


		// default constructor
		vattrib()
			: attributeArray{ nullptr }, arraySize{ 0 }, count{ 0 }
		{
		}


		// copy constructor
		vattrib(const vattrib& va)
			: attributeArray{ nullptr }, arraySize{ 0 }, count{ 0 }
		{
			if (va.arraySize)
			{
				arraySize = va.arraySize;
				count = va.count;
				attributeArray = new TYPE[va.arraySize];

				for (size_t i = 0; i < va.arraySize; i++)
				{
					attributeArray[i] = va.attributeArray[i];
				}
			}
		}


		// constructor with arguments
		vattrib(TYPE* array, size_t size, size_t c)
			: attributeArray{ new TYPE[size] }, arraySize{ size }, count{ c }
		{
			for (size_t i = 0; i < size; i++)
			{
				attributeArray[i] = array[i];
			}
		}


		// move constructor
		vattrib(vattrib&& va) noexcept
			: attributeArray{ va.attributeArray }, arraySize{ va.arraySize }, count{ va.count }
		{
			va.attributeArray = nullptr;
			va.arraySize = 0;
			va.count = 0;
		}


		// destrcutor
		~vattrib()
		{
			delete attributeArray;
		}


		// = operator overload
		vattrib& operator=(const vattrib& va)
		{
			delete attributeArray;

			if (va.arraySize)
			{
				arraySize = va.arraySize;
				count = va.count;
				attributeArray = new TYPE[va.arraySize];

				for (size_t i = 0; i < va.arraySize; i++)
				{
					attributeArray[i] = va.attributeArray[i];
				}
			}
			else
			{
				arraySize = 0;
				count = 0;
				attributeArray = nullptr;
			}

			return *this;
		}


		size_t getSizeinBytes() const
		{
			return arraySize * sizeof(TYPE);
		}


		size_t getSize() const
		{
			return arraySize / count;
		}
		

		void clear()
		{
			delete attributeArray;
			attributeArray = nullptr;
			arraySize = 0;
			count = 0;
		}
	};

	
	/*
	* 
	*  kengine::mesh class
	* 
	*  This is a container for a geometric vertex data.
	* 
	*/
	class mesh
	{
	public:
		mesh();
		mesh(vattrib<float>& v, vattrib<float>& c, vattrib<float>& t, vattrib<float>& n, vattrib<unsigned int>& i);
		mesh(const mesh& m); // copy constructor
		mesh(mesh&& m) noexcept; // move constructor
		~mesh();
		
		mesh& operator=(const mesh& m); // copy assigment

		void dump() const;
		void clear();

		void setPosition(vattrib<float>& v) { position = v; }
		const vattrib<float> * getPosition() const { return &position; }

		void setColors(vattrib<float>& c) { colors = c; }
		const vattrib<float> * getColors() const { return &colors; }

		void setTexCoords(vattrib<float>& t) { texCoords = t; }
		const vattrib<float>* getTexCoords() const { return &texCoords; }

		void setNormals(vattrib<float>& n) { normals = n; }
		const vattrib<float>* getNormals() const { return &normals; }

		void setIndices(vattrib<unsigned int>& i) { indices = i; }
		const vattrib<unsigned int> * getIndices() const { return &indices; }

	private:
		vattrib<float> position;
		vattrib<float> colors;
		vattrib<float> texCoords;
		vattrib<float> normals;
		vattrib<unsigned int> indices;
	};


	/*
	* 
	*  functions to create basic geometric models
	* 
	*/

	mesh triangle(float size);
	mesh quad(float size);
}

#endif