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

namespace kengine
{
	// ------------------------------------------------------------------------
	//   model class is a container for a geometric vertex data
	// ------------------------------------------------------------------------
	class model
	{
	public:
		model(); // default constructor
		model(const model& m); // copy constructor
		model(model&& m); // move constructor
		~model();

		model& operator=(const model& m); // copy assigment

		void load(const float *v, const size_t n);
		void dump() const;
		
		size_t getSizeInBytes() const { return size * sizeof(float); }
		size_t getTotalVertices() const { return size / 3; }
		const float* getVertexArray() const { return vertexArray;  }

	private:
		float* vertexArray;
		size_t size;
	};
}

#endif