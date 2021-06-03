/*
	Game Engine Image Library
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

#ifndef K_ENGINE_IMAGE_LIBRARY_H
#define K_ENGINE_IMAGE_LIBRARY_H

#include <string>

namespace DIBLIB
{
	enum { DIBLIB_RGB, DIBLIB_RLE8, DIBLIB_RLE4, DIBLIB_BITFIELDS, DIBLIB_JPEG, DIBLIB_PNG };
	
	class WORD
	{
	public:
		WORD();

		unsigned short get() const;
		void set(unsigned short word);
		void swap();
		unsigned char getByte1();
		unsigned char getByte2();
	private:
		unsigned char byte1;
		unsigned char byte2;
	};

	class DWORD
	{
	public:
		DWORD();

		unsigned long get() const;
		void set(unsigned long dword);
		void swap();
		unsigned char getByte1();
		unsigned char getByte2();
		unsigned char getByte3();
		unsigned char getByte4();
	private:
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
		unsigned char byte4;
	};

	class RGBQUAD
	{
	public:
		RGBQUAD();

		void print();

	private:
		unsigned char rgbBlue;
		unsigned char rgbGreen;
		unsigned char rgbRed;
		unsigned char rgbReserved;
	};

	class BITMAPFILEHEADER
	{
	public:
		void print();
		void swap();

		DWORD getBfSize();
		DWORD getBfOffBits();

	private:
		WORD bfType;
		DWORD bfSize;
		WORD bfReserved1;
		WORD bfReserved2;
		DWORD bfOffBits;
	};

	class BITMAPINFOHEADER
	{
	public:
		void print();
		void swap();

		DWORD getBiSize();
		DWORD getBiWidth();
		DWORD getBiHeight();
		WORD getBiBitCount();
		DWORD getBiCompression();
		DWORD getBiSizeImage();

	private:
		DWORD biSize;
		DWORD biWidth;
		DWORD biHeight;
		WORD biPlanes;
		WORD biBitCount;
		DWORD biCompression;
		DWORD biSizeImage;
		DWORD biXPelsPerMeter;
		DWORD biYPelsPerMeter;
		DWORD biClrUsed;
		DWORD biClrImportant;
	};
}

class DIB
{
public:
	DIB();
	~DIB();

	void loadFile(std::string filename, int swap);
	void release();
	void printDump();
	void printColorIndexDump();

	unsigned long getWidth();
	unsigned long getHeight();
	unsigned char* getColorIndex();
	unsigned long getColorTableSize();
	unsigned long getColorIndexSize();

private:
	DIBLIB::BITMAPFILEHEADER bmfHeader;
	DIBLIB::BITMAPINFOHEADER bmiHeader;
	DIBLIB::RGBQUAD* bmiColors;
	unsigned long colorTableSize;
	unsigned long colorIndexSize;
	unsigned char* colorIndex;
};

#endif