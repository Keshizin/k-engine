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
#include <keaux.h>
#include <fstream>


/*
*
*  kengine::raw_img class - member class definition
*
*/
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


/*
*
*  function to load obj files
*
*/
kengine::mesh kengine::kraw_load_obj(std::string filename)
{
	std::ifstream filestream(filename, std::ios::in | std::ios::binary);

	if (!filestream)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Fail to load file (RAW OBJ): " << filename);
		// throw std::exception("This file cannot be opened.\n");
	}

	char sign[4] = {};
	filestream.read(sign, sizeof(sign));

	unsigned char attrib_count = 0;
	filestream.read(reinterpret_cast<char*>(&attrib_count), sizeof(unsigned char));

	kengine::mesh m;

	size_t count = 0;
	size_t arraySize = 0;
	float* vertexAttribute = nullptr;


	/*
	* 
	*  reading kengine::vattrib vertex positions!
	* 
	*/

	if (attrib_count & 0x80)
	{
		filestream.read(reinterpret_cast<char*>(&count), sizeof(size_t));
		filestream.read(reinterpret_cast<char*>(&arraySize), sizeof(size_t));

		vertexAttribute = new float[arraySize];

		for (size_t i = 0; i < arraySize; i++)
		{
			float data = 0;
			filestream.read(reinterpret_cast<char*>(&data), sizeof(float));
			vertexAttribute[i] = data;
		}

		kengine::vattrib<float> v = {
			vertexAttribute,
			arraySize,
			count
		};

		m.setPosition(v);
		delete[] vertexAttribute;
	}

	/*
	* 
	*  reading kengine::vattrib vertex colors!
	* 
	*/

	if (attrib_count & 0x40)
	{ 
		filestream.read(reinterpret_cast<char*>(&count), sizeof(size_t));
		filestream.read(reinterpret_cast<char*>(&arraySize), sizeof(size_t));
		vertexAttribute = new float[arraySize];

		for (size_t i = 0; i < arraySize; i++)
		{
			float data = 0;
			filestream.read(reinterpret_cast<char*>(&data), sizeof(float));
			vertexAttribute[i] = data;
		}

		kengine::vattrib<float> c = {
			vertexAttribute,
			arraySize,
			count
		};

		m.setColors(c);
		delete[] vertexAttribute;
	}

	/*
	* 
	*  reading kengine::vattrib texture vertices!
	* 
	*/

	if (attrib_count & 0x20)
	{
		filestream.read(reinterpret_cast<char*>(&count), sizeof(size_t));
		filestream.read(reinterpret_cast<char*>(&arraySize), sizeof(size_t));
		vertexAttribute = new float[arraySize];

		for (size_t i = 0; i < arraySize; i++)
		{
			float data = 0;
			filestream.read(reinterpret_cast<char*>(&data), sizeof(float));
			vertexAttribute[i] = data;
		}

		kengine::vattrib<float> t = {
			vertexAttribute,
			arraySize,
			count
		};

		m.setTexCoords(t);
		delete[] vertexAttribute;
	}


	/*
	* 
	*  reading kengine::vattrib vertex normals!
	* 
	*/

	if (attrib_count & 0x10)
	{
		filestream.read(reinterpret_cast<char*>(&count), sizeof(size_t));
		filestream.read(reinterpret_cast<char*>(&arraySize), sizeof(size_t));
		vertexAttribute = new float[arraySize];

		for (size_t i = 0; i < arraySize; i++)
		{
			float data = 0;
			filestream.read(reinterpret_cast<char*>(&data), sizeof(float));
			vertexAttribute[i] = data;
		}

		kengine::vattrib<float> n = {
			vertexAttribute,
			arraySize,
			count
		};

		m.setNormals(n);
		delete[] vertexAttribute;
	}

	return m;
}