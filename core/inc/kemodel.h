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

#ifndef K_ENGINE_MODEL_H
#define K_ENGINE_MODEL_H

#include <iostream>

namespace kengine
{
	// ------------------------------------------------------------------------
	//  struct for store vertex attributes (i.e. positions, colors, etc) 
	// ------------------------------------------------------------------------
	template <typename TYPE>
	struct vattrib
	{
		TYPE* attributeArray;
		size_t arraySize;
		size_t count; // count of elements per attribute (i.e. must be 4 for RGBA)

		size_t getSizeinBytes() const { return arraySize * sizeof(TYPE); }
		size_t getSize() const { return arraySize / count; }

		// default constructor
		vattrib()
			: attributeArray { nullptr }, arraySize { 0 }, count { 0 }
		{
			// std::cout << "> kengine::vattrib default constructor - [" << this << "]" << std::endl;
		}

		// copy constructor
		vattrib(const vattrib& va)
			: attributeArray { nullptr }, arraySize { 0 }, count { 0 }
		{
			// std::cout << "> kengine::vattrib copy constructor - [" << this << "]" << std::endl;

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
			: attributeArray { new TYPE[size] }, arraySize { size }, count { c }
		{
			// std::cout << "> kengine::vattrib constructor with arguments - [" << this << "]" << std::endl;

			for (size_t i = 0; i < size; i++)
			{
				attributeArray[i] = array[i];
			}
		}

		// move constructor
		vattrib(vattrib&& va) noexcept
			: attributeArray { va.attributeArray }, arraySize { va.arraySize }, count { va.count }
		{
			// std::cout << "> kengine::vattrib move constructor - [" << this << "]" << std::endl;
			va.attributeArray = nullptr;
			va.arraySize = 0;
			va.count = 0;
		}

		~vattrib()
		{
			// std::cout << "> kengine::vattrib destructor - [" << this << "]" << std::endl;
			delete attributeArray;
		}

		vattrib& operator=(const vattrib& va)
		{
			// std::cout << "> kengine::vattrib = operator overload - [" << this << "]" << std::endl;

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
	};

	// ------------------------------------------------------------------------
	//  model class is a container for a geometric vertex data
	//  *default constructor and load method must be disabled to avoid
	//   unnecessary copying of data
	// ------------------------------------------------------------------------
	class model
	{
	public:
		//model(); // default constructor
		model(struct vattrib<float>& v, struct vattrib<float>& c, struct vattrib<int>& i);
		model(const model& m); // copy constructor
		model(model&& m) noexcept; // move constructor
		~model();

		model& operator=(const model& m); // copy assigment

		//void load(const struct vattrib<float> &v);
		void dump() const;

		const struct vattrib<float> * getCoords() const { return &coords; }

	private:
		struct vattrib<float> coords;
		struct vattrib<float> colors;
		struct vattrib<int>  indices;
	};
}

#endif