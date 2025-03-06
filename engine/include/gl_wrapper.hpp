/*
   K-Engine OpenGL Wrapper
   This file is part of the K-Engine.

   Copyright (C) 2020-2025 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_OPENGL_WRAPPER_HPP
#define K_ENGINE_OPENGL_WRAPPER_HPP

#include <k_math.hpp>
#include <mesh.hpp>

// #if defined() allows to use #elif
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/wglext.h>
#include <GL/glext.h>
#elif defined (__ANDROID__)
#include <EGL/egl.h>
#include <GLES3/gl32.h>
#elif defined (__linux__)
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glxext.h>
#endif

#include <string>
#include <unordered_map>

/*
	References:
		- https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
*/

extern PFNGLCLEARBUFFERFVPROC glClearBufferfv; // OpenGL 3.0
extern PFNGLCREATEBUFFERSPROC glCreateBuffers; // OpenGL 4.5
extern PFNGLGENBUFFERSPROC glGenBuffers; // OpenGL 2.0
extern PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage; // OpenGL 4.5
extern PFNGLBUFFERSTORAGEPROC glBufferStorage; // OpenGL 4.4
extern PFNGLNAMEDBUFFERDATAPROC glNamedBufferData; // OpenGL 4.5
extern PFNGLBUFFERDATAPROC glBufferData; // OpenGL 2.0
extern PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData; // OpenGL 4.5
extern PFNGLBUFFERSUBDATAPROC glBufferSubData; // OpenGL 2.0
extern PFNGLCLEARNAMEDBUFFERDATAPROC glClearNamedBufferData; // OpenGL 4.5
extern PFNGLCLEARBUFFERDATAPROC glClearBufferData; // OpenGL 4.3
extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC glCopyNamedBufferSubData; // OpenGL 4.5
extern PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData; // OpenGL 3.1
extern PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData; // OpenGL 4.5
extern PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData; // OpenGL 2.0
extern PFNGLMAPNAMEDBUFFERPROC glMapNamedBuffer; // OpenGL 4.5
extern PFNGLMAPBUFFERPROC glMapBuffer; // OpenGL 2.0
extern PFNGLUNMAPNAMEDBUFFERPROC glUnmapNamedBuffer; // OpenGL 4.5
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer; // OpenGL 2.0
extern PFNGLMAPNAMEDBUFFERRANGEPROC glMapNamedBufferRange; // OpenGL 4.5
extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange; // OpenGL 3.0
extern PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glFlushMappedNamedBufferRange; // OpenGL 4.5
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange; // OpenGL 3.0
extern PFNGLBINDBUFFERPROC glBindBuffer; // OpenGL 2.0
extern PFNGLISBUFFERPROC glIsBuffer; // OpenGL 2.0=
extern PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData; // OpenGL 4.3
extern PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData; // OpenGL 4.3
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers; // OpenGL 2.0
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays; // OpenGL 3.0
extern PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays; // OpenGL 4.5
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray; // OpenGL 3.0
extern PFNGLISVERTEXARRAYPROC glIsVertexArray; // OpenGL 3.0
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays; // OpenGL 3.0
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer; // OpenGL 2.0
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer; // OpenGL 3.0
extern PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer; // OpenGL 4.1
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray; // OpenGL 2.0
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray; // OpenGL 2.0
extern PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib; // OpenGL 4.5
extern PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib; // OpenGL 4.5
extern PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat; // OpenGL 4.5
extern PFNGLVERTEXARRAYVERTEXBUFFERSPROC glVertexArrayVertexBuffers; // OpenGL 4.5
extern PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer; // OpenGL 4.3
extern PFNGLVERTEXATTRIBFORMATPROC glVertexAtttribFormat; // OpenGL 4.3
extern PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding; // OpenGL 4.3
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation; // OpenGL 2.0
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance; // OpenGL 4.2
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv; // OpenGL 2.0
extern PFNGLGETSTRINGIPROC glGetStringi; // OpenGL 3.0
extern PFNGLCREATESHADERPROC glCreateShader; // OpenGL 2.0
extern PFNGLDELETESHADERPROC glDeleteShader; // OpenGL 2.0
extern PFNGLSHADERSOURCEPROC glShaderSource; // OpenGL 2.0
extern PFNGLCOMPILESHADERPROC glCompileShader; // OpenGL 2.0
extern PFNGLGETSHADERIVPROC glGetShaderiv; // OpenGL 2.0
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog; // OpenGL 2.0
extern PFNGLCREATEPROGRAMPROC glCreateProgram; // OpenGL 2.0
extern PFNGLDELETEPROGRAMPROC glDeleteProgram; // OpenGL 2.0
extern PFNGLATTACHSHADERPROC glAttachShader; // OpenGL 2.0
extern PFNGLDETACHSHADERPROC glDetachShader; // OpenGL 2.0
extern PFNGLLINKPROGRAMPROC glLinkProgram; // OpenGL 2.0
extern PFNGLGETPROGRAMIVPROC glGetProgramiv; // OpenGL 2.0
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog; // OpenGL 2.0
extern PFNGLUSEPROGRAMPROC glUseProgram; // OpenGL 2.0
extern PFNGLGETPROGRAMBINARYPROC glGetProgramBinary; // OpenGL 4.1
extern PFNGLPROGRAMBINARYPROC glProgramBinary; // OpenGL 4.1
extern PFNGLSHADERBINARYPROC glShaderBinary; // OpenGL 4.1
extern PFNGLSPECIALIZESHADERPROC glSpecializeShader; // OpenGL 4.1
extern PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv; //OpenGL 4.3
extern PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv; // OpenGL 4.3
extern PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName; // OpenGL 4.3
extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib; // OpenGL 2.0
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation; // OpenGL 2.0
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation; // OpenGL 2.0
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform; // OpenGL 2.0
extern PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName; // OpenGL 3.1
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex; // OpenGL 3.1
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv; // OpenGL 3.1
extern PFNGLGETUNIFORMINDICESPROC glGetUniformIndices; // OpenGL 3.1
extern PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv; // OpenGL 3.1
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase; // OpenGL 3.0
extern PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv; // OpenGL 4.1
extern PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines; // OpenGL 4.5
extern PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines; // OpenGL 4.5
extern PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages; // OpenGL 4.1
extern PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline; // OpenGL 4.1
extern PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f; // OpenGL 4.1
extern PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram; // OpenGL 4.1
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback; // OpenGL 4.3
extern PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl; // OpenGL 4.3
extern PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup; // OpenGL 4.3
extern PFNGLPOPDEBUGGROUPPROC glPopDebugGroup; // OpenGL 4.3
extern PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex; // OpenGL 3.1

namespace kengine {
	enum CONTEXT_FLAG {
#if defined(_WIN32)
		CONTEXT_DEBUG_BIT_ARB = WGL_CONTEXT_DEBUG_BIT_ARB,
		CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		CONTEXT_CORE_PROFILE_BIT_ABR = WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
#elif(__ANDROID__)
		CONTEXT_DEBUG_BIT_ARB = EGL_CONTEXT_OPENGL_DEBUG,
		CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE,
		CONTEXT_CORE_PROFILE_BIT_ABR = EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
		CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT
#elif(__linux__)
		CONTEXT_DEBUG_BIT_ARB = GLX_CONTEXT_DEBUG_BIT_ARB,
		CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		CONTEXT_CORE_PROFILE_BIT_ABR = GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
#endif
	};

	/*
		OpenGL compatibility profile

		- contextFlag:
			- CONTEXT_DEBUG_BIT_ARB
			- CONTEXT_FORWARD_COMPATIBLE_BIT_ARB

		- profileMask:
			- CONTEXT_CORE_PROFILE_BIT_ABR
			- CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB
	*/
	struct compatibility_profile {
		int contextFlag = 0x00;
		int profileMask = 0x00;
	};

	/*
		Get an OpenGL function address (it must be defined by OS API Wrapper)
	*/
	void* getGLFunctionAddress(std::string name);

	/*
		Get all OpenGL functions (!) must be refactor (!)
	*/
	bool getAllGLProcedures();

	/*
		This struct is used to pass a list of GLSL shaders that can be compiled.

		Values for GLuint type:

			- GL_VERTEX_SHADER
			- GL_GEOMETRY_SHADER
			- GL_TESS_EVALUATION_SHADER
			- GL_TESS_CONTROL_SHADER
			- GL_COMPUTE_SHADER
			- GL_FRAGMENT_SHADER
	*/
	struct ShaderInfo
	{
		GLuint type;
		std::string filename;
	};

	/*
		This class represents a GLSL program
	*/
	class GLSLprogram
	{
	public:
		GLSLprogram() {}
		~GLSLprogram();

		GLSLprogram(const GLSLprogram& copy) = delete; // copy constructor
		GLSLprogram(GLSLprogram&& move) = delete; // move constructor
		GLSLprogram& operator=(const GLSLprogram& copy) = delete; // copy assignment
		GLSLprogram& operator=(GLSLprogram&&) = delete; // move assigment

		bool loadShaders(const kengine::ShaderInfo* shaders);
		void useProgram();

		void saveBinary(const std::string& name);
		void loadBinary(const std::string& name);

		void setUniform(std::string name, bool transpose, GLfloat* value);

		/*
			(!) for perfomance: it doesn't use std::unordered_map and is inline
		*/
		void setUniformL(GLint location, bool transpose, GLfloat* value) {
			glUniformMatrix4fv(location, 1, transpose, value);
		}

		GLint getLocation(std::string name) {
			return uniformMap[name];
		}

		GLuint getProgramID() const { return programID; }

		void print() const;

		//void setUniform(std::string name, int value);
		//void setUniform(std::string name, const kengine::phong_light& l);
		//void setUniform(std::string name, const kengine::phong_material& m);
		//void setUniform(std::string name, const kengine::lighting_model& lm);
		//void setUniform(std::string name, const kengine::toon_shading_info& tsi); // deprecated
		//void setUniform(std::string name, const kengine::fog_info& fi);
		//void setUniform(std::string name, const kengine::pbr_light& l);
		//void setUniform(std::string name, const kengine::pbr_material& m);
		// the parameter size specifies the number of array elements. this should be 1 if the targeted uniform variable is not an array
		// void setUniform(std::string name, GLsizei size, float* data);
	private:
		GLuint programID = 0;
		GLenum binaryFormat = 0;
		std::unordered_map<std::string, GLint> uniformMap; // std::map vs std::unordered_map
	};

	/*
		This class encapsulate the vertex buffer object and vertex array object
	*/
	class mesh_node {
		static constexpr int MAX_VBO = 1;

	public:
		mesh_node() {}
		~mesh_node() { clear(); }

		mesh_node(const mesh_node& copy) = delete; // copy constructor
		mesh_node(mesh_node&& move) noexcept = delete; // move constructor
		mesh_node& operator=(const mesh_node& copy) = delete; // copy assignment
		mesh_node& operator=(mesh_node&&) = delete; // move assigment

		/*
			Create new buffer objects for the mesh m. This method will destroy all previous loaded objects.
		*/
		void load(mesh& m, size_t size = 1); // no DSA commands
		void clear();
		void drawArrays() const;
		void setMode(GLenum mode) { m_mode = mode; }

	private:
		GLuint m_vbo[MAX_VBO] = { 0 };
		GLuint m_vao = 0;
		GLsizei m_count = 0;
		GLenum m_mode = GL_TRIANGLES;

		//GLsizei countElement = 0;
		//size_t max_size = 1;
		//int hasModelMatrix = 1;
	};

	/*
		Helper function to compile GLSL shader
	*/
	GLuint compileShader(GLuint shaderType, std::string filename);

	/*
		Helper function to compile SPIR-V shader
	*/
	GLuint compileSPIRVShader(GLuint shaderType, const std::string& name);

	std::string getShaderType(GLuint shader_type);
	std::string getGLSLType(GLint type);
	std::string readFromFile(std::string filename);

	/*
		OpenGL callback function for debug messages (https://www.khronos.org/opengl/wiki/Debug_Output)
		Note: Use breakpoint on OpenGL callback function "openGLDebugMessageCallback" with callstack actived for debbuging.
	*/
	void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param);
}

#endif