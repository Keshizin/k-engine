/*
	Game Engine Window Class
	This file is part of the K-Engine.

	Copyright (C) 2020 Fabio Takeshi Ishikawa

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

#include <geapiwrapper.h>

// ****************************************************************************
//  K-Engine Window Style
// ****************************************************************************
#define K_WINDOW_SPLASH              0x01
#define K_WINDOW_DEFAULT             0x02
#define K_WINDOW_NO_SYS              0x03
#define K_WINDOW_WINDOWED_FULLSCREEN 0x04
#define K_WINDOW_COMPLETE            0x05

// ****************************************************************************
//  K-Engine Window Show Types
// ****************************************************************************
#define K_WINDOW_SHOW 0x05
#define K_WINDOW_FORCEMINIMIZE 0x0B

// ****************************************************************************
//  Game Engine Window Class
// ****************************************************************************
class GEWindow
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructors
	// ------------------------------------------------------------------------
	GEWindow(GEAPIWrapper *apiWrapper);

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	int create();
	int destroy();
	int show(int showType);

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	void setApiWrapper(GEAPIWrapper *apiWrapper);
	GEAPIWrapper *getApiWrapper();

	void setX(int x);
	int getX();

	void setY(int y);
	int getY();
	
	void setWidth(int width);
	int getWidth();

	void setHeight(int height);
	int getHeight();

	void setName(std::string name);
	std::string getName();

	void setStyle(unsigned int style);
	unsigned int getStyle();

private:
	GEAPIWrapper *apiWrapper;
	int x;
	int y;
	int width;
	int height;
	std::string name;
	unsigned int style;
};

#endif