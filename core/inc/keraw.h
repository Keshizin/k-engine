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

#ifndef K_ENGINE_RAW_H
#define K_ENGINE_RAW_H

#include <kemesh.h>
#include <string>

namespace kengine
{

	/*
	* 
	*  kengine::raw_img class
	* 
	*/
	class raw_img
	{
	public:
		raw_img();
		~raw_img();

		raw_img(const raw_img& copy) = delete; // copy constructor
		raw_img(raw_img&& move) noexcept = delete; // move constructor
		raw_img& operator=(const raw_img& copy) = delete; // copy assignment

		bool loadfile(std::string filename);

		int getWidth() const;
		int getHeight() const;
		const unsigned char* const getPixels() const;

	private:
		int width;
		int height;
		unsigned char* pixels;
	};


	/*
	* 
	*  function to load obj files
	* 
	*/
	kengine::mesh kraw_load_obj(std::string filename);
}

#endif