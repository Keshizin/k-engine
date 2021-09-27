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

#include <kewindow.h>
#include <iostream>

// ****************************************************************************
//  Constructors and Destructors
// ****************************************************************************
KEWindow::KEWindow(KEWINAPIWrapper* apiWrapper)
	: apiWrapper(apiWrapper), x(0), y(0), width(640), height(640), name("K-ENGINE DEFAULT NAME!"), style(K_WINDOW_DEFAULT)
{
}

KEWindow::KEWindow(KEWINAPIWrapper* apiWrapper, int x, int y, int width, int height, std::string name, unsigned int style)
	: apiWrapper(apiWrapper), x(x), y(y), width(width), height(height), name(name), style(style)
{
}

// ****************************************************************************
//  Public Methods
// ****************************************************************************
int KEWindow::create()
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

int KEWindow::destroy()
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

int KEWindow::show(int showType)
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
void KEWindow::setApiWrapper(KEWINAPIWrapper* apiWrapperParam)
{
	this->apiWrapper = apiWrapperParam;
}

KEWINAPIWrapper* KEWindow::getApiWrapper() const
{
	return apiWrapper;
}

void KEWindow::setX(int xParam)
{
	this->x = xParam;
}

int KEWindow::getX() const
{
	return x;
}

void KEWindow::setY(int yParam)
{
	this->y = yParam;
}

int KEWindow::getY() const
{
	return y;
}

void KEWindow::setWidth(int widthParam)
{
	this->width = widthParam;
}

int KEWindow::getWidth() const
{
	return width;
}

void KEWindow::setHeight(int heightParam)
{
	this->height = heightParam;
}

int KEWindow::getHeight() const
{
	return height;
}

void KEWindow::setName(std::string nameParam)
{
	this->name = nameParam;
}

std::string KEWindow::getName() const
{
	return name;
}

void KEWindow::setStyle(unsigned int styleParam)
{
	this->style = styleParam;
}

unsigned int KEWindow::getStyle() const
{
	return style;
}

void KEWindow::setWindow(int x, int y, int width, int height, std::string name, unsigned int style)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->name = name;
	this->style = style;
}