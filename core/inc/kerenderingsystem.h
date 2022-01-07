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

#ifndef K_ENGINE_RENDERING_SYSTEM_H
#define K_ENGINE_RENDERING_SYSTEM_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <GLEXT/wglext.h>
#include <GLEXT/glext.h>

#include <kemodel.h>

class KEWINAPIWrapper;

// ----------------------------------------------------------------------------
//  OpenGL Procedures Extension for Win32
// ----------------------------------------------------------------------------
extern PFNWGLSWAPINTERVALEXTPROC        wglSwapIntervalEXT;
extern PFNGLGENBUFFERSPROC              glGenBuffers;
extern PFNGLISBUFFERPROC                glIsBuffer;
extern PFNGLBINDBUFFERPROC              glBindBuffer;
extern PFNGLBUFFERDATAPROC              glBufferData;
extern PFNGLBUFFERSUBDATAPROC           glBufferSubData;
extern PFNGLMAPBUFFERPROC               glMapBuffer;
extern PFNGLUNMAPBUFFERPROC             glUnmapBuffer;
extern PFNGLMAPBUFFERRANGEPROC          glMapBufferRange;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC  glFlushMappedBufferRange;
extern PFNGLCOPYBUFFERSUBDATAPROC       glCopyBufferSubData;
extern PFNGLDELETEBUFFERSARBPROC        glDeleteBuffers;
extern PFNGLGENVERTEXARRAYSPROC         glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC         glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC      glDeleteVertexArrays;
extern PFNGLPRIMITIVERESTARTINDEXPROC   glPrimitiveRestartIndex;
extern PFNGLCREATEBUFFERSPROC           glCreateBuffers;
extern PFNGLCLEARBUFFERFVPROC           glClearBufferfv;
extern PFNGLNAMEDBUFFERSTORAGEPROC      glNamedBufferStorage;
extern PFNGLCREATEVERTEXARRAYSPROC      glCreateVertexArrays;
extern PFNGLUSEPROGRAMPROC              glUseProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC     glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLCREATEPROGRAMPROC           glCreateProgram;
extern PFNGLCREATESHADERPROC            glCreateShader;
extern PFNGLSHADERSOURCEPROC            glShaderSource;
extern PFNGLCOMPILESHADERPROC           glCompileShader;
extern PFNGLGETSHADERIVPROC             glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC        glGetShaderInfoLog;
extern PFNGLATTACHSHADERPROC            glAttachShader;
extern PFNGLLINKPROGRAMPROC             glLinkProgram;
extern PFNGLGETPROGRAMIVPROC            glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC       glGetProgramInfoLog;
extern PFNGLMULTIDRAWELEMENTSPROC       glMultiDrawElements;
extern PFNGLUNIFORMMATRIX4FVPROC        glUniformMatrix4fv;
extern PFNGLGETUNIFORMLOCATIONPROC      glGetUniformLocation;
extern PFNGLDRAWELEMENTSBASEVERTEXPROC  glDrawElementsBaseVertex;
extern PFNGLDRAWARRAYSINSTANCEDPROC     glDrawArraysInstanced;
extern PFNGLBUFFERSTORAGEPROC           glBufferStorage;
extern PFNGLISVERTEXARRAYPROC           glIsVertexArray;
extern PFNGLNAMEDBUFFERSUBDATAPROC      glNamedBufferSubData;
extern PFNGLDELETEPROGRAMPROC           glDeleteProgram;
extern PFNGLVERTEXATTRIBDIVISORPROC     glVertexAttribDivisor;

//typedef struct {
//	double left;
//	double right;
//	double top;
//	double bottom;
//} KERECT;

namespace kengine
{
	class ModelManager;

	// ------------------------------------------------------------------------
	//  K-Engine Model Node class
	// ------------------------------------------------------------------------
	class modelnode
	{
		friend class modelmanager;

	public:
		explicit modelnode(const kengine::model &m);
		~modelnode();

		void draw() const;

	private:
		GLuint vao;
		GLuint vbo;

		kengine::model data;
		modelnode *next;
	};

	// ------------------------------------------------------------------------
	//  K-Engine Model Node Manager class
	// ------------------------------------------------------------------------
	class modelmanager
	{
	public:
		modelmanager();
		~modelmanager();

		void add(const kengine::model &m);
		bool isEmpty() const;

		void drawModels() const;

	private:
		modelnode *first;
		modelnode *last;
	};
}

// ****************************************************************************
//  K-Engine Rendering System Class
// ****************************************************************************
class KERenderingSystem
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructors
	// ------------------------------------------------------------------------
	explicit KERenderingSystem(KEWINAPIWrapper* apiWrapperParam);

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	//void setViewport(int x, int y, int width, int height);
	//void setProjection();
	//void setBackgroundColor(float red, float green, float blue, float alpha);

	int startup();
	int setVSync(int vsync);
	void getGLVersion() const;

	//void setLightModel(int isLightEnable);

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	//int getRenderingContext() const;
	//void setRenderingContext(int renderingContextParam);

	//void setRenderingWindow(double left, double right, double bottom, double top);
	//KERECT getRenderingWindow() const;
	
	//void setWindowAspectCorrectionState(bool state);
	
	//void setProjectionFOVY(double fovyParam);
	//double getProjectionFOVY() const;

	//void setProjectionZNear(double projectionZNearParam);
	//double getProjectionZNear() const;
	//
	//void setProjectionZFar(double projectionZFarParam);
	//double getProjectionZFar() const;
	//
	//void setZoom(double zoomParam);
	//double getZoom() const;
	//
	//void setRenderingWindowOffsetX(double offset);
	//double getRenderingWindowOffsetX() const;
	//
	//void setRenderingWindowOffsetY(double offset);
	//double getRenderingWindowOffsetY() const;

	//void setLightModelAmbient(float red, float green, float blue, float alpha);

private:
	KEWINAPIWrapper* apiWrapper;

	//int renderingContext;
	//int viewportWidth;
	//int viewportHeight;
	//KERECT renderingWindow;
	//bool windowAspectCorrectionState;
	//double projectionFOVY;
	//double projectionZNear;
	//double projectionZFar;
	//double zoom;
	//double renderingWindowOffsetX;
	//double renderingWindowOffsetY;
	//float lightModelAmbient[4];
};

#endif