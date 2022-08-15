/*
	K-Engine KRAW Header
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

#include <keraw.h>
#include <fstream>
#include <keaux.h>


kengine::raw_img::raw_img()
	: width{ 0 }, height{ 0 }, pixels{ nullptr }
{
}


kengine::raw_img::~raw_img()
{
	delete[] pixels;
}


bool kengine::raw_img::loadfile(std::string filename)
{
	std::ifstream filestream(filename, std::ios::in | std::ios::binary);

	if (!filestream)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Fail to load file (RAW_IMG): " << filename);
		// throw std::exception("This file cannot be opened.\n");
		return false;
	}

	filestream.seekg(0, std::ios::end);
	std::streamoff fileSize = filestream.tellg();
	filestream.seekg(0, std::ios::beg);

	char sign[4] = {};
	filestream.read(sign, sizeof(sign));
	filestream.read(reinterpret_cast<char*>(&width), sizeof(int));
	filestream.read(reinterpret_cast<char*>(&height), sizeof(int));

	size_t dataSize = fileSize - (2 * sizeof(int)) - 4;
	pixels = new unsigned char[dataSize];
	filestream.read(reinterpret_cast<char*>(pixels), static_cast<std::streamsize>(dataSize));

	return true;
}


int kengine::raw_img::getWidth() const
{
	return width;
}


int kengine::raw_img::getHeight() const
{
	return height;
}


const unsigned char* const kengine::raw_img::getPixels() const
{
	return pixels;
}


// ----------------------------------------------------------------------------
//  kengine::raw_mesh class
// ----------------------------------------------------------------------------
kengine::mesh kengine::kraw_load_obj(std::string filename)
{
	std::ifstream filestream(filename, std::ios::in | std::ios::binary);

	if (!filestream)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Fail to load file (RAW OBJ): " << filename);
		// throw std::exception("This file cannot be opened.\n");
	}

	//filestream.seekg(0, std::ios::end);
	//std::streamoff fileSize = filestream.tellg();
	//filestream.seekg(0, std::ios::beg);

	char sign[4] = {};
	filestream.read(sign, sizeof(sign));

	// reading kengine::vattrib vertex positions!
	size_t count;
	filestream.read(reinterpret_cast<char*>(&count), sizeof(size_t));
	size_t arraySize;
	filestream.read(reinterpret_cast<char*>(&arraySize), sizeof(size_t));

	float* vertexPosition = new float[arraySize];

	for (size_t i = 0; i < arraySize; i++)
	{
		float data = 0;
		filestream.read(reinterpret_cast<char*>(&data), sizeof(float));
		vertexPosition[i] = data;
	}

	kengine::vattrib<float> v = {
		vertexPosition,
		arraySize,
		count
	};

	kengine::mesh m(v);

	delete[] vertexPosition;

	return m;
}