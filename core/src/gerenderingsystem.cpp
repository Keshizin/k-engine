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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <gerenderingsystem.h>
#include <iostream>

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
	viewportWidth = width;
	viewportHeight = height;
}

void GERenderingSystem::setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(renderingContext == K_CONTEXT_2D)
	{
		GLdouble left = windowLeft;
		GLdouble right = windowRight;
		GLdouble bottom = windowBottom;
		GLdouble top = windowTop;

		if(windowAspectCorrectionState)
		{
			if(viewportWidth <= viewportHeight)
			{
				windowAspectCorrection = static_cast<GLdouble>(viewportHeight) / static_cast<double>(viewportWidth);
				bottom *= windowAspectCorrection;
				top *= windowAspectCorrection;
			}
			else
			{
				windowAspectCorrection = static_cast<GLdouble>(viewportWidth) / static_cast<double>(viewportHeight);
				left *= windowAspectCorrection;
				right *= windowAspectCorrection;
			}
		}

		glOrtho(left, right, bottom, top, -1.0, 1.0);
	}
	else if(renderingContext == K_CONTEXT_3D_PERSPECTIVE)
	{
		windowAspectCorrection = static_cast<GLdouble>(viewportWidth) / static_cast<double>(viewportHeight);
		gluPerspective(projectionFOVY, windowAspectCorrection, projectionZNear, projectionZFar);
	}
	else if(renderingContext == K_CONTEXT_3D_ORTOGRAPHIC)
	{
		glOrtho(windowLeft, windowRight, windowBottom, windowTop, projectionZNear, projectionZFar);
	}
}

void GERenderingSystem::drawWorldAxis()
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(windowLeft, 0.0f, 0.0f);
	glVertex3f(windowRight, 0.0f, 0.0f);
	glColor3f(0.0, 1.0f, 0.0f);
	glVertex3f(0.0f, windowTop, 0.0f);
	glVertex3f(0.0f, windowBottom, 0.0f);
	glEnd();	
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

void GERenderingSystem::setWindow(double left, double right, double bottom, double top)
{
	this->windowLeft = left;
	this->windowRight = right;
	this->windowTop = top;
	this->windowBottom = bottom;
}

void GERenderingSystem::setProjectionZNear(double projectionZNear)
{
	this->projectionZNear = projectionZNear;
}

double GERenderingSystem::getProjectionZNear()
{
	return projectionZNear;
}

void GERenderingSystem::setProjectionZFar(double projectionZFar)
{
	this->projectionZFar = projectionZFar;
}

double GERenderingSystem::getProjectionZFar()
{
	return projectionZFar;
}

void GERenderingSystem::setProjectionFOVY(double fovy)
{
	this->projectionFOVY;
}

double GERenderingSystem::getProjectionFOVY()
{
	return projectionFOVY;
}

void GERenderingSystem::setWindowAspectCorrection(double windowAspectCorrection)
{
	this->windowAspectCorrection = windowAspectCorrection;
}

double GERenderingSystem::getWindowAspectCorrection()
{
	return windowAspectCorrection;
}

void GERenderingSystem::setWindowAspectCorrectionState(bool state)
{
	this->windowAspectCorrectionState = state;
}

bool GERenderingSystem::getWindowAspectCorrectionState()
{
	return windowAspectCorrectionState;
}