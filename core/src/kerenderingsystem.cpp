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
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLEXT/wglext.h>
#include <GLEXT/glext.h>

// ----------------------------------------------------------------------------
//  OpenGL Procedures Extension for Win32
// ----------------------------------------------------------------------------
PFNWGLSWAPINTERVALEXTPROC        wglSwapIntervalEXT = 0;
PFNGLGENBUFFERSPROC              glGenBuffers = 0;
PFNGLISBUFFERPROC                glIsBuffer = 0;
PFNGLBINDBUFFERPROC              glBindBuffer = 0;
PFNGLBUFFERDATAPROC              glBufferData = 0;
PFNGLBUFFERSUBDATAPROC           glBufferSubData = 0;
PFNGLMAPBUFFERPROC               glMapBuffer = 0;
PFNGLUNMAPBUFFERPROC             glUnmapBuffer = 0;
PFNGLMAPBUFFERRANGEPROC          glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC  glFlushMappedBufferRange = 0;
PFNGLCOPYBUFFERSUBDATAPROC       glCopyBufferSubData = 0;
PFNGLDELETEBUFFERSARBPROC        glDeleteBuffers = 0;
PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC         glBindVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC      glDeleteVertexArrays = 0;
PFNGLPRIMITIVERESTARTINDEXPROC   glPrimitiveRestartIndex = 0;
PFNGLCREATEBUFFERSPROC           glCreateBuffers = 0;
PFNGLCLEARBUFFERFVPROC           glClearBufferfv = 0;
PFNGLNAMEDBUFFERSTORAGEPROC      glNamedBufferStorage = 0;
PFNGLCREATEVERTEXARRAYSPROC      glCreateVertexArrays = 0;
PFNGLUSEPROGRAMPROC              glUseProgram = 0;
PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLCREATEPROGRAMPROC           glCreateProgram = 0;
PFNGLCREATESHADERPROC            glCreateShader = 0;
PFNGLSHADERSOURCEPROC            glShaderSource = 0;
PFNGLCOMPILESHADERPROC           glCompileShader = 0;
PFNGLGETSHADERIVPROC             glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog = 0;
PFNGLATTACHSHADERPROC            glAttachShader = 0;
PFNGLLINKPROGRAMPROC             glLinkProgram = 0;
PFNGLGETPROGRAMIVPROC            glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog = 0;
PFNGLMULTIDRAWELEMENTSPROC       glMultiDrawElements = 0;
PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv = 0;
PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation = 0;
PFNGLDRAWELEMENTSBASEVERTEXPROC  glDrawElementsBaseVertex = 0;
PFNGLDRAWARRAYSINSTANCEDPROC     glDrawArraysInstanced = 0;
PFNGLBUFFERSTORAGEPROC           glBufferStorage = 0;
PFNGLISVERTEXARRAYPROC           glIsVertexArray = 0;
PFNGLNAMEDBUFFERSUBDATAPROC      glNamedBufferSubData = 0;
PFNGLDELETEPROGRAMPROC           glDeleteProgram = 0;
PFNGLVERTEXATTRIBDIVISORPROC     glVertexAttribDivisor = 0;

void getProcedureAddress();

// ****************************************************************************
//  KERenderingSystem Constructors and Destructors
// ****************************************************************************
KERenderingSystem::KERenderingSystem(KEWINAPIWrapper* apiWrapperParam)
	: apiWrapper(apiWrapperParam)
{
}

int KERenderingSystem::startup()
{
	if(!apiWrapper)
	{
		std::cout << "(!) ERROR - It was not possible initialize rendering system: no apiwrapper.\n" << std::endl;
		return 0;
	}

	// (ATENÇÃO) É possível que neste ponto, apiWrapper não esteja mais
	// apontando para o objeto. Fazer essa validação!
	apiWrapper->initializeRenderingSystem();
	getProcedureAddress();

	/*
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

	// KEShader vertexShader("../shaders/", "main_vertex_shader.vert");
	KEShader vertexShader("../shaders/", "teste.vert");
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

	// KEShader fragmentShader("../shaders/", "main_fragment_shader.frag");
	KEShader fragmentShader("../shaders/", "teste.frag");
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
	programID = glCreateProgram();
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

	// render_model_matrix_loc = glGetUniformLocation(programID, "model_matrix");
	render_projection_matrix_loc = glGetUniformLocation(programID, "projection_matrix");

	// glPixelStorei (GL_UNPACK_ALIGNMENT, 4);

	// modelos de Shading (GL_FLAT ou GL_SMOOTH)
	// glShadeModel(GL_SMOOTH);
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_NORMALIZE);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/
	return 1;
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

void getProcedureAddress()
{
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
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
	glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)wglGetProcAddress("glMultiDrawElements");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)wglGetProcAddress("glDrawElementsBaseVertex");
	glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)wglGetProcAddress("glDrawArraysInstanced");
	glBufferStorage = (PFNGLBUFFERSTORAGEPROC)wglGetProcAddress("glBufferStorage");
	glIsVertexArray = (PFNGLISVERTEXARRAYPROC)wglGetProcAddress("glIsVertexArray");
	glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)wglGetProcAddress("glNamedBufferSubData");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");

	if (!wglSwapIntervalEXT &&
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
		!glGetProgramInfoLog &&
		!glMultiDrawElements &&
		!glUniformMatrix4fv &&
		!glGetUniformLocation &&
		!glDrawElementsBaseVertex &&
		!glDrawArraysInstanced &&
		!glBufferStorage &&
		!glIsVertexArray &&
		!glNamedBufferSubData &&
		!glDeleteProgram &&
		!glVertexAttribDivisor)
	{
		std::cout << "(!) ERROR - It was not possible to load GL extension: " << glGetError() << "\n" << std::endl;
	}
}