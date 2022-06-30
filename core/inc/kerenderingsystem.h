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
	//  (!) struct kengine::ShaderInfo
	//
	//	Esta struct serve para passar uma lista de shaders que podem ser
	//  compilados.
	// 
	//		- GL_VERTEX_SHADER
	//		- GL_GEOMETRY_SHADER
	//		- GL_TESS_EVALUATION_SHADER
	//		- GL_TESS_CONTROL_SHADER
	//		- GL_COMPUTE_SHADER
	//		- GL_FRAGMENT_SHADER
	// ------------------------------------------------------------------------
	struct ShaderInfo
	{
		GLuint type;
		std::string filename;
	};


	// ------------------------------------------------------------------------
	//  função para compilar shader GLSL
	// ------------------------------------------------------------------------
	GLuint compileShader(GLuint shader_type, std::string filename);


	// ------------------------------------------------------------------------
	//  (!) kengine::GLSLprogram class
	// 
	//  Esta classe fornece a base para criar shaders mais complexos que
	//  utilizam uniform variables, uniform blocks etc.
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
		GLuint getProgram() const { return programID; }

	protected:
		GLuint programID;
	};


	// ------------------------------------------------------------------------
	//  (!) kengine::TransformProgram class
	// 
	//  Esta classe é um shader herdade de GLSLprogram que possui:
	//		- cores (vertex attributes)
	//		- coordenadas de textura UV (vertex attributes)
	//		- matrizes de visualização (vertex attributes)
	//		- matriz de projeção (uniform matrix)
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
		GLint projectionView_location;
	};


	// ------------------------------------------------------------------------
	//  (!) kengine::meshnode class
	// ------------------------------------------------------------------------
	class meshnode
	{
		friend class instancedmeshnode;
		static constexpr int VBO_COUNT = 2;

	public:
		explicit meshnode(const kengine::mesh& m);
		~meshnode();

		meshnode(const meshnode& copy) = delete; // copy constructor
		meshnode(meshnode&& move) noexcept = delete; // move constructor
		meshnode& operator=(const meshnode& copy) = delete; // copy assignment
		
		void draw();

	private:
		GLuint vao;
		GLuint vbo[VBO_COUNT];
		GLsizei count;
	};


	// ---------------------------------------------------------------------------
	//  kengine::instancedmodelnode class
	// ---------------------------------------------------------------------------
	class instancedmeshnode : public meshnode
	{
	public:
		instancedmeshnode(int size, kengine::mesh& m);
		~instancedmeshnode();

		instancedmeshnode(const instancedmeshnode& copy) = delete; // copy constructor
		instancedmeshnode(instancedmeshnode&& move) noexcept = delete; // move constructor
		instancedmeshnode& operator=(const instancedmeshnode& copy) = delete; // copy assignment

		void update(const long long int size, float* data) const;
		void draw(int size) const;

	private:
		int max_size;
		GLuint modelview_vbo;
	};


	// ------------------------------------------------------------------------
	//  (!) kengine::instanceduvmeshnode class
	// ------------------------------------------------------------------------
	class instanceduvmeshnode
	{
		static constexpr int TOTAL_VBO = 3;

	public:
		explicit instanceduvmeshnode(const size_t size, const kengine::mesh& m);
		~instanceduvmeshnode();

		instanceduvmeshnode(const instanceduvmeshnode& copy) = delete; // copy constructor
		instanceduvmeshnode(instanceduvmeshnode&& move) noexcept = delete; // move constructor
		instanceduvmeshnode& operator=(const instanceduvmeshnode& copy) = delete; // copy assignment

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
