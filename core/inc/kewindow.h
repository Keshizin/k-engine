/*
	K-Engine Window Class
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

#ifndef K_ENGINE_WINDOW_CLASS_H
#define K_ENGINE_WINDOW_CLASS_H

#include <string>
#include <keconstants.h>

class KEWINAPIWrapper;

// ****************************************************************************
//  K-Engine Window Class
// ****************************************************************************
class KEWindow
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructors
	// ------------------------------------------------------------------------
	explicit KEWindow(KEWINAPIWrapper* apiWrapper);

	KEWindow(KEWINAPIWrapper* apiWrapper, int x, int y, int width, int height, std::string name, unsigned int style);

	KEWindow(const KEWindow& win)
		: apiWrapper(0), x(0), y(0), width(0), height(0), name(""), style(K_WINDOW_DEFAULT)
	{
		// (!) Tenha cuidado com chamada implícica do construtor de cópia.
		//     Pode ocorrer problemas se dois objetos apontarem para o mesmo apiWrapper.
	}

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	int create();
	int destroy();
	int show(int showType);

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	void setApiWrapper(KEWINAPIWrapper* apiWrapper);
	KEWINAPIWrapper* getApiWrapper() const;

	void setX(int x);
	int getX() const;

	void setY(int y);
	int getY() const;
	
	void setWidth(int width);
	int getWidth() const;

	void setHeight(int height);
	int getHeight() const;

	void setName(std::string name);
	std::string getName() const;

	void setStyle(unsigned int style);
	unsigned int getStyle() const;

	void setWindow(int x, int y, int width, int height, std::string name, unsigned int style);

private:
	KEWINAPIWrapper* apiWrapper;

	int x;
	int y;
	int width;
	int height;
	std::string name;
	unsigned int style;
};

#endif