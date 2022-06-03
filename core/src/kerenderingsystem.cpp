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

#include <kerenderingsystem.h>
#include <kewinapiwrapper.h>
#include <keconstants.h>
#include <keaux.h>
#include <keutils.h>

#include <iostream>


// ----------------------------------------------------------------------------
//  função para compilar shader GLSL
// ----------------------------------------------------------------------------
GLuint kengine::compileShader(GLuint shader_type, std::string filename)
{
	GLuint shader = glCreateShader(shader_type);

	if (!shader)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create a vertex shader!")
		return 0;
	}

	std::string source = readFromFile(filename);

	if (source == "")
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "This file " << filename << " cannot be opened.")
		glDeleteShader(shader);
		return 0;
	}

	const GLchar* vertexSource = source.c_str();

	glShaderSource(shader, 1, &vertexSource, nullptr);
	glCompileShader(shader);

	GLint param = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &param);

	if (param == GL_FALSE)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &param);

		if (param > 0)
		{
			std::string infoLog(static_cast<unsigned int>(param), ' ');
			GLsizei length;
			glGetShaderInfoLog(shader, param, &length, &infoLog[0]);
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to compile a shader [" << shader_type << "]:\n" << infoLog)
		}

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}


// ------------------------------------------------------------------------
//  (!) kengine::GLSLprogram class
// 
//  Esta classe fornece a base para criar shaders mais complexos que
//  utilizam uniform variables, uniform blocks etc.
// ------------------------------------------------------------------------
kengine::GLSLprogram::GLSLprogram()
	: programID{ 0 }
{
}


kengine::GLSLprogram::~GLSLprogram()
{
	glDeleteProgram(programID);
}


bool kengine::GLSLprogram::loadShaders(kengine::ShaderInfo* shaderInfo)
{
	if (shaderInfo)
	{
		if (programID)
		{
			glDeleteProgram(programID);
		}

		GLuint shaders[6] = { 0 };

		for (int index = 0; shaderInfo[index].type != GL_NONE && index < 6; index++)
		{
			shaders[index] = compileShader(shaderInfo[index].type, shaderInfo[index].filename);
		}

		programID = glCreateProgram();

		if (!programID)
		{
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create a shader program!")
			return false;
		}

		for (int index = 0; index < 6; index++)
		{
			if (shaders[index])
			{
				glAttachShader(programID, shaders[index]);
			}
		}

		glLinkProgram(programID);

		GLint param;
		glGetProgramiv(programID, GL_LINK_STATUS, &param);

		if (param == GL_FALSE)
		{
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &param);

			if (param > 0)
			{
				std::string infoLog(static_cast<unsigned int>(param), ' ');
				GLsizei length;
				glGetProgramInfoLog(programID, param, &length, &infoLog[0]);
				K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to link a shader program: \n" << infoLog)
			}
		}

		for (int index = 0; index < 6; index++)
		{
			if (shaders[index])
			{
				glDetachShader(programID, shaders[index]);
				glDeleteShader(shaders[index]);
			}
		}

		return true;
	}

	return false;
}


void kengine::GLSLprogram::useProgram()
{
	if (programID)
		glUseProgram(programID);
}


// ------------------------------------------------------------------------
//  (!) kengine::TransformProgram class
// 
//  Esta classe é um shader herdade de GLSLprogram que possui:
//		- cores (vertex attributes)
//		- coordenadas de textura UV (vertex attributes)
//		- matrizes de visualização (vertex attributes)
//		- matriz de projeção (uniform matrix)
// ------------------------------------------------------------------------
kengine::TransformProgram::TransformProgram()
	:projectionView_location{ -1 }
{
}


kengine::TransformProgram::~TransformProgram()
{
}


bool kengine::TransformProgram::loadShaders(kengine::ShaderInfo* shaders)
{
	if (!GLSLprogram::loadShaders(shaders))
		return false;

	projectionView_location = glGetUniformLocation(programID, "projectionView");

	return true;
}


void kengine::TransformProgram::setProjection(kengine::matrix& p)
{
	if (projectionView_location != -1)
	{
		glUniformMatrix4fv(projectionView_location, 1, GL_FALSE, p.value());
	}
}


// ------------------------------------------------------------------------
//  kengine::modelnode class
// ------------------------------------------------------------------------
kengine::modelnode::modelnode(kengine::model& m)
	: vao{ 0 }, vbo{ 0 }, count{ 0 }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::modelnode constructor with argument - [" << this << "]")

	glCreateBuffers(VBO_COUNT, vbo);

	if (glGetError() == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create VBO : " << glGetError())
	}

	//  store elements in VBO
	const kengine::vattrib<unsigned int>* indices = m.getIndices();
	glNamedBufferStorage(vbo[0], static_cast<GLsizeiptr>(indices->getSizeinBytes()), indices->attributeArray, 0);

	GLuint error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (index): " << glGetError())
	}

	//  store vertex attributes in VBO
	glNamedBufferStorage(vbo[1], static_cast<GLsizeiptr>(m.getSizeInBytes()), nullptr, GL_DYNAMIC_STORAGE_BIT);

	error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (vertex attributes): " << glGetError())
	}

	const kengine::vattrib<float>* coordsPTR = m.getCoords();
	glNamedBufferSubData(vbo[1], 0, static_cast<GLsizeiptr>(coordsPTR->getSizeinBytes()), coordsPTR->attributeArray);

	const kengine::vattrib<float>* colorsPTR = m.getColors();
	glNamedBufferSubData(vbo[1], static_cast<GLsizeiptr>(coordsPTR->getSizeinBytes()), static_cast<GLsizeiptr>(colorsPTR->getSizeinBytes()), colorsPTR->attributeArray);

	const kengine::vattrib<float>* texCoordsPTR = m.getTexCoords();
	glNamedBufferSubData(vbo[1], static_cast<GLsizeiptr>(coordsPTR->getSizeinBytes() + colorsPTR->getSizeinBytes()), static_cast<GLsizeiptr>(texCoordsPTR->getSizeinBytes()), texCoordsPTR->attributeArray);

	//  creating VAO
	glCreateVertexArrays(1, &vao);

	if (glGetError() == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create vao: " << glGetError())
	}

	glBindVertexArray(vao);

	if (glGetError() == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to bind VAO : " << glGetError())
	}

	//  Shader Plumbing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)coordsPTR->getSizeinBytes());
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(coordsPTR->getSizeinBytes() + colorsPTR->getSizeinBytes()));

	count = static_cast<GLsizei>(m.getIndices()->getSize());
}


kengine::modelnode::~modelnode()
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::modelnode destructor - [" << this << "]")

	glInvalidateBufferData(vbo[0]);
	glInvalidateBufferData(vbo[1]);
	glDeleteBuffers(VBO_COUNT, vbo);
	glDeleteVertexArrays(1, &vao);
}


void kengine::modelnode::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}


// ----------------------------------------------------------------------------
//  kengine::instancedmodelnode class
// ----------------------------------------------------------------------------
kengine::instancedmodelnode::instancedmodelnode(int size, kengine::model& m)
	: modelnode{ m }, max_size{ size }, modelview_vbo{ 0 }
{
	glCreateBuffers(1, &modelview_vbo);
	glNamedBufferStorage(modelview_vbo, static_cast<GLsizeiptr>(max_size * 16LL * sizeof(GLfloat)), nullptr, GL_DYNAMIC_STORAGE_BIT);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, modelview_vbo);

	for (unsigned int i = 0; i < 4; i++)
	{
		glVertexAttribPointer(3UL + i, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizeiptr>(16 * sizeof(GLfloat)), (const GLvoid*)(sizeof(float) * 4 * i));
		glEnableVertexAttribArray(3UL + i);
		glVertexAttribDivisor(3UL + i, 1);
	}
}

kengine::instancedmodelnode::~instancedmodelnode()
{
	glInvalidateBufferData(modelview_vbo);
	glDeleteBuffers(1, &modelview_vbo);
}

void kengine::instancedmodelnode::update(const long long int size, float* data) const
{
	glNamedBufferSubData(modelview_vbo, 0, static_cast<GLsizeiptr>(size * 16LL * sizeof(GLfloat)), data);
}

void kengine::instancedmodelnode::draw(int size) const
{
	if (size < 0)
		size = 0;

	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, size);
}


// ---------------------------------------------------------------------------
//  kengine::texture class
// ---------------------------------------------------------------------------
kengine::texture::texture(const kengine::raw_img& img)
	: id{0}
{
	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	GLenum error = glGetError();

	if (error == GL_INVALID_ENUM || error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create texture object: " << error)
	}

	glTextureStorage2D(id, 1, GL_RGBA8, img.getWidth(), img.getHeight());

	error = glGetError();

	if (error == GL_INVALID_OPERATION || error == GL_INVALID_ENUM || error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to storage texture object: " << error)
	}

	glTextureSubImage2D(id, 0, 0, 0, img.getWidth(), img.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());

	if (error == GL_INVALID_OPERATION || error == GL_INVALID_ENUM || error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to specify texture subimage: " << error)
	}
}


kengine::texture::~texture()
{
	glDeleteTextures(1, &id);

	GLenum error = glGetError();

	if (error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to delete texture object: " << error)
	}
}


void kengine::texture::bindTexture(int unit, int texture)
{
	if (texture)
		glBindTextureUnit(static_cast<GLuint>(unit), id);
	else
		glBindTextureUnit(static_cast<GLuint>(unit), 0);

	GLenum error = glGetError();

	if (error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to bind texture unit: " << error)
	}
}


// ----------------------------------------------------------------------------
//  kengine::rendersystem class
// ----------------------------------------------------------------------------
kengine::renderingsystem::renderingsystem(kengine::win32wrapper* w)
	: win(w)
{
}


kengine::renderingsystem::~renderingsystem()
{
}


int kengine::renderingsystem::startup()
{
	if (!win)
	{
		return 0;
	}

	win->initializeRenderingSystem();
	getProcedureAddress();

	return 1;
}


void kengine::renderingsystem::printInfo() const
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	int maxTextureImageUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits);

	int maxCombinedTexImageUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTexImageUnits);

	std::cout << "> Rendering System: OpenGL\n"
		<< "  renderer: " << renderer << "\n"
		<< "  vendor: " << vendor << "\n"
		<< "  version: " << version << "\n"
		<< "  GLSL Version: " << glslVersion << "\n"
		<< "  max vertex attribs: " << GL_MAX_VERTEX_ATTRIBS << "\n"
		<< "  max texture units: " << maxTextureImageUnits << "\n"
		<< "  max combined texture units: " << maxCombinedTexImageUnits << "\n"
		<< "  max texture buffer size: " << GL_MAX_TEXTURE_BUFFER_SIZE << "\n"
		//<< "  max texture size: " << GL_MAX_TEXTURE_SIZE << "\n"
		//<< "  max uniform block size: " << GL_MAX_UNIFORM_BLOCK_SIZE << "\n"
		//<< "  max shader storage block size: " << GL_MAX_SHADER_STORAGE_BLOCK_SIZE << "\n"
		<< std::endl;
}


int kengine::renderingsystem::setVSync(int vsync)
{
	return wglSwapIntervalEXT(vsync);
}


void kengine::renderingsystem::setPolygonMode(int modeParam) const
{
	GLenum mode = GL_FILL;

	if (modeParam == K_RENDERING_MODE_POINT)
		mode = GL_POINT;
	else if (modeParam == K_RENDERING_MODE_LINE)
		mode = GL_LINE;

	glPolygonMode(GL_FRONT_AND_BACK, mode);
}


void kengine::renderingsystem::setCullFace(int modeParam) const
{
	if (modeParam == K_RENDERING_MODE_CULL_FACE_DISABLE)
	{
		glDisable(GL_CULL_FACE);
		return;
	}

	glEnable(GL_CULL_FACE);

	switch (modeParam)
	{
	case K_RENDERING_MODE_CULL_FACE_FRONT:
		glCullFace(GL_FRONT);
		break;

	case K_RENDERING_MODE_CULL_FACE_BACK:
		glCullFace(GL_BACK);
		break;

	case K_RENDERING_MODE_CULL_FACE_FRONT_AND_BACK:
		glCullFace(GL_FRONT_AND_BACK);
		break;
	}
}


void kengine::renderingsystem::setViewport(int x, int y, int width, int height) const
{
	glViewport(x, y, width, height);
}