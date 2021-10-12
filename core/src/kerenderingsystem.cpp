/*
	K-Engine Rendering System
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

#include <kerenderingsystem.h>
#include <kewinapiwrapper.h>
#include <keconstants.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>

#include <iostream>

// ****************************************************************************
//  KERenderingSystem Constructors and Destructors
// ****************************************************************************
KERenderingSystem::KERenderingSystem(KEWINAPIWrapper* apiWrapperParam) :
	apiWrapper(apiWrapperParam),
	renderingContext(K_CONTEXT_2D),
	viewportWidth(0),
	viewportHeight(0),
	windowAspectCorrection(1.0),
	windowAspectCorrectionState(true),
	projectionFOVY(0.0),
	projectionZNear(0.0),
	projectionZFar(0.0),
	zoom(0.0),
	renderingWindowOffsetX(0.0),
	renderingWindowOffsetY(0.0)
{
	renderingWindow.left   = 0.0;
	renderingWindow.right  = 0.0;
	renderingWindow.top    = 0.0;
	renderingWindow.bottom = 0.0;
}

// ****************************************************************************
//  KERenderingSystem Public Methods
// ****************************************************************************
void KERenderingSystem::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, (GLsizei) width, (GLsizei) height);
	viewportWidth = width;
	viewportHeight = height;
}

void KERenderingSystem::setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(renderingContext == K_CONTEXT_2D)
	{
		KERECT window;
		window.left = renderingWindow.left;
		window.right = renderingWindow.right;
		window.top = renderingWindow.top;
		window.bottom = renderingWindow.bottom;

		if(windowAspectCorrectionState)
		{
			if(viewportWidth <= viewportHeight)
			{
				windowAspectCorrection = static_cast<double>(viewportHeight) / static_cast<double>(viewportWidth);
				window.bottom *= windowAspectCorrection;
				window.top *= windowAspectCorrection;
			}
			else
			{
				windowAspectCorrection = static_cast<double>(viewportWidth) / static_cast<double>(viewportHeight);
				window.left *= windowAspectCorrection;
				window.right *= windowAspectCorrection;
			}
		}

		glOrtho(
			window.left   + renderingWindowOffsetX + zoom,
			window.right  + renderingWindowOffsetX - zoom,
			window.bottom + renderingWindowOffsetY + zoom,
			window.top    + renderingWindowOffsetY - zoom,
			-1.0,
			1.0);
	}
	else if(renderingContext == K_CONTEXT_3D_PERSPECTIVE)
	{
		windowAspectCorrection = static_cast<double>(viewportWidth) / static_cast<double>(viewportHeight);
		gluPerspective(projectionFOVY, windowAspectCorrection, projectionZNear, projectionZFar);
		// o processo de PAN deve ser realizado através do observador
	}
	else if(renderingContext == K_CONTEXT_3D_ORTOGRAPHIC)
	{
		KERECT window;
		window.left = renderingWindow.left;
		window.right = renderingWindow.right;
		window.top = renderingWindow.top;
		window.bottom = renderingWindow.bottom;

		if(windowAspectCorrectionState)
		{
			if(viewportWidth <= viewportHeight)
			{
				windowAspectCorrection = static_cast<double>(viewportHeight) / static_cast<double>(viewportWidth);
				window.bottom *= windowAspectCorrection;
				window.top *= windowAspectCorrection;
			}
			else
			{
				windowAspectCorrection = static_cast<double>(viewportWidth) / static_cast<double>(viewportHeight);
				window.left *= windowAspectCorrection;
				window.right *= windowAspectCorrection;
			}
		}

		glOrtho(
			window.left   + renderingWindowOffsetX + zoom,
			window.right  + renderingWindowOffsetX - zoom,
			window.bottom + renderingWindowOffsetY + zoom,
			window.top    + renderingWindowOffsetY - zoom,
			projectionZNear,
			projectionZFar);
	}
}

int KERenderingSystem::initialize()
{
	if(!apiWrapper)
	{
		std::cout << "(!) ERROR - It was not possible initialize rendering system: no apiwrapper.\n" << std::endl;
		return 0;
	}

	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
	// apontando para o objeto. Fazer essa validação!
	apiWrapper->initializeRenderingSystem();

	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	return 1;
}

// ****************************************************************************
//  KERenderingSystem - Getters and Setters
// ****************************************************************************
void KERenderingSystem::setRenderingContext(int renderingContextParam)
{
	renderingContext = renderingContextParam;
}

void KERenderingSystem::setRenderingWindow(double left, double right, double bottom, double top)
{
	renderingWindow.left   = left;
	renderingWindow.right  = right;
	renderingWindow.bottom = bottom;
	renderingWindow.top    = top;
}

void KERenderingSystem::setWindowAspectCorrectionState(bool state)
{
	windowAspectCorrectionState = state;
}

void KERenderingSystem::setProjectionFOVY(double fovyParam)
{
	projectionFOVY = fovyParam;
}

double KERenderingSystem::getProjectionFOVY() const
{
	return projectionFOVY;
}

void KERenderingSystem::setProjectionZNear(double projectionZNearParam)
{
	projectionZNear = projectionZNearParam;
}

void KERenderingSystem::setProjectionZFar(double projectionZFarParam)
{
	projectionZFar = projectionZFarParam;
}

void KERenderingSystem::setZoom(double zoomParam)
{
	zoom = zoomParam;
}

double KERenderingSystem::getZoom() const
{
	return zoom;
}

void KERenderingSystem::setRenderingWindowOffsetX(double offset)
{
	renderingWindowOffsetX = offset;
}

double KERenderingSystem::getRenderingWindowOffsetX() const
{
	return renderingWindowOffsetX;
}

void KERenderingSystem::setRenderingWindowOffsetY(double offset)
{
	renderingWindowOffsetY = offset;
}

double KERenderingSystem::getRenderingWindowOffsetY() const
{
	return renderingWindowOffsetY;
}