/*
	K-Engine Image Library
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

namespace KEIMAGE
{
	enum { KEIMAGE_RGB, KEIMAGE_RLE8, KEIMAGE_RLE4, KEIMAGE_BITFIELDS, KEIMAGE_JPEG, KEIMAGE_PNG };
	
	class WORD
	{
	public:
		WORD();

		unsigned short get() const;
		void set(unsigned short word);
		void swap();
		unsigned char getByte1() const;
		unsigned char getByte2() const;
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
		unsigned char getByte1() const;
		unsigned char getByte2() const;
		unsigned char getByte3() const;
		unsigned char getByte4() const;
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

		void print() const;

	private:
		unsigned char rgbBlue;
		unsigned char rgbGreen;
		unsigned char rgbRed;
		unsigned char rgbReserved;
	};

	class BITMAPFILEHEADER
	{
	public:
		void print() const;
		void swap();

		DWORD getBfSize() const;
		DWORD getBfOffBits() const;

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
		void print() const;
		void swap();

		DWORD getBiSize() const;
		DWORD getBiWidth() const;
		DWORD getBiHeight() const;
		WORD getBiBitCount() const;
		DWORD getBiCompression() const;
		DWORD getBiSizeImage() const;

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

	void loadfile(std::string filename, int swap);
	void release();
	void printDump() const;
	void printColorIndexDump() const;

	unsigned long getBiBitCount() const;
	unsigned long getWidth() const;
	unsigned long getHeight() const ;
	unsigned char* getColorIndex() const;
	unsigned long getColorTableSize() const;
	unsigned long getColorIndexSize() const;

private:
	KEIMAGE::BITMAPFILEHEADER bmfHeader;
	KEIMAGE::BITMAPINFOHEADER bmiHeader;
	KEIMAGE::RGBQUAD* bmiColors;
	unsigned long colorTableSize;
	unsigned long colorIndexSize;
	unsigned char* colorIndex;
};

#endif