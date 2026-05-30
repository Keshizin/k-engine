/*
   K-Engine OpenGL Wrapper
   This file is part of the K-Engine.

   Copyright (C) 2020-2026 Fabio Takeshi Ishikawa

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

#include <camera.hpp>

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
#include <memory>

#define BUFFER_OFFSET(offset) (const GLvoid*)offset

/*
	References:
		- https://www.khronos.org/opengl/wiki/Load_OpenGL_Functions
*/

/*
	OpenGL extension: GLSL shaders
*/

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
extern PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib; // OpenGL 2.0
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation; // OpenGL 2.0
extern PFNGLUSEPROGRAMPROC glUseProgram; // OpenGL 2.0
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation; // OpenGL 2.0
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform; // OpenGL 2.0
extern PFNGLUNIFORM1FPROC glUniform1f; // OpenGL 2.0
extern PFNGLUNIFORM3FPROC glUniform3f; // OpenGL 2.0
extern PFNGLUNIFORM4FPROC glUniform4f; // OpenGL 2.0
extern PFNGLUNIFORM4FVPROC glUniform4fv; // OpenGL 2.0
extern PFNGLUNIFORM1IPROC glUniform1i; // OpenGL 2.0
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv; // OpenGL 2.0
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase; // OpenGL 3.0
extern PFNGLPROGRAMPARAMETERIPROC glProgramParameteri; // OpenGL 3.0
extern PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName; // OpenGL 3.1
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex; // OpenGL 3.1
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv; // OpenGL 3.1
extern PFNGLGETUNIFORMINDICESPROC glGetUniformIndices; // OpenGL 3.1
extern PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv; // OpenGL 3.1
extern PFNGLGETPROGRAMBINARYPROC glGetProgramBinary; // OpenGL 4.1
extern PFNGLPROGRAMBINARYPROC glProgramBinary; // OpenGL 4.1
extern PFNGLSHADERBINARYPROC glShaderBinary; // OpenGL 4.1
extern PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv; // OpenGL 4.1
extern PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages; // OpenGL 4.1
extern PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline; // OpenGL 4.1
extern PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f; // OpenGL 4.1
extern PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram; // OpenGL 4.1
extern PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv; //OpenGL 4.3
extern PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv; // OpenGL 4.3
extern PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName; // OpenGL 4.3
extern PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines; // OpenGL 4.5
extern PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines; // OpenGL 4.5
extern PFNGLSPECIALIZESHADERPROC glSpecializeShader; // OpenGL 4.6

/*
	OpenGL extension: buffers objects
*/

extern PFNGLGENBUFFERSPROC glGenBuffers; // OpenGL 2.0
extern PFNGLISBUFFERPROC glIsBuffer; // OpenGL 2.0
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers; // OpenGL 2.0
extern PFNGLBINDBUFFERPROC glBindBuffer; // OpenGL 2.0
extern PFNGLBUFFERDATAPROC glBufferData; // OpenGL 2.0
extern PFNGLBUFFERSUBDATAPROC glBufferSubData; // OpenGL 2.0
extern PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData; // OpenGL 2.0
extern PFNGLMAPBUFFERPROC glMapBuffer; // OpenGL 2.0
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer; // OpenGL 2.0
extern PFNGLMAPBUFFERRANGEPROC glMapBufferRange; // OpenGL 3.0
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange; // OpenGL 3.0
extern PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData; // OpenGL 3.1
extern PFNGLCLEARBUFFERDATAPROC glClearBufferData; // OpenGL 4.3
extern PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData; // OpenGL 4.3
extern PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData; // OpenGL 4.3
extern PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData; // OpenGL 4.3
extern PFNGLBUFFERSTORAGEPROC glBufferStorage; // OpenGL 4.4
extern PFNGLCREATEBUFFERSPROC glCreateBuffers; // OpenGL 4.5
extern PFNGLNAMEDBUFFERDATAPROC glNamedBufferData; // OpenGL 4.5
extern PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage; // OpenGL 4.5
extern PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData; // OpenGL 4.5
extern PFNGLCLEARNAMEDBUFFERDATAPROC glClearNamedBufferData; // OpenGL 4.5
extern PFNGLCLEARNAMEDBUFFERSUBDATAPROC glClearNamedBufferSubData; // OpenGL 4.5
extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC glCopyNamedBufferSubData; // OpenGL 4.5
extern PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData; // OpenGL 4.5
extern PFNGLMAPNAMEDBUFFERPROC glMapNamedBuffer; // OpenGL 4.5
extern PFNGLUNMAPNAMEDBUFFERPROC glUnmapNamedBuffer; // OpenGL 4.5
extern PFNGLMAPNAMEDBUFFERRANGEPROC glMapNamedBufferRange; // OpenGL 4.5
extern PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glFlushMappedNamedBufferRange; // OpenGL 4.5

/*
	OpenGL extension: Vertex Array Objects
*/

extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray; // OpenGL 2.0
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray; // OpenGL 2.0
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer; // OpenGL 2.0
extern PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation; // OpenGL 2.0
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays; // OpenGL 3.0
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays; // OpenGL 3.0
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray; // OpenGL 3.0
extern PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation; // OpenGL 3.0
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer; // OpenGL 3.0
extern PFNGLPROVOKINGVERTEXPROC glProvokingVertex; // OpenGL 3.2
extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor; // OpenGL 3.3
extern PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer; // OpenGL 4.1
extern PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer; // OpenGL 4.3
extern PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat; // OpenGL 4.3
extern PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding; // OpenGL 4.3
extern PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays; // OpenGL 4.5

/*
	OpenGL extensions: OpenGL drawing comands
*/
extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced; // OpenGL 3.1
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced; // OpenGL 3.1

/*
	OpenGL extension: texture objects
*/
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap; // OpenGL 3.0
extern PFNGLCREATETEXTURESPROC glCreateTextures; // OpenGL 4.5

/*
	OpenGL extension: OpenGL debug messages
*/
extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback; // OpenGL 4.3
extern PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl; // OpenGL 4.3
extern PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup; // OpenGL 4.3
extern PFNGLPOPDEBUGGROUPPROC glPopDebugGroup; // OpenGL 4.3

/*
	OpenGL extension: transform feedback objects
*/
extern PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks; // OpenGL 4.0
extern PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback; // OpenGL 4.0
extern PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback; // OpenGL 4.0
extern PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks; // OpenGL 4.0
extern PFNGLCREATETRANSFORMFEEDBACKSPROC glCreateTransformFeedbacks; // OpenGL 4.5
extern PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glTransformFeedbackBufferBase; // OpenGL 4.5
extern PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glTransformFeedbackBufferRange; // OpenGL 4.5

/*
*/
extern PFNGLDEPTHRANGEFPROC glDepthRangef;
extern PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange; // OpenGL 4.4

//
extern PFNGLCLEARBUFFERFVPROC glClearBufferfv; // OpenGL 3.0
extern PFNGLISVERTEXARRAYPROC glIsVertexArray; // OpenGL 3.0
extern PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib; // OpenGL 4.5
extern PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib; // OpenGL 4.5
extern PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat; // OpenGL 4.5
extern PFNGLVERTEXARRAYVERTEXBUFFERSPROC glVertexArrayVertexBuffers; // OpenGL 4.5
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance; // OpenGL 4.2
extern PFNGLGETSTRINGIPROC glGetStringi; // OpenGL 3.0
extern PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex; // OpenGL 3.1
extern PFNGLTEXSTORAGE1DPROC glTexStorage1D; // OpenGL 4.2
extern PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D; // OpenGL 4.5
extern PFNGLTEXSTORAGE2DPROC glTexStorage2D; // OpenGL 4.2
extern PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D; // OpenGL 4.5
extern PFNGLTEXSTORAGE3DPROC glTexStorage3D; // OpenGL 4.2
extern PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D; // OpenGL 4.5
extern PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D; // OpenGL 4.5
extern PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D; // OpenGL 4.5
//extern PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D; // OpenGL 2.0+
extern PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D; // OpenGL 4.5+
extern PFNGLBINDTEXTUREUNITPROC glBindTextureUnit; // OpenGL 4.5
extern PFNGLTEXBUFFERPROC glTexBuffer; // OpenGL 3.1+
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding; // OpenGL 3.1+

#if defined(_WIN32)
	extern PFNGLCLIENTACTIVETEXTUREPROC glActiveTexture; // OpenGL 2.0+
#endif

namespace kengine {
	enum CONTEXT_FLAG {
#if defined(_WIN32)
		CONTEXT_DEBUG_BIT_ARB = WGL_CONTEXT_DEBUG_BIT_ARB,
		CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		CONTEXT_CORE_PROFILE_BIT_ARB = WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
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
				- CONTEXT_CORE_PROFILE_BIT_ABR (old features removed)
				- CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB (maintain compatibility with older functionality)
	*/
	struct compatibility_profile {
		unsigned int contextFlag = 0x00;
		unsigned int profileMask = 0x00;
	};

	/*
		Get an OpenGL function address (it must be defined by OS API Wrapper)
	*/
	void* getGLFunctionAddress(const std::string& name);

	/*
		Get all OpenGL functions
	*/
	bool getGLProcedures();

	/*
		Get OpenGL info
	*/
	std::string getGLInfo(bool extension);

	/*
		This function reads a text file and return in the std::string form
	*/
	std::string readFromFile(const std::string& filename);

	/*
		Get the shader type as string
	*/
	std::string getShaderType(GLuint shaderType);

	/*
		Get the GLSL type as string
	*/
	std::string getGLSLType(GLint type);

	/*
		Get the OpenGL component data type size (i.e. data size of GLfloat, GLint, etc)
	*/
	size_t getGLTypeSize(GLenum type);

	/*
		Get the number of components of the OpenGL data type (i.e. data type size of vec3, mat4, etc)
	*/
	size_t getGLTypeComponents(const std::string& type);

	/*
		This function compile a GLSL shader
	*/
	GLuint compileShader(GLuint shaderType, const std::string& filename);

	/*
		Helper function to compile SPIR-V shader
	*/
	GLuint compileSPIRVShader(GLuint shaderType, const std::string& name);

	/*
		This struct is used to pass a list of GLSL shaders that can be compiled.

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

		bool loadShaders(const ShaderInfo* shaders);
		void useProgram();
		void clear();

		/*
			shader program binary is supported only in 4.1 version (and Apple has deprecated OpenGL)
		*/
		void saveBinary(const std::string& filename);

		/*
			shader program binary is supported only in 4.1 version (and Apple has deprecated OpenGL)
		*/
		void loadBinary(const std::string& filename);

		void setUniform(const std::string& name, bool transpose, GLfloat* value) const;
		void setUniform(const std::string& name, GLsizei count, bool transpose, void* data);
		void setUniform(const std::string& name, GLfloat value) const;
		void setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const;
		void setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const;
		void setUniform(const std::string& name, int value) const;

		GLuint getProgram() const { return m_programHandle; }
		void print() const;

	private:
		GLuint m_programHandle = 0;
		std::unordered_map<std::string, GLint> m_uniformMap;
	};

	/*
		Uniform Block Object's uniform variable
	*/
	struct ubo_var {
		std::string name;
		std::vector<GLfloat> data;
	};

	/*
		This class encapsulate the uniform buffer object (UBO)
	*/
	class uniform_buffer_object {
	public:
		uniform_buffer_object() {}
		~uniform_buffer_object() { clear(); }

		uniform_buffer_object(const uniform_buffer_object& copy) = delete; // copy constructor
		uniform_buffer_object(uniform_buffer_object&& move) noexcept = delete; // move constructor
		uniform_buffer_object& operator=(const uniform_buffer_object& copy) = delete; // copy assignment
		uniform_buffer_object& operator=(uniform_buffer_object&&) = delete; // move assigment

		void load(GLuint program, const std::string& name, const std::vector<ubo_var>& uboVars);
		void update();
		void clear();

	private:
		GLuint m_ubo = 0;
	};

#define VATTRIB_CAST(x) reinterpret_cast<unsigned char*>(x)

	/*
		Class to store an vertex attribute and data information
	*/
	class vattrib
	{
	public:
		// default constructor
		//vattrib() {}

		// destructor
		~vattrib() { clear(); }

		// constructor with arguments
		vattrib(const unsigned char* array, size_t arraySize, size_t c, size_t type, GLenum target);

		// copy constructor
		vattrib(const vattrib& va);

		// copy assignment = operator
		vattrib& operator=(const vattrib& va);

		// move constructor
		vattrib(vattrib&& move) noexcept;

		// move assignment = operator
		vattrib& operator=(vattrib&& move);

		// == operator overload
		bool operator==(const vattrib& op) const;

		void clear();
		void dump();

		/*
		*/
		unsigned char* data() const { return m_attributeArray; }

		/*
			Total size in bytes
		*/
		size_t size() const { return m_arraySize; }

		/*
		*/
		size_t count() const { return m_count; }

		/*
		*/
		GLenum type() const { return m_type; }

		/*
		*/
		GLenum target() const { return m_target; }
		void setTarget(GLenum target) { m_target = target; }

		/*
		*/
		size_t getVertexCount() const {
			size_t size = 0;

			switch (m_type) {
			case GL_UNSIGNED_SHORT:
				size = (m_arraySize / sizeof(GLushort)) / m_count;
				break;

			case GL_FLOAT:
				size = (m_arraySize / sizeof(GLfloat)) / m_count;
				break;
			}

			return size;
		}

	private:
		// array of vertex attribute data
		unsigned char* m_attributeArray = nullptr;

		// array size in bytes
		size_t m_arraySize = 0;

		// count of elements per vertex attribute (i.e. must be 4 for RGBA)
		size_t m_count = 0;

		// data type of the attribute (based on OpenGL data type: i.e. GL_FLOAT is 0x1406)
		size_t m_type = 0;

		// buffer binding target
		GLenum m_target = 0;
	};


	/*
		This class stores handles for vertex buffer data (VBO)
	*/
	class gpu_buffer {
	public:
		gpu_buffer() {}
		explicit gpu_buffer(size_t size);
		~gpu_buffer();

		gpu_buffer(const gpu_buffer& copy) = delete; // copy constructor
		gpu_buffer(gpu_buffer&& move) noexcept; // move constructor
		gpu_buffer& operator=(const gpu_buffer& copy) = delete; // copy assignment
		gpu_buffer& operator=(gpu_buffer&&) = delete; // move assigment

		GLuint& operator[](size_t i) { return m_buffers[i]; } // (!) check invalid index
		GLuint operator[](size_t i) const { return m_buffers[i]; } // (!) check invalid index

		void addBuffers(size_t size);
		void load(size_t index, const kengine::vattrib& va);
		void load(size_t index, GLenum target, size_t size, const void* data);
		void update(size_t index, GLenum target, size_t size, const void* data);

		void clear();
		size_t size() const { return m_buffers.size(); }

	private:
		std::vector<GLuint> m_buffers;
	};


	/*
		This class is the fundamental unit of rendering - the data required for GPU draw calls

		Note: This class object can't be copied neither moved
		Note: VAO is created in the constructor. So, a public method for clear don't make sense because is there no situation to clear the primitive_render after it has been created.
		Note: primitive_render pode referenciar seu próprio gpu_buffer (local) ou um gpu_buffer que foi alocado externamente (global). Quando estiver utilizando gpu_buffer global, gpu_buffer deve ser nullptr.
	*/
	class primitive_render
	{
	public:
		primitive_render();
		explicit primitive_render(std::unique_ptr<gpu_buffer>&& buffer);
		~primitive_render();

		primitive_render(const primitive_render& copy) = delete; // copy constructor
		primitive_render& operator=(const primitive_render& copy) = delete; // copy assigment
		primitive_render(primitive_render&& move) = delete; // move constructor
		primitive_render& operator=(primitive_render&& move) = delete; // move assigment

		void loadAttributeInfo(GLuint location, GLuint vbo, GLenum target, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);
		void loadAttributeInfo(GLuint location, GLuint vbo, const kengine::vattrib& va);

		void bindBuffer(GLenum target, GLuint buffer);
		void bindBuffer(GLuint vbo, const kengine::vattrib& va);

		void drawArrays(GLint start, GLsizei count);
		void drawArraysInstanced(GLint start, GLsizei count, GLsizei instanceCount);
		void drawElements(GLsizei count, const void* offset);
		void draw();
		void drawInstanced(GLsizei instanceCount);

		void setMode(GLenum mode) { m_mode = mode; }
		void setCount(GLsizei count) { m_count = count; }
		void setElements(GLsizei elements) { m_elements = elements; }
		void setElementsOffset(GLsizei offset) { m_elementsOffset = offset; }
		void setIndexType(GLenum type) { m_indexType = type; }
	

	private:
		GLsizei m_count = 0;
		GLsizei m_elements = 0;
		GLsizei m_elementsOffset = 0;
		GLuint m_vao = 0;
		GLenum m_mode = GL_TRIANGLES;
		GLenum m_indexType = GL_UNSIGNED_SHORT;
		std::unique_ptr<gpu_buffer> m_buffer; // local gpu buffer or global gpu buffer
		
		void clear();
	};

	/*
		This class stores array of primitive_render
	*/
	class mesh_render
	{
	public:
		mesh_render() {}
		~mesh_render() {}

		mesh_render(const mesh_render& copy) = delete; // copy constructor
		mesh_render(mesh_render&& move) = delete; // move constructor
		mesh_render& operator=(const mesh_render& copy) = delete; // copy assignment
		mesh_render& operator=(mesh_render&& move) = delete; // move assigment

		void draw() {
			for (auto& prim : m_primitives) {
				prim->draw();
			}
		}

		void pushPrimitive(std::unique_ptr<kengine::primitive_render>&& primitive) { m_primitives.push_back(std::move(primitive)); }
		void setName(const std::string& name) { m_name = name; }
		std::string getName() const { return m_name; }

	private:
		std::vector<std::unique_ptr<kengine::primitive_render>> m_primitives;
		std::string m_name;
	};


	/*
	*/
	enum class TEXTURE_TYPE {
		_2D,
	};


	/*
	*/
	class texture {
	public:
		texture() {};
		~texture();

		texture(const texture& copy) = delete; // copy constructor
		texture(texture&& move) noexcept = delete; // move constructor
		texture& operator=(const texture& copy) = delete; // copy assignment
		texture& operator=(texture&&) = delete; // move assigment

		void create(TEXTURE_TYPE target, GLsizei levels, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data);
		void update(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data);
		void bindTextureUnit(GLuint textureUnit);
		void bind();
		void unbind();
		void setWrappingMode(GLint sParam, GLint tParam);
		void clear();
	
	private:
		GLuint m_name = 0;
		GLenum m_target = 0;
	};

	/*
		OpenGL callback function for debug messages (https://www.khronos.org/opengl/wiki/Debug_Output)
		Note: Use breakpoint on OpenGL callback function "openGLDebugMessageCallback" with callstack actived for debbuging.
	*/
	void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param);

	/*
	*/
	std::unique_ptr<primitive_render> triangle(float size, std::unordered_map<std::string, GLuint>& shaderInfo);
	std::unique_ptr<primitive_render> quad(float width, float height, std::unordered_map<std::string, GLuint>& shaderInfo);
	std::unique_ptr<primitive_render> cube(float size, std::unordered_map<std::string, GLuint>& shaderInfo);
	std::unique_ptr<primitive_render> circle(float radius, size_t points, std::unordered_map<std::string, GLuint>& shaderInfo);
	// circle solid with triangles
	std::unique_ptr<primitive_render> sphere(GLfloat radius, size_t slices, size_t points, std::unordered_map<std::string, GLuint>& shaderInfo);

	std::unique_ptr<primitive_render> torus(size_t precision, GLfloat inner, GLfloat outer, std::unordered_map<std::string, GLuint>& shaderInfo);

	std::unique_ptr<primitive_render> grid3D(std::unordered_map<std::string, GLuint>& shaderInfo);
	std::unique_ptr<primitive_render> frustum3D(const projection_info<float>& projInfo, std::unordered_map<std::string, GLuint>& shaderInfo);

}

#endif