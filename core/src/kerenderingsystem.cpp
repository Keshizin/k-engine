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
#include <kemodel.h>
#include <keimage.h>
#include <kelight.h>
#include <keshader.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <GLEXT/wglext.h>
#include <GLEXT/glext.h>

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
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex = 0;
PFNGLCREATEBUFFERSPROC glCreateBuffers = 0;
PFNGLCLEARBUFFERFVPROC glClearBufferfv = 0;
PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage = 0;
PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;

// ****************************************************************************
//  drawModel - Function to draw model 2D/3D
// ****************************************************************************
void drawModel(const KEModel &model, int mode)
{
	// int c = 0;

	for(int face = 0; face < model.faces.size(); face++)
	{
		if(mode == 1)
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_POLYGON);

		for(int vertex = 0; vertex < model.faces[face].vertex_index.size(); vertex++)
		{
			// if(model.faces[face].vertex_normal_index.size())
			// {
			// 	if(!model.faces[face].material_name.empty())
			// 	{
			// 		glDisable(GL_COLOR_MATERIAL);

			// 		int index = model.getMaterial(model.faces[face].material_name);

			// 		if(index != -1)
			// 		{
			// 			glMaterialfv(GL_FRONT, GL_AMBIENT,   model.materials[index].Ka);
			// 			glMaterialfv(GL_FRONT, GL_DIFFUSE,   model.materials[index].Kd);
			// 			glMaterialfv(GL_FRONT, GL_SPECULAR,  model.materials[index].Ks);
			// 			glMaterialfv(GL_FRONT, GL_EMISSION,  model.materials[index].Ke);
			// 			glMaterialf(GL_FRONT,  GL_SHININESS, model.materials[index].Ns);
			// 		}
			// 	}

			// 	glNormal3f(
			// 		model.vertexNormals[ model.faces[face].vertex_normal_index[vertex] - 1 ].i,
			// 		model.vertexNormals[ model.faces[face].vertex_normal_index[vertex] - 1 ].j,
			// 		model.vertexNormals[ model.faces[face].vertex_normal_index[vertex] - 1 ].k);
			// }

			glVertex4d(
				model.geometricVertices[ model.faces[face].vertex_index[vertex] - 1 ].x,
				model.geometricVertices[ model.faces[face].vertex_index[vertex] - 1 ].y,
				model.geometricVertices[ model.faces[face].vertex_index[vertex] - 1 ].z,
				model.geometricVertices[ model.faces[face].vertex_index[vertex] - 1 ].w);
		}

		glEnd();
	}

	// std::cout << "> c: " << c << std::endl; 
}

void setVertexArray(KEModel &model)
{
	model.setArrayVertex();

	// GLuint vao;

	// glGenVertexArrays(1, &vao);

	// glBindVertexArray(vao);

	GLuint name = 0;
	glGenBuffers(1, &name);

	std::cout << "@debug | name: " << name << std::endl;

	if(glIsBuffer(name))
	{

		std::cout << "@debug | 1" << std::endl;
	}
	else
	{
		std::cout << "@debug | 2" << std::endl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, name);

	glBufferData(GL_ARRAY_BUFFER, model.totalOfVertices * sizeof(double), model.vertexArray, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	// glVertexPointer(3, GL_DOUBLE, 0, model.vertexArray);
}

void drawModel2(const KEModel &model, int mode)
{
	// GLenum type;

	// if(mode == 1)
	// 	type = GL_LINE_LOOP;
	// else
	// 	// type = GL_QUADS;
	// 	type = GL_TRIANGLES;

	glDrawArrays(GL_TRIANGLES, 0, model.totalOfVertices);

	// for(int face = 0; face < model.faces.size(); face++)
	// {
	// 	glBegin(type);
		
	// 	for(int vertex = 0; vertex < model.faces[face].vertex_index.size(); vertex++)
	// 	{
	// 		glArrayElement((model.faces[face].vertex_index[vertex] - 1));
	// 	}

	// 	glEnd();
	// }
}

GLuint vao = 0;

void draw()
{
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f }; 
	glClearBufferfv(GL_COLOR, 0, black);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void set(const KEModel &model)
{
	// GLuint vbo;

	// creating VAOs
	// GLuint vao;
	// glCreateVertexArrays(1, &vao);

	// creating VBOs
	// glCreateBuffers(1, &vbo);
	// glNamedBufferStorage(vbo, model.totalOfVertices, model.vertexArray, 0);

	// Setting up Shaders
	// ShaderInfo shaders[] = {
	// 	{GL_VERTEX_SHADER, "triangles.vert"},
	// 	{GL_FRAGMENT_SHADER, "triangles.frag"},
	// 	{GL_NONE, NULL}
	// };

	// GLuint program = LoadShaders(shaders);
	// glUseProgram(program);

	// glBindVertexArray(vao);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
	// glEnableVertexAttribArray(0);

	static const GLfloat vertices[6][2] = 
	{ 
		{ -0.90, -0.90 }, // Triangle 1 
		{0.85, -0.90 }, 
		{ -0.90,0.85 }, 
		{0.90, -0.85 }, // Triangle 2 
		{0.90,0.90 }, 
		{ -0.85,0.90 } 
	};

	GLuint vbo;
	glCreateBuffers(1, &vbo); 
	glNamedBufferStorage(vbo, sizeof(vertices), vertices, 0); 

	glGenVertexArrays(1, &vao); 
	glBindVertexArray(vao); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(0);
}

// ****************************************************************************
//  drawImage - Function to draw bitmap images
// ****************************************************************************
void drawImage(int posX, int posY, const DIB &image)
{
	glRasterPos2i(posX, posY);

	switch(image.getBiBitCount())
	{
		case 1:
			glBitmap(
				image.getWidth(),
				image.getHeight(),
				posX,
				posY,
				image.getWidth(),
				image.getHeight(),
				image.getColorIndex());
			
			break;

		case 24:
			glDrawPixels(
				image.getWidth(),
				image.getHeight(),
				GL_RGB,
				GL_UNSIGNED_BYTE,
				image.getColorIndex());
			
			break;
	}
}

// ****************************************************************************
//  setLighting - Function to active lighting
// ****************************************************************************
void setLight(const KELight &light, int isLightEnable, int lightParam)
{
	if(isLightEnable)
	{
		glEnable(static_cast<GLenum>(lightParam));
		glLightfv(lightParam, GL_AMBIENT,  light.ambient);
		glLightfv(lightParam, GL_DIFFUSE,  light.diffuse);
		glLightfv(lightParam, GL_SPECULAR, light.specular);
		glLightfv(lightParam, GL_POSITION, light.position);
	}
	else
		glDisable(static_cast<GLenum>(lightParam));
}

// ****************************************************************************
//  KERenderingSystem Constructors and Destructors
// ****************************************************************************
KERenderingSystem::KERenderingSystem(KEWINAPIWrapper* apiWrapperParam) :
	apiWrapper(apiWrapperParam),
	renderingContext(K_CONTEXT_2D),
	viewportWidth(0),
	viewportHeight(0),
	windowAspectCorrectionState(true),
	projectionFOVY(0.0),
	projectionZNear(0.0),
	projectionZFar(0.0),
	zoom(0.0),
	renderingWindowOffsetX(0.0),
	renderingWindowOffsetY(0.0),
	lightModelAmbient()
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
				double windowAspectCorrection = static_cast<double>(viewportHeight) / static_cast<double>(viewportWidth);
				window.bottom *= windowAspectCorrection;
				window.top *= windowAspectCorrection;
			}
			else
			{
				double windowAspectCorrection = static_cast<double>(viewportWidth) / static_cast<double>(viewportHeight);
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
		double windowAspectCorrection = static_cast<double>(viewportWidth) / static_cast<double>(viewportHeight);
		gluPerspective(projectionFOVY, windowAspectCorrection, projectionZNear, projectionZFar);
		// o processo de PAN deve ser realizado através do observador
	}
	else if(renderingContext == K_CONTEXT_3D_ORTHOGRAPHIC)
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
				double windowAspectCorrection = static_cast<double>(viewportHeight) / static_cast<double>(viewportWidth);
				window.bottom *= windowAspectCorrection;
				window.top *= windowAspectCorrection;
			}
			else
			{
				double windowAspectCorrection = static_cast<double>(viewportWidth) / static_cast<double>(viewportHeight);
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

void KERenderingSystem::setBackgroundColor(float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha);
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
	glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)wglGetProcAddress("glPrimitiveRestartIndex");
	glCreateBuffers = (PFNGLCREATEBUFFERSPROC)wglGetProcAddress("glCreateBuffers");
	glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)wglGetProcAddress("glClearBufferfv");
	glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)wglGetProcAddress("glNamedBufferStorage");
	glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)wglGetProcAddress("glCreateVertexArrays");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");

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
		!glDeleteVertexArrays &&
		!glPrimitiveRestartIndex &&
		!glCreateBuffers &&
		!glClearBufferfv &&
		!glNamedBufferStorage &&
		!glCreateVertexArrays &&
		!glUseProgram &&
		!glVertexAttribPointer &&
		!glEnableVertexAttribArray &&
		!glCreateProgram &&
		!glCreateShader &&
		!glShaderSource &&
		!glCompileShader &&
		!glGetShaderiv &&
		!glGetShaderInfoLog &&
		!glAttachShader &&
		!glLinkProgram &&
		!glGetProgramiv &&
		!glGetProgramInfoLog)
	{
		std::cout << "(!) ERROR - It was not possible to load GL extension: " << glGetError() << "\n" << std::endl;
		return 0;
	}

	// (!) Enabling VERTEX SHADER and FRAGMENT SHADER here!
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

	if(!vertexShaderID)
	{
		std::cout << "(!) ERROR - It was not possible to create a vertex shader!\n" << std::endl;
	}

	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	if(!fragmentShaderID)
	{
		std::cout << "(!) ERROR - It was not possible to create a fragment shader!\n" << std::endl;
	}

	KEShader vertexShader("../shaders/", "main_vertex_shader.vert");
	const GLchar * vertexSource = vertexShader.getSource();

	glShaderSource(vertexShaderID, 1, &vertexSource, NULL);
	glCompileShader(vertexShaderID);
	GLint param = 0;

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &param);

	if(!param)
	{
		char buffer[500] = {0};

		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &param);
		glGetShaderInfoLog(vertexShaderID, 500, &param, buffer);

		std::cout << "(!) ERROR - vertex shader compilation:\n" << buffer << std::endl;
	}

	KEShader fragmentShader("../shaders/", "main_fragment_shader.frag");
	const GLchar * fragmentSource = fragmentShader.getSource();

	glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShaderID);
	param = 0;

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &param);

	if(!param)
	{
		char buffer[500] = {0};

		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &param);
		glGetShaderInfoLog(fragmentShaderID, 500, &param, buffer);

		std::cout << "(!) ERROR - fragment shader compilation:\n" << buffer << std::endl;
	}

	// Creating the Main Shader
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	param = 0;

	glGetProgramiv(programID, GL_LINK_STATUS, &param);

	if(!param)
	{
		char buffer[500] = {0};

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &param);
		glGetProgramInfoLog(programID, 500, &param, buffer);

		std::cout << "(!) ERROR - fragment shader compilation:\n" << buffer << std::endl;
	}

	glUseProgram(programID);

	// glPixelStorei (GL_UNPACK_ALIGNMENT, 4);

	// modelos de Shading (GL_FLAT ou GL_SMOOTH)
	// glShadeModel(GL_SMOOTH);
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_NORMALIZE);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return 1;
}

void KERenderingSystem::setLightModel(int isLightEnable)
{
	if(isLightEnable)
	{
		glEnable(GL_LIGHTING);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
	}
	else
		glDisable(GL_LIGHTING);
}

int KERenderingSystem::setVSync(int vsync)
{
	return wglSwapIntervalEXT(vsync);
}

void KERenderingSystem::getGLVersion() const
{
	std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
}

// ****************************************************************************
//  KERenderingSystem - Getters and Setters
// ****************************************************************************
void KERenderingSystem::setRenderingContext(int renderingContextParam)
{
	renderingContext = renderingContextParam;
}

int KERenderingSystem::getRenderingContext() const
{
	return renderingContext;
}

void KERenderingSystem::setRenderingWindow(double left, double right, double bottom, double top)
{
	renderingWindow.left   = left;
	renderingWindow.right  = right;
	renderingWindow.bottom = bottom;
	renderingWindow.top    = top;
}

KERECT KERenderingSystem::getRenderingWindow() const
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
			double windowAspectCorrection = static_cast<GLdouble>(viewportHeight) / static_cast<double>(viewportWidth);
			window.bottom *= windowAspectCorrection;
			window.top *= windowAspectCorrection;
		}
		else
		{
			double windowAspectCorrection = static_cast<GLdouble>(viewportWidth) / static_cast<double>(viewportHeight);
			window.left *= windowAspectCorrection;
			window.right *= windowAspectCorrection;
		}
	}

	window.left   += renderingWindowOffsetX + zoom;
	window.right  += renderingWindowOffsetX - zoom;
	window.top    += renderingWindowOffsetX - zoom;
	window.bottom += renderingWindowOffsetX + zoom;
	return window;
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

double KERenderingSystem::getProjectionZNear() const
{
	return projectionZNear;
}

void KERenderingSystem::setProjectionZFar(double projectionZFarParam)
{
	projectionZFar = projectionZFarParam;
}

double KERenderingSystem::getProjectionZFar() const
{
	return projectionZFar;
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

void KERenderingSystem::setLightModelAmbient(float red, float green, float blue, float alpha)
{
	lightModelAmbient[0] = red;
	lightModelAmbient[1] = green;
	lightModelAmbient[2] = blue;
	lightModelAmbient[3] = alpha;
}