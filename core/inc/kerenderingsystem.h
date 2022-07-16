 /*
	K-Engine Rendering System
	This file is part of the K-Engine.

	Copyright (C) 2022 Fabio Takeshi Ishikawa

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

#include <keglwrapper.h>
#include <kemesh.h>
#include <kemath.h>
#include <keraw.h>

#include <string>


namespace kengine
{
	// ------------------------------------------------------------------------
	// 
	//  (!) struct kengine::ShaderInfo
	//
	//	This struct is used to pass a list of shaders that can be compiled.
	// 
	//		- GL_VERTEX_SHADER
	//		- GL_GEOMETRY_SHADER
	//		- GL_TESS_EVALUATION_SHADER
	//		- GL_TESS_CONTROL_SHADER
	//		- GL_COMPUTE_SHADER
	//		- GL_FRAGMENT_SHADER
	// 
	// ------------------------------------------------------------------------
	struct ShaderInfo
	{
		GLuint type;
		std::string filename;
	};


	// ------------------------------------------------------------------------
	// 
	//  (!) Helper function to compile GLSL shader.
	// 
	// ------------------------------------------------------------------------
	GLuint compileShader(GLuint shader_type, std::string filename);


	// ------------------------------------------------------------------------
	// 
	//  (!) kengine::GLSLprogram class
	//
	//  This class provides the basis for creating more complex shader program
	//  that use uniform variables, uniform blocks, etc.
	// 
	// ------------------------------------------------------------------------
	class GLSLprogram
	{
	public:
		GLSLprogram();
		~GLSLprogram();

		GLSLprogram(const GLSLprogram& copy) = delete; // copy constructor
		GLSLprogram(GLSLprogram&& move) = delete; // move constructor
		GLSLprogram& operator=(const GLSLprogram& copy) = delete; // copy assignment

		bool loadShaders(kengine::ShaderInfo* shaders);
		void useProgram();

	protected:
		GLuint programID;
	};


	// ------------------------------------------------------------------------
	// 
	//  (!) kengine::TransformProgram class
	//
	//  This class is a shader inherited from GLSLprogram that has the
	//  following uniform variables:
	// 
	//		- projection view matrix (uniform matrix)
	// 
	// ------------------------------------------------------------------------
	class TransformProgram : public GLSLprogram
	{
	public:
		TransformProgram();
		~TransformProgram();

		TransformProgram(const TransformProgram& copy) = delete; // copy constructor
		TransformProgram(TransformProgram&& move) = delete; // move constructor
		TransformProgram& operator=(const TransformProgram& copy) = delete; // copy assignment

		bool loadShaders(kengine::ShaderInfo* shaders);
		void setProjection(kengine::matrix& p);

	private:
		GLint projectionViewLocation;
	};


	// ------------------------------------------------------------------------
	//  (!) kengine::meshnode class
	// ------------------------------------------------------------------------
	class mesh_node
	{
		friend class instanced_mesh_node;
		static constexpr int VBO_COUNT = 2;

	public:
		explicit mesh_node(const kengine::mesh& m);
		~mesh_node();

		mesh_node(const mesh_node& copy) = delete; // copy constructor
		mesh_node(mesh_node&& move) noexcept = delete; // move constructor
		mesh_node& operator=(const mesh_node& copy) = delete; // copy assignment
		
		void draw();

	private:
		GLuint vao;
		GLuint vbo[VBO_COUNT];
		GLsizei count;
	};


	// ---------------------------------------------------------------------------
	//  (!) kengine::instanced_mesh_node class
	// ---------------------------------------------------------------------------
	class instanced_mesh_node : public mesh_node
	{
	public:
		instanced_mesh_node(int size, kengine::mesh& m);
		~instanced_mesh_node();

		instanced_mesh_node(const instanced_mesh_node& copy) = delete; // copy constructor
		instanced_mesh_node(instanced_mesh_node&& move) noexcept = delete; // move constructor
		instanced_mesh_node& operator=(const instanced_mesh_node& copy) = delete; // copy assignment

		void update(const long long int size, float* data) const;
		void draw(int size) const;

	private:
		int max_size;
		GLuint modelview_vbo;
	};


	// ------------------------------------------------------------------------
	//  (!) kengine::instanced_uv_mesh_node class
	// ------------------------------------------------------------------------
	class instanced_uv_mesh_node
	{
		static constexpr int TOTAL_VBO = 3;

	public:
		explicit instanced_uv_mesh_node(const size_t size, const kengine::mesh& m);
		~instanced_uv_mesh_node();

		instanced_uv_mesh_node(const instanced_uv_mesh_node& copy) = delete; // copy constructor
		instanced_uv_mesh_node(instanced_uv_mesh_node&& move) noexcept = delete; // move constructor
		instanced_uv_mesh_node& operator=(const instanced_uv_mesh_node& copy) = delete; // copy assignment

		void updateModelView(size_t size, float* modelview) const;
		void updateUV(size_t size, float* uv) const;

		void draw(int size) const;

	private:
		size_t max_size;
		GLuint vao;
		GLuint vbo[TOTAL_VBO];
		GLsizei count;
	};


	// ---------------------------------------------------------------------------
	// 
	//  (!) kengine::primitive_mesh_batch class
	//
	//  This is a abstract opengl batch class to store and draw N primitive
	//  objects like points, lines, quadrilaterals, and etc.
	//
	//  Note: max_size is a quan
	// 
	// ---------------------------------------------------------------------------
	enum class PRIMITIVE_TYPE { PRIMITIVE_POINT, PRIMITIVE_LINE };

	class primitive_mesh_batch
	{
		static constexpr int TOTAL_VBO = 1;

	public:
		primitive_mesh_batch(const size_t size, const PRIMITIVE_TYPE primType, const float* color);
		~primitive_mesh_batch();

		primitive_mesh_batch(const primitive_mesh_batch& copy) = delete; // copy constructor
		primitive_mesh_batch(primitive_mesh_batch&& move) noexcept = delete; // move constructor
		primitive_mesh_batch& operator=(const primitive_mesh_batch& copy) = delete; // copy assignment

		void setPointSize(const float pointSize);
		void setLineWidth(const float width);

		void update(size_t size, float* data) const;
		void draw(int size) const;

	private:
		size_t batchSize;
		GLenum mode;
		GLuint vbo[TOTAL_VBO];
		GLuint vao;
	};


	// ---------------------------------------------------------------------------
	//  kengine::texture class
	// ---------------------------------------------------------------------------
	class texture
	{
	public:
		texture(const kengine::raw_img& img, GLuint textureUnit);
		~texture();

		texture(const texture& copy) = delete; // copy constructor
		texture(texture&& move) noexcept = delete; // move constructor
		texture& operator=(const texture& copy) = delete; // copy assignment

		void bindTexture(int texture);

	private:
		GLuint id;
		GLuint texUnit;
	};


	// ---------------------------------------------------------------------------
	//  kengine::atlas class
	// ---------------------------------------------------------------------------
	class atlas
	{
	public:
		atlas(std::string filename, const size_t frames, GLuint textureUnit);
		~atlas();

		void bindTexture(int texture);
		void copyFrame(const int frame, float* buffer);

	private:
		kengine::texture* tex;
		float* uv;
	};


	// ------------------------------------------------------------------------
	//  kengine::viewing_window class
	// ------------------------------------------------------------------------
	class viewing_window
	{
	public:
		viewing_window();
		~viewing_window();

		viewing_window(const viewing_window& copy) = delete; // copy constructor
		viewing_window(viewing_window&& move) noexcept = delete; // move constructor
		viewing_window& operator=(const viewing_window& copy) = delete; // copy assignment

		float _near;
		float _far;
		kengine::rect window;
		kengine::matrix projection;
	};

	// ---------------------------------------------------------------------------
	//  kengine::renderingsystem class
	// ---------------------------------------------------------------------------
	class win32wrapper;

	enum class RENDER_CONTEXT { RENDER_CONTEXT_2D, RENDER_CONTEXT_3D_ORTHO, RENDER_CONTEXT_3D_FRUSTUM };

	class renderingsystem
	{
	public:
		explicit renderingsystem(kengine::win32wrapper* w);
		renderingsystem(kengine::win32wrapper* w, RENDER_CONTEXT context);
		~renderingsystem();

		renderingsystem(const renderingsystem& copy) = delete; // copy constructor
		renderingsystem(renderingsystem&& move) noexcept = delete; // move constructor
		renderingsystem& operator=(const renderingsystem& copy) = delete; // copy assignment

		int startup();
		void printInfo() const;

		int setVSync(int vsync);
		void setPolygonMode(int mode) const;
		void setCullFace(int mode) const;
		void setDepthTest(int mode) const;
		void setBlendingTest(int mode) const;
		void setViewport(int x, int y, int width, int height) const;
		
		void setViewingWindow(int width, int height, float left, float right, float bottom, float top, float nearPlane, float farPlane);
		void setViewingWindow(int width, int height);
		const kengine::matrix& getProjection() const;

		// getters and setters
		void setRenderContext(RENDER_CONTEXT context) { renderContext = context; }

	private:
		RENDER_CONTEXT renderContext;
		kengine::win32wrapper* api;
		kengine::viewing_window viewingWindow;
	};
}

#endif
