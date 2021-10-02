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

#include <geimage.h>
#include <keaux.h>

#include <iostream>
#include <iomanip>
#include <fstream>

// ----------------------------------------------------------------------------
//  WORD CLASS METHOD DEFINITIONS
// ----------------------------------------------------------------------------
DIBLIB::WORD::WORD() : byte1(0), byte2(0) {}

unsigned short DIBLIB::WORD::get() const
{
	return static_cast<unsigned short>((0 | byte1) << 8 | byte2);
}

void DIBLIB::WORD::set(unsigned short word)
{
	K_UNREFERENCED_PARAMETER(word);
}

void DIBLIB::WORD::swap()
{
	unsigned char temp = byte2;
	byte2 = byte1;
	byte1 = temp;
}

unsigned char DIBLIB::WORD::getByte1()
{
	return byte1;
}

unsigned char DIBLIB::WORD::getByte2()
{
	return byte2;
}

// ----------------------------------------------------------------------------
//  DWORD CLASS METHODS
// ----------------------------------------------------------------------------
DIBLIB::DWORD::DWORD() : byte1(0), byte2(0), byte3(0), byte4(0) {}

unsigned long DIBLIB::DWORD::get() const
{
	return static_cast<unsigned long>((((0 | byte1) << 8 | byte2) << 8 | byte3) << 8 | byte4);
}

void DIBLIB::DWORD::set(unsigned long dword)
{
	K_UNREFERENCED_PARAMETER(dword);
}

void DIBLIB::DWORD::swap()
{
	unsigned char temp = byte1;
	byte1 = byte4;
	byte4 = temp;
	temp = byte2;
	byte2 = byte3;
	byte3 = temp;
}

unsigned char DIBLIB::DWORD::getByte1()
{
	return byte1;
}

unsigned char DIBLIB::DWORD::getByte2()
{
	return byte2;
}

unsigned char DIBLIB::DWORD::getByte3()
{
	return byte3;
}

unsigned char DIBLIB::DWORD::getByte4()
{
	return byte4;
}

// ----------------------------------------------------------------------------
//  RGBQUAD CLASS METHODS
// ----------------------------------------------------------------------------
DIBLIB::RGBQUAD::RGBQUAD() : rgbBlue(0), rgbGreen(0), rgbRed(0), rgbReserved(0) {}

void DIBLIB::RGBQUAD::print()
{
	std::cout
		<< "================================================================================"
		<< "\n R G B Q U A D   -   " << sizeof(DIBLIB::RGBQUAD) << " bytes"
		<< "\n================================================================================"
		<< "\n    rgbBlue: " << std::setw(4) << static_cast<int>(rgbBlue)
		<< "\n   rgbGreen: " << std::setw(4) << static_cast<int>(rgbGreen)
		<< "\n      gbRed: " << std::setw(4) << static_cast<int>(rgbRed)
		<< "\nrgbReserved: " << std::setw(4) << static_cast<int>(rgbReserved)
		<< "\n================================================================================" << std::endl;
}

// ----------------------------------------------------------------------------
//  BITMAPFILEHEADER CLASS METHODS
// ----------------------------------------------------------------------------
void DIBLIB::BITMAPFILEHEADER::print()
{
	std::cout
		<< "================================================================================"
		<< "\n B I T M A P   F I L E   H E A D E R   -   " << sizeof(DIBLIB::BITMAPFILEHEADER) << " bytes"
		<< "\n================================================================================"
		<< "\n     bfType: "
		<< std::setw(4) << bfType.getByte1()
		<< std::setw(4) << bfType.getByte2()
		<< std::setw(4) << "-"
		<< std::setw(4) << "-" << " | "
		<< bfType.get()
		<< "\n     bfSize: "
		<< std::setw(4) << static_cast<int>(bfSize.getByte1())
		<< std::setw(4) << static_cast<int>(bfSize.getByte2())
		<< std::setw(4) << static_cast<int>(bfSize.getByte3())
		<< std::setw(4) << static_cast<int>(bfSize.getByte4()) << " | "
		<< bfSize.get()
		<< "\nbfReserved1: "
		<< std::setw(4) << static_cast<int>(bfReserved1.getByte1())
		<< std::setw(4) << static_cast<int>(bfReserved1.getByte2())
		<< std::setw(4) << "-"
		<< std::setw(4) << "-" << " | "
		<< bfReserved1.get()
		<< "\nbfReserved2: "
		<< std::setw(4) << static_cast<int>(bfReserved2.getByte1())
		<< std::setw(4) << static_cast<int>(bfReserved2.getByte2())
		<< std::setw(4) << "-"
		<< std::setw(4) << "-" << " | "
		<< bfReserved2.get()
		<< "\n  bfOffBits: "
		<< std::setw(4) << static_cast<int>(bfOffBits.getByte1())
		<< std::setw(4) << static_cast<int>(bfOffBits.getByte2())
		<< std::setw(4) << static_cast<int>(bfOffBits.getByte3())
		<< std::setw(4) << static_cast<int>(bfOffBits.getByte4()) << " | "
		<< bfOffBits.get()
		<< "\n================================================================================" << std::endl;
}

void DIBLIB::BITMAPFILEHEADER::swap()
{
	bfSize.swap();
	bfReserved1.swap();
	bfReserved2.swap();
	bfOffBits.swap();
}

DIBLIB::DWORD DIBLIB::BITMAPFILEHEADER::getBfSize()
{
	return bfSize;
}

DIBLIB::DWORD DIBLIB::BITMAPFILEHEADER::getBfOffBits()
{
	return bfOffBits;
}


// ----------------------------------------------------------------------------
//  BITMAPINFOHEADER CLASS METHODS
// ----------------------------------------------------------------------------
void DIBLIB::BITMAPINFOHEADER::print()
{
	std::cout
		<< "================================================================================"
		<< "\n B I T M A P   I N F O   H E A D E R   -   " << sizeof(DIBLIB::BITMAPINFOHEADER) << " bytes"
		<< "\n================================================================================"
		<< "\n         biSize: "
		<< std::setw(4) << static_cast<int>(biSize.getByte1())
		<< std::setw(4) << static_cast<int>(biSize.getByte2())
		<< std::setw(4) << static_cast<int>(biSize.getByte3())
		<< std::setw(4) << static_cast<int>(biSize.getByte4()) << " | "
		<< biSize.get()
		<< "\n        biWidth: "
		<< std::setw(4) << static_cast<int>(biWidth.getByte1())
		<< std::setw(4) << static_cast<int>(biWidth.getByte2())
		<< std::setw(4) << static_cast<int>(biWidth.getByte3())
		<< std::setw(4) << static_cast<int>(biWidth.getByte4()) << " | "
		<< biWidth.get()
		<< "\n       biHeight: "
		<< std::setw(4) << static_cast<int>(biHeight.getByte1())
		<< std::setw(4) << static_cast<int>(biHeight.getByte2())
		<< std::setw(4) << static_cast<int>(biHeight.getByte3())
		<< std::setw(4) << static_cast<int>(biHeight.getByte4()) << " | "
		<< biHeight.get()
		<< "\n       biPlanes: "
		<< std::setw(4) << static_cast<int>(biPlanes.getByte1())
		<< std::setw(4) << static_cast<int>(biPlanes.getByte2())
		<< std::setw(4) << "-"
		<< std::setw(4) << "-" << " | "
		<< biPlanes.get()
		<< "\n     biBitCount: "
		<< std::setw(4) << static_cast<int>(biBitCount.getByte1())
		<< std::setw(4) << static_cast<int>(biBitCount.getByte2())
		<< std::setw(4) << "-"
		<< std::setw(4) << "-" << " | "
		<< biBitCount.get()
		<< "\n  biCompression: "
		<< std::setw(4) << static_cast<int>(biCompression.getByte1())
		<< std::setw(4) << static_cast<int>(biCompression.getByte2())
		<< std::setw(4) << static_cast<int>(biCompression.getByte3())
		<< std::setw(4) << static_cast<int>(biCompression.getByte4()) << " | "
		<< biCompression.get()
		<< "\n    biSizeImage: "
		<< std::setw(4) << static_cast<int>(biSizeImage.getByte1())
		<< std::setw(4) << static_cast<int>(biSizeImage.getByte2())
		<< std::setw(4) << static_cast<int>(biSizeImage.getByte3())
		<< std::setw(4) << static_cast<int>(biSizeImage.getByte4()) << " | "
		<< biSizeImage.get()
		<< "\nbiXPelsPerMeter: "
		<< std::setw(4) << static_cast<int>(biXPelsPerMeter.getByte1())
		<< std::setw(4) << static_cast<int>(biXPelsPerMeter.getByte2())
		<< std::setw(4) << static_cast<int>(biXPelsPerMeter.getByte3())
		<< std::setw(4) << static_cast<int>(biXPelsPerMeter.getByte4()) << " | "
		<< biXPelsPerMeter.get()
		<< "\nbiYPelsPerMeter: "
		<< std::setw(4) << static_cast<int>(biYPelsPerMeter.getByte1())
		<< std::setw(4) << static_cast<int>(biYPelsPerMeter.getByte2())
		<< std::setw(4) << static_cast<int>(biYPelsPerMeter.getByte3())
		<< std::setw(4) << static_cast<int>(biYPelsPerMeter.getByte4()) << " | "
		<< biYPelsPerMeter.get()
		<< "\n      biClrUsed: "
		<< std::setw(4) << static_cast<int>(biClrUsed.getByte1())
		<< std::setw(4) << static_cast<int>(biClrUsed.getByte2())
		<< std::setw(4) << static_cast<int>(biClrUsed.getByte3())
		<< std::setw(4) << static_cast<int>(biClrUsed.getByte4()) << " | "
		<< biClrUsed.get()
		<< "\n biClrImportant: "
		<< std::setw(4) << static_cast<int>(biClrImportant.getByte1())
		<< std::setw(4) << static_cast<int>(biClrImportant.getByte2())
		<< std::setw(4) << static_cast<int>(biClrImportant.getByte3())
		<< std::setw(4) << static_cast<int>(biClrImportant.getByte4()) << " | "
		<< biClrImportant.get()
		<< "\n================================================================================" << std::endl;
}

void DIBLIB::BITMAPINFOHEADER::swap()
{
	biSize.swap();
	biWidth.swap();
	biHeight.swap();
	biPlanes.swap();
	biBitCount.swap();
	biCompression.swap();
	biSizeImage.swap();
	biXPelsPerMeter.swap();
	biYPelsPerMeter.swap();
	biClrUsed.swap();
	biClrImportant.swap();
}

DIBLIB::DWORD DIBLIB::BITMAPINFOHEADER::getBiSize()
{
	return biSize;
}

DIBLIB::DWORD DIBLIB::BITMAPINFOHEADER::getBiWidth()
{
	return biWidth;
}

DIBLIB::DWORD DIBLIB::BITMAPINFOHEADER::getBiHeight()
{
	return biHeight;
}

DIBLIB::WORD DIBLIB::BITMAPINFOHEADER::getBiBitCount()
{
	return biBitCount;
}

DIBLIB::DWORD DIBLIB::BITMAPINFOHEADER::getBiCompression()
{
	return biCompression;
}

DIBLIB::DWORD DIBLIB::BITMAPINFOHEADER::getBiSizeImage()
{
	return biSizeImage;
}

// ----------------------------------------------------------------------------
//  DIB CLASS METHODS
// ----------------------------------------------------------------------------
DIB::DIB() : bmiColors(0), colorTableSize(0), colorIndexSize(0), colorIndex(0) {}

DIB::~DIB()
{
	delete bmiColors;
	delete colorIndex;
}

void DIB::loadFile(std::string filename, int swap)
{
	std::ifstream bitmapFile(filename, std::ios::in | std::ios::binary);

	if (!bitmapFile)
	{
		// remove the output below and throw exception!
		std::cout << "(!) This file (" << filename << ") cannot be opened.\n" << std::endl;
		return;
	}

	// reading BITMAPFILEHEADER
	bitmapFile.read(reinterpret_cast<char*>(&bmfHeader), sizeof(DIBLIB::BITMAPFILEHEADER));
	bmfHeader.swap();

	// reading BITMAPINFOHEADER
	bitmapFile.read(reinterpret_cast<char*>(&bmiHeader), sizeof(DIBLIB::BITMAPINFOHEADER));
	bmiHeader.swap();

	// reading RGBQUAD (COLOR TABLE)
	colorTableSize = bmfHeader.getBfOffBits().get() - (bmiHeader.getBiSize().get() + sizeof(DIBLIB::BITMAPFILEHEADER));

	if (colorTableSize)
	{
		colorTableSize /= sizeof(DIBLIB::RGBQUAD);
		bmiColors = new DIBLIB::RGBQUAD[colorTableSize];

		for (unsigned long i = 0; i < colorTableSize; i++)
		{
			bitmapFile.read(reinterpret_cast<char*>(&bmiColors[i]), sizeof(DIBLIB::RGBQUAD));
		}
	}

	// reading COLOR INDEX ARRAY
	colorIndexSize = bmfHeader.getBfSize().get() - bmfHeader.getBfOffBits().get();
	colorIndex = new unsigned char[colorIndexSize];

	if (swap)
	{
		unsigned long index = 0;
		unsigned long width;
		long height;
		unsigned long offset;

		switch (bmiHeader.getBiBitCount().get())
		{
		case 24:
			width = bmiHeader.getBiWidth().get();
			height = static_cast<long>(bmiHeader.getBiHeight().get());
			offset = (24 * width + 31) / 32 * 4 - width * 3;

			if (height < 0)
				height *= -1;

			for (long lines = 0; lines < height; lines++)
			{
				for (unsigned bytes = 0; bytes < width; bytes++)
				{
					bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index + 2]), sizeof(unsigned char));
					bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index + 1]), sizeof(unsigned char));
					bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index]), sizeof(unsigned char));
					index += 3;
				}

				for (unsigned bytes = 0; bytes < offset; bytes++)
				{
					bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index++]), sizeof(unsigned char));
				}
			}

			break;

		case 32:
			if (bmiHeader.getBiCompression().get() == DIBLIB::DIBLIB_RGB)
			{
				width = bmiHeader.getBiWidth().get();
				height = static_cast<long>(bmiHeader.getBiHeight().get());
				offset = (32 * width + 31) / 32 * 4 - width * 4;

				if (height < 0)
					height *= -1;

				for (long lines = 0; lines < height; lines++)
				{
					for (unsigned bytes = 0; bytes < width; bytes++)
					{
						bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index + 2]), sizeof(unsigned char));
						bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index + 1]), sizeof(unsigned char));
						bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index]), sizeof(unsigned char));
						bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index + 3]), sizeof(unsigned char));
						index += 4;
					}

					for (unsigned bytes = 0; bytes < offset; bytes++)
					{
						bitmapFile.read(reinterpret_cast<char*>(&colorIndex[index++]), sizeof(unsigned char));
					}
				}
			}

			break;
		}
	}
	else
	{
		bitmapFile.read(reinterpret_cast<char*>(colorIndex), colorIndexSize);
	}

}

void DIB::release()
{
	delete bmiColors;
	bmiColors = 0;
	colorTableSize = 0;
	delete colorIndex;
	colorIndex = 0;
	colorIndexSize = 0;
}

void DIB::printColorIndexDump()
{
	std::cout
		<< "================================================================================"
		<< "\n C O L O R   I N D E X   D U M P -   " << colorIndexSize << " bytes"
		<< "\n================================================================================"
		<< std::endl;

	for (unsigned long i = 0; i < colorIndexSize; i++)
	{
		std::cout << std::setw(4) << std::hex << static_cast<int>(colorIndex[i]);

		if (((i + 1) % 20) == 0)
		{
			std::cout << std::endl;
		}
	}

	std::cout << std::dec
		<< "\n================================================================================" << std::endl;
}

void DIB::printDump()
{
	bmfHeader.print();
	bmiHeader.print();

	for (unsigned long i = 0; i < colorTableSize; i++)
		bmiColors[i].print();

	printColorIndexDump();
}

unsigned long DIB::getWidth()
{
	return bmiHeader.getBiWidth().get();
}

unsigned long DIB::getHeight()
{
	return bmiHeader.getBiHeight().get();
}

unsigned char* DIB::getColorIndex()
{
	return colorIndex;
}

unsigned long DIB::getColorTableSize()
{
	return colorTableSize;
}

unsigned long DIB::getColorIndexSize()
{
	return colorIndexSize;
}