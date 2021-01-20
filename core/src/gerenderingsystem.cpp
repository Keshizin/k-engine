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
#include <GLEXT/wglext.h>
#include <GLEXT/glext.h>

#include <gerenderingsystem.h>
#include <iostream>

// ----------------------------------------------------------------------------
//  OpenGL Procedures Extension for Win32
// ----------------------------------------------------------------------------
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLISBUFFERPROC glIsBuffer = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
PFNGLMAPBUFFERPROC glMapBuffer = 0;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = 0;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = 0;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = 0;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;

// ****************************************************************************
//  GEModel Class Definition - Constructors and Destructors
// ****************************************************************************
GEModel::GEModel()
{
}

GEModel::GEModel(MODEL *modelParam)
{
	this->model = modelParam;
}

GEModel::~GEModel()
{
	delete model;
}

void GEModel::loadToMemory()
{
}

void GEModel::releaseFromMemory()
{
}



// ****************************************************************************
//  Constructors and Destructors
// ****************************************************************************
GERenderingSystem::GERenderingSystem(GEAPIWrapper *apiWrapper)
{
	this->apiWrapper = apiWrapper;
	this->renderingContext = K_CONTEXT_2D;
	this->viewportWidth = 0;
	this->viewportHeight = 0;
	this->windowLeft = 0.0;
	this->windowRight = 0.0;
	this->windowTop = 0.0;
	this->windowBottom = 0.0;
	this->projectionZNear = 0.0;
	this->projectionZFar = 0.0;
	this->projectionFOVY = 0.0;
	this->windowAspectCorrection = 0.0;
	this->windowAspectCorrectionState = false;
	this->globalAxisState = false;
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
	if(!apiWrapper->initializeRenderingSystem())
		return 0;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");

	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glIsBuffer = (PFNGLISBUFFERPROC)wglGetProcAddress("glIsBuffer");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
	glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)wglGetProcAddress("glFlushMappedBufferRange");
	glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)wglGetProcAddress("glCopyBufferSubData");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffers");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");

	if(
		!wglSwapIntervalEXT &&
		!glGenBuffers &&
		!glIsBuffer &&
		!glBindBuffer &&
		!glBufferData &&
		!glBufferSubData &&
		!glMapBuffer &&
		!glUnmapBuffer &&
		!glMapBufferRange &&
		!glFlushMappedBufferRange &&
		!glCopyBufferSubData &&
		!glDeleteBuffers &&
		!glGenVertexArrays &&
		!glBindVertexArray &&
		!glDeleteVertexArrays)
	{
		std::cout << "(!) ERROR - It was not possible to load GL extension: " << glGetError() << "\n" << std::endl;
		return 0;
	}

	return 1;
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
	return wglSwapIntervalEXT(vsync);
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

		std::cout << "@debug | projectionFOVY: " << projectionFOVY << std::endl;
		std::cout << "@debug | windowAspectCorrection: " << windowAspectCorrection << std::endl;
		std::cout << "@debug | projectionZNear: " << projectionZNear << std::endl;
		std::cout << "@debug | projectionZFar: " << projectionZFar << std::endl;

		gluPerspective(projectionFOVY, windowAspectCorrection, projectionZNear, projectionZFar);
	}
	else if(renderingContext == K_CONTEXT_3D_ORTOGRAPHIC)
	{
		glOrtho(windowLeft, windowRight, windowBottom, windowTop, projectionZNear, projectionZFar);
	}
}

void GERenderingSystem::drawGlobaldAxis()
{
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3d(windowLeft, 0.0, 0.0);
	glVertex3d(windowRight, 0.0, 0.0);
	glColor3f(0.0, 1.0f, 0.0f);
	glVertex3d(0.0, windowTop, 0.0);
	glVertex3d(0.0, windowBottom, 0.0);
	glEnd();

	glPopMatrix();
}

void GERenderingSystem::getSystemVersion()
{
	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

// ****************************************************************************
//  Getters and Setters
// ****************************************************************************
void GERenderingSystem::setRenderingContext(int renderingContextParam)
{
	this->renderingContext = renderingContextParam;
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

void GERenderingSystem::setProjectionZNear(double projectionZNearParam)
{
	this->projectionZNear = projectionZNearParam;
}

double GERenderingSystem::getProjectionZNear()
{
	return projectionZNear;
}

void GERenderingSystem::setProjectionZFar(double projectionZFarParam)
{
	this->projectionZFar = projectionZFarParam;
}

double GERenderingSystem::getProjectionZFar()
{
	return projectionZFar;
}

void GERenderingSystem::setProjectionFOVY(double fovyParam)
{
	this->projectionFOVY = fovyParam;
}

double GERenderingSystem::getProjectionFOVY()
{
	return projectionFOVY;
}

void GERenderingSystem::setWindowAspectCorrection(double windowAspectCorrectionParam)
{
	this->windowAspectCorrection = windowAspectCorrectionParam;
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

void GERenderingSystem::setGlobalAxisState(bool state)
{
	globalAxisState = state;
}