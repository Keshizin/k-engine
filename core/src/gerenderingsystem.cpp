// /*
// 	Game Engine Rendering System
// 	This file is part of the K-Engine.

// 	Copyright (C) 2021 Fabio Takeshi Ishikawa

// 	Permission is hereby granted, free of charge, to any person obtaining a copy
// 	of this software and associated documentation files (the "Software"), to deal
// 	in the Software without restriction, including without limitation the rights
// 	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// 	copies of the Software, and to permit persons to whom the Software is
// 	furnished to do so, subject to the following conditions:

// 	The above copyright notice and this permission notice shall be included in all
// 	copies or substantial portions of the Software.

// 	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// 	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// 	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// 	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// 	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// 	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// 	SOFTWARE.
// */

// #define WIN32_LEAN_AND_MEAN
// #include <windows.h>

// #include <GL/gl.h>
// #include <GL/glu.h>
// #include <GLEXT/wglext.h>
// #include <GLEXT/glext.h>

// #include <gerenderingsystem.h>
// #include <iostream>

// // ----------------------------------------------------------------------------
// //  OpenGL Procedures Extension for Win32
// // ----------------------------------------------------------------------------
// PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = 0;
// PFNGLGENBUFFERSPROC glGenBuffers = 0;
// PFNGLISBUFFERPROC glIsBuffer = 0;
// PFNGLBINDBUFFERPROC glBindBuffer = 0;
// PFNGLBUFFERDATAPROC glBufferData = 0;
// PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
// PFNGLMAPBUFFERPROC glMapBuffer = 0;
// PFNGLUNMAPBUFFERPROC glUnmapBuffer = 0;
// PFNGLMAPBUFFERRANGEPROC glMapBufferRange = 0;
// PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = 0;
// PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = 0;
// PFNGLDELETEBUFFERSARBPROC glDeleteBuffers = 0;
// PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
// PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
// PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;

// // ****************************************************************************
// //  GEModel Class Definition - Constructors and Destructors
// // ****************************************************************************
// GEEntity::GEEntity()
// {
// 	translate.x = 0.0f;
// 	translate.y = 0.0f;
// 	translate.z = 0.0f;

// 	rotate.angle = 0.0f;
// 	rotate.x = 0.0f;
// 	rotate.y = 0.0f;
// 	rotate.z = 0.0f;

// 	scale.x  = 1.0f;
// 	scale.y  = 1.0f;
// 	scale.z  = 1.0f;

// 	speed.x = 0.0f;
// 	speed.y = 0.0f;
// 	speed.z = 0.0f;

// 	bounding.left = 0.0f;
// 	bounding.right = 0.0f;
// 	bounding.top = 0.0f;
// 	bounding.bottom = 0.0f;
// }

// GEEntity::GEEntity(MODEL *modelParam)
// {
// 	this->model = modelParam;
	
// 	translate.x = 0.0f;
// 	translate.y = 0.0f;
// 	translate.z = 0.0f;

// 	rotate.angle = 0.0f;
// 	rotate.x = 0.0f;
// 	rotate.y = 0.0f;
// 	rotate.z = 0.0f;

// 	scale.x  = 1.0f;
// 	scale.y  = 1.0f;
// 	scale.z  = 1.0f;

// 	speed.x = 0.0f;
// 	speed.y = 0.0f;
// 	speed.z = 0.0f;

// 	bounding.left = 0.0f;
// 	bounding.right = 0.0f;
// 	bounding.top = 0.0f;
// 	bounding.bottom = 0.0f;
// }

// GEEntity::~GEEntity()
// {
// 	// releaseFromMemory();
// 	delete model->vertices;
// 	delete model->colors;
// 	delete model->indices;
// 	delete model;
// }

// void GEEntity::update(double frameTime)
// {
// 	glPushMatrix();
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();

// 	translate.x += speed.x * frameTime;

// 	if(translate.x - 0.5f > bounding.right)
// 		translate.x = bounding.left - 0.5f;

// 	rotate.angle += 500 * speed.x * frameTime;

// 	if(rotate.angle > 360.0f)
// 		rotate.angle = 0.0f;

// 	glTranslatef(translate.x, translate.y, translate.z);
// 	glRotatef(rotate.angle, rotate.x, rotate.y, rotate.z);
// 	glScalef(scale.x, scale.y, scale.z);
// }

// void GEEntity::draw()
// {
// 	// This code will be replaced for VAO, VBO, vertex/fragment shader techniques!
// 	glBegin(GL_TRIANGLES);

// 	for(int i = 0; i < model->total_indices; i++)
// 	{
// 		glColor3f(
// 			model->colors[model->indices[i]].r,
// 			model->colors[model->indices[i]].g,
// 			model->colors[model->indices[i]].b);

// 		glVertex3f(
// 			model->vertices[model->indices[i]].x,
// 			model->vertices[model->indices[i]].y,
// 			model->vertices[model->indices[i]].z);
// 	}

// 	glEnd();

// 	glPopMatrix();

// 	// ------------------------------------------------------------------------
// 	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 	// glDrawArrays(GL_TRIANGLES, 0, 6);
// }

// void GEEntity::setTranslate(float x, float y, float z)
// {
// 	translate.x = x;
// 	translate.y = y;
// 	translate.z = z;
// }

// void GEEntity::setRotate(float angle, float x, float y, float z)
// {
// 	rotate.angle = angle;
// 	rotate.x = x;
// 	rotate.y = y;
// 	rotate.z = z;
// }

// void GEEntity::setScale(float x, float y, float z)
// {
// 	scale.x = x;
// 	scale.y = y;
// 	scale.z = z;
// }

// void GEEntity::setSpeed(float x, float y, float z)
// {
// 	speed.x = x;
// 	speed.y = y;
// 	speed.z = z;
// }

// void GEEntity::setBounding(double left, double right, double top, double bottom)
// {
// 	bounding.left = left;
// 	bounding.right = right;
// 	bounding.top = top;
// 	bounding.bottom = bottom;
// }

// // void GEEntity::loadToMemory()
// // {
// 	// GLuint vao;
// 	// GLuint bo[3] = {0};

// 	// glGenVertexArrays(1, &vao);
// 	// glBindVertexArray(vao);

// 	// if(model->vertices)
// 	// {
// 	// 	glGenBuffers(1, &bo[0]);
// 	// 	glBindBuffer(GL_ARRAY_BUFFER, bo[0]);
// 	// 	glBufferData(GL_ARRAY_BUFFER, model->total_vertex * sizeof(VERTEX), model->vertices, GL_STATIC_DRAW);

// 	// 	glEnableClientState(GL_VERTEX_ARRAY); // (remove)
// 	// 	glVertexPointer(3, GL_FLOAT, 0, 0); // (remove)
// 	// }

// 	// if(model->colors)
// 	// {
// 	// 	glGenBuffers(1, &bo[1]);
// 	// 	glBindBuffer(GL_ARRAY_BUFFER, bo[1]);
// 	// 	glBufferData(GL_ARRAY_BUFFER, model->total_vertex * sizeof(COLOR), model->colors, GL_STATIC_DRAW);

// 	// 	glEnableClientState(GL_COLOR_ARRAY); // (remove)
// 	// 	glColorPointer(3, GL_FLOAT, 0, 0); // (remove)
// 	// }

// 	// if(model->indices)
// 	// {
// 	// 	glGenBuffers(1, &bo[2]);
// 	// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo[2]);
// 	// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->total_indices * sizeof(unsigned int), model->indices, GL_STATIC_DRAW);
// 	// }
// // }

// // void GEEntity::releaseFromMemory()
// // {
// // }

// // ****************************************************************************
// //  Constructors and Destructors
// // ****************************************************************************
// GERenderingSystem::GERenderingSystem(KEWINAPIWrapper* apiWrapper)
// {
// 	this->apiWrapper = apiWrapper;
// 	// this->renderingContext = K_CONTEXT_2D;
// 	this->viewportWidth = 0;
// 	this->viewportHeight = 0;

// 	this->renderingWindow.left = 0.0f;
// 	this->renderingWindow.right = 0.0f;
// 	this->renderingWindow.top = 0.0f;
// 	this->renderingWindow.bottom = 0.0f;

// 	this->renderingWindowOffsetX = 0.0f;
// 	this->renderingWindowOffsetY = 0.0f;

// 	this->zoom = 0.0f;

// 	this->projectionZNear = 0.0;
// 	this->projectionZFar = 0.0;
// 	this->projectionFOVY = 0.0;
// 	this->windowAspectCorrection = 0.0;
// 	this->windowAspectCorrectionState = false;
// 	this->globalAxisState = false;
// }

// // ****************************************************************************
// //  Public Methods
// // ****************************************************************************
// int GERenderingSystem::initialize()
// {
// 	if(!apiWrapper)
// 	{
// 		std::cout << "(!) ERROR - It was not possible initialize rendering system: no apiwrapper.\n" << std::endl;
// 		return 0;
// 	}

// 	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
// 	// apontando para o objeto. Fazer essa validação!
// 	if(!apiWrapper->initializeRenderingSystem())
// 		return 0;

// 	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC) wglGetProcAddress("wglSwapIntervalEXT");

// 	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
// 	glIsBuffer = (PFNGLISBUFFERPROC)wglGetProcAddress("glIsBuffer");
// 	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
// 	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
// 	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
// 	glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
// 	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
// 	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
// 	glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)wglGetProcAddress("glFlushMappedBufferRange");
// 	glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)wglGetProcAddress("glCopyBufferSubData");
// 	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffers");
// 	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
// 	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
// 	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");

// 	if(
// 		!wglSwapIntervalEXT &&
// 		!glGenBuffers &&
// 		!glIsBuffer &&
// 		!glBindBuffer &&
// 		!glBufferData &&
// 		!glBufferSubData &&
// 		!glMapBuffer &&
// 		!glUnmapBuffer &&
// 		!glMapBufferRange &&
// 		!glFlushMappedBufferRange &&
// 		!glCopyBufferSubData &&
// 		!glDeleteBuffers &&
// 		!glGenVertexArrays &&
// 		!glBindVertexArray &&
// 		!glDeleteVertexArrays)
// 	{
// 		std::cout << "(!) ERROR - It was not possible to load GL extension: " << glGetError() << "\n" << std::endl;
// 		return 0;
// 	}

// 	return 1;
// }

// void GERenderingSystem::resetView()
// {
// 	glMatrixMode(GL_MODELVIEW);
// 	glLoadIdentity();
// }

// void GERenderingSystem::renderFrame()
// {
// 	// if(!apiWrapper)
// 	// {
// 	// 	std::cout << "(!) ERROR - It was not possible initialize rendering system: no apiwrapper.\n" << std::endl;
// 	// 	return;
// 	// }

// 	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
// 	// apontando para o objeto. Fazer essa validação!

// 	// UPDATE CAMERA
// 	// UPDATE SCENE ELEMENTS
// 	// RENDER SCENE
// 	// SWAP BUFFERS

// 	// this->apiWrapper->swapBuffers();
// }

// int GERenderingSystem::setVSync(int vsync)
// {
// 	return wglSwapIntervalEXT(vsync);
// }

// void GERenderingSystem::drawGlobaldAxis()
// {
// 	// glPushMatrix();
// 	// glMatrixMode(GL_MODELVIEW);
// 	// glLoadIdentity();

// 	// glBegin(GL_LINES);
// 	// glColor3f(1.0f, 0.0f, 0.0f);
// 	// glVertex3d(renderingWindow.left, 0.0, 0.0);
// 	// glVertex3d(renderingWindow.right, 0.0, 0.0);
// 	// glColor3f(0.0, 1.0f, 0.0f);
// 	// glVertex3d(0.0, renderingWindow.top, 0.0);
// 	// glVertex3d(0.0, renderingWindow.bottom, 0.0);
// 	// glEnd();

// 	// glPopMatrix();
// }

// void GERenderingSystem::getSystemVersion()
// {
// 	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
// }

// // ****************************************************************************
// //  Getters and Setters
// // ****************************************************************************
// int GERenderingSystem::getRenderingContext()
// {
// 	// return renderingContext;
// 	return 1;
// }

// GERECT GERenderingSystem::getRenderingWindow()
// {
// 	GERECT window;

// 	if(windowAspectCorrectionState)
// 	{
// 		if(viewportWidth <= viewportHeight)
// 		{
// 			windowAspectCorrection = static_cast<GLdouble>(viewportHeight) / static_cast<double>(viewportWidth);
// 			window.bottom *= windowAspectCorrection;
// 			window.top *= windowAspectCorrection;
// 		}
// 		else
// 		{
// 			windowAspectCorrection = static_cast<GLdouble>(viewportWidth) / static_cast<double>(viewportHeight);

// 			window.left *= windowAspectCorrection;
// 			window.right *= windowAspectCorrection;
// 		}
// 	}

// 	window.left += renderingWindowOffsetX + zoom;
// 	window.right += renderingWindowOffsetX - zoom;
// 	window.top += renderingWindowOffsetX - zoom;
// 	window.bottom += renderingWindowOffsetX + zoom;

// 	return window;
// }

// double GERenderingSystem::getProjectionZNear()
// {
// 	return projectionZNear;
// }

// double GERenderingSystem::getProjectionFOVY()
// {
// 	return projectionFOVY;
// }

// void GERenderingSystem::setWindowAspectCorrection(double windowAspectCorrectionParam)
// {
// 	this->windowAspectCorrection = windowAspectCorrectionParam;
// }

// double GERenderingSystem::getWindowAspectCorrection()
// {
// 	return windowAspectCorrection;
// }

// void GERenderingSystem::setWindowAspectCorrectionState(bool state)
// {
// 	this->windowAspectCorrectionState = state;
// }

// bool GERenderingSystem::getWindowAspectCorrectionState()
// {
// 	return windowAspectCorrectionState;
// }

// void GERenderingSystem::setGlobalAxisState(bool state)
// {
// 	globalAxisState = state;
// }