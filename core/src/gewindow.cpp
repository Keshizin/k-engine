/*
	Game Engine Window Class
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

#include <gewindow.h>
#include <iostream>

// ****************************************************************************
//  Constructors and Destructors
// ****************************************************************************
GEWindow::GEWindow(GEWINAPIWrapper* apiWrapper)
{
	this->apiWrapper = apiWrapper;
	width = 640;
	height = 480;
	x = 0;
	y = 0;
	style = K_WINDOW_DEFAULT;
}

// ****************************************************************************
//  Public Methods
// ****************************************************************************
int GEWindow::create()
{	
	if(!apiWrapper)
	{
		std::cout << "(!) ERROR - It was not possible create a window: no apiwrapper.\n" << std::endl;
		return 0;
	}

	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
	// apontando para o objeto. Fazer essa validação!
	return apiWrapper->createWindow(x, y, width, height, name, style);
}

int GEWindow::destroy()
{
	if(!apiWrapper)
	{
		std::cout << "(!) ERROR - It was not possible destroy a window: no apiwrapper.\n" << std::endl;
		return 0;
	}

	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
	// apontando para o objeto. Fazer essa validação!
	return apiWrapper->destroyWindow();
}

int GEWindow::show(int showType)
{
	if(!apiWrapper)
	{
		std::cout << "(!) ERROR - It was not possible show a window: no apiwrapper.\n" << std::endl;
		return 0;
	}

	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
	// apontando para o objeto. Fazer essa validação!
	return apiWrapper->showWindow(showType);
}

// ****************************************************************************
//  Getters and Setters
// ****************************************************************************
void GEWindow::setApiWrapper(GEWINAPIWrapper* apiWrapperParam)
{
	this->apiWrapper = apiWrapperParam;
}

GEWINAPIWrapper* GEWindow::getApiWrapper()
{
	return apiWrapper;
}

void GEWindow::setX(int xParam)
{
	this->x = xParam;
}

int GEWindow::getX()
{
	return x;
}

void GEWindow::setY(int yParam)
{
	this->y = yParam;
}

int GEWindow::getY()
{
	return y;
}

void GEWindow::setWidth(int widthParam)
{
	this->width = widthParam;
}

int GEWindow::getWidth()
{
	return width;
}

void GEWindow::setHeight(int heightParam)
{
	this->height = heightParam;
}

int GEWindow::getHeight()
{
	return height;
}

void GEWindow::setName(std::string nameParam)
{
	this->name = nameParam;
}

std::string GEWindow::getName()
{
	return name;
}

void GEWindow::setStyle(unsigned int styleParam)
{
	this->style = styleParam;
}

unsigned int GEWindow::getStyle()
{
	return style;
}