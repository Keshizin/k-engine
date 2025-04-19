/*
	K-Engine Media Library
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

#ifndef K_ENGINE_MEDIA_HPP
#define K_ENGINE_MEDIA_HPP

#include <string>
#include <fstream>

namespace kengine
{
	/*
		kengine::raw_img class
	*/
	class k_img
	{
	public:
		k_img() {}
		~k_img() { delete[] pixels; }

		k_img(const k_img& copy) = delete; // copy constructor
		k_img(k_img&& move) noexcept = delete; // move constructor
		k_img& operator=(const k_img& copy) = delete; // copy assignment
		k_img& operator=(k_img&&) = delete; // move assigment

		bool loadfile(const std::string& filename)
		{
			std::ifstream filestream(filename, std::ios::in | std::ios::binary);

			if (!filestream)
			{
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

		bool writeFile(std::string filename, int widthParam, int heightParam, void* data, long long data_size) {
			std::ofstream file_stream(filename, std::ios::binary);

			if (!file_stream)
			{
				return false;
			}

			char sign[] = { 'K', 'I', 'M', 'G' };
			file_stream.write(reinterpret_cast<const char*>(&sign), sizeof(sign));
			file_stream.write(reinterpret_cast<const char*>(&width), sizeof(int));
			file_stream.write(reinterpret_cast<const char*>(&height), sizeof(int));
			file_stream.write(reinterpret_cast<const char*>(data), data_size);

			return true;
		}

		int getWidth() const { return width; }
		int getHeight() const { return height; }
		unsigned char* getPixels() { return pixels; }

	private:
		int width = 0;
		int height = 0;
		unsigned char* pixels = nullptr;
	};
}

#endif