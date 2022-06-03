/*
	K-Engine Geometric Model
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

#ifndef K_ENGINE_MODEL_H
#define K_ENGINE_MODEL_H

namespace kengine
{
	// ------------------------------------------------------------------------
	//  struct for store vertex attributes (i.e. positions, colors, etc) 
	// ------------------------------------------------------------------------
	template <typename TYPE>
	struct vattrib
	{
		TYPE*  attributeArray;
		size_t arraySize;
		size_t count; // count of elements per attribute (i.e. must be 4 for RGBA)

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

		size_t getSizeinBytes() const { return arraySize * sizeof(TYPE); }
		size_t getSize() const { return arraySize / count; }
		
		void clear()
		{
			delete attributeArray;
			attributeArray = nullptr;
			arraySize = 0;
			count = 0;
		}
	};

	// ------------------------------------------------------------------------
	//  (!) kengine::model class
	// 
	//  This is a container for a geometric vertex data
	// ------------------------------------------------------------------------
	class model
	{
	public:
		//model(); // default constructor disabled to avoid unncessary copying of data
		model(struct vattrib<float>& v, struct vattrib<float>& c, struct vattrib<float>& t, struct vattrib<unsigned int>& i);
		model(const model& m); // copy constructor
		model(model&& m) noexcept; // move constructor
		~model();
		
		model& operator=(const model& m); // copy assigment

		//void load(const struct vattrib<float> &v); // disabled to avoid unncessary copying of data
		void dump() const;
		size_t getSizeInBytes() const { return coords.getSizeinBytes() + colors.getSizeinBytes() + texCoords.getSizeinBytes(); }
		void release();

		const struct vattrib<float> * getCoords() const { return &coords; }
		const struct vattrib<float> * getColors() const { return &colors; }
		const struct vattrib<float>* getTexCoords() const { return &texCoords; }
		const struct vattrib<unsigned int> * getIndices() const { return &indices; }

	private:
		struct vattrib<float> coords;
		struct vattrib<float> colors;
		struct vattrib<float> texCoords;
		struct vattrib<unsigned int>  indices;
	};

	// ------------------------------------------------------------------------
	//  functions to create basic geometric models
	// ------------------------------------------------------------------------
	model triangle(float size);
	model quad(float size);
	model cube(float size);
}

#endif