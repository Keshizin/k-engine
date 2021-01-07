/*
	Game Engine Rendering System
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

#include <gerenderingsystem.h>
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>

// ****************************************************************************
//  Constructors and Destructors
// ****************************************************************************
GERenderingSystem::GERenderingSystem(GEAPIWrapper *apiWrapper)
{
	this->apiWrapper = apiWrapper;
	this->renderingContext = K_CONTEXT_2D;
}

// ****************************************************************************
//  Public Methods
// ****************************************************************************
int GERenderingSystem::initialize()
{
	if(!apiWrapper)
	{
		std::cout << "(!) ERROR - It was not possible initialize rendering system: no apiwrapper.\n" << std::endl;
		return 0;
	}

	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
	// apontando para o objeto. Fazer essa validação!
	return apiWrapper->initializeRenderingSystem();
}

void GERenderingSystem::renderFrame()
{
	if(!apiWrapper)
	{
		std::cout << "(!) ERROR - It was not possible initialize rendering system: no apiwrapper.\n" << std::endl;
		return;
	}

	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
	// apontando para o objeto. Fazer essa validação!

	// UPDATE CAMERA
	// UPDATE SCENE ELEMENTS
	// RENDER SCENE
	// SWAP BUFFERS

	this->apiWrapper->swapBuffers();
}

int GERenderingSystem::setVSync(int vsync)
{
	return apiWrapper->setVSync(vsync);
}

void GERenderingSystem::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, (GLsizei) width, (GLsizei) height);
}

void GERenderingSystem::setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(renderingContext == K_CONTEXT_2D)
	{
		glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
	}
	else if(renderingContext == K_CONTEXT_3D_PERSPECTIVE)
	{
		gluPerspective(45, 1, 1, 100);
	}
	else if(renderingContext == K_CONTEXT_3D_ORTOGRAPHIC)
	{
		glOrtho(-50, 50, -50, 50, 1, 100);
	}
}

// ****************************************************************************
//  Getters and Setters
// ****************************************************************************
void GERenderingSystem::setRenderingContext(int renderingContext)
{
	this->renderingContext = renderingContext;
}

int GERenderingSystem::getRenderingContext()
{
	return renderingContext;
}