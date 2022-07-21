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
//  This class is a shader inherited from GLSLprogram that has the
//  following uniform variables:
// 
//		- projection view matrix (uniform matrix)
// 
// ------------------------------------------------------------------------
kengine::TransformProgram::TransformProgram()
	: uniformLocations{ nullptr }
{
}


kengine::TransformProgram::~TransformProgram()
{
	delete[] uniformLocations;
}


bool kengine::TransformProgram::loadShaders(kengine::ShaderInfo* shaders)
{
	if (!GLSLprogram::loadShaders(shaders))
		return false;

	// getting the list of active uniform variables
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(programID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	if (numUniforms > 0)
	{
		uniformLocations = new GLint[static_cast<size_t>(numUniforms)];

		GLenum properties[] = { GL_LOCATION, GL_BLOCK_INDEX };

		for (auto i = 0; i < numUniforms; i++)
		{
			GLint results[2];
			glGetProgramResourceiv(programID, GL_UNIFORM, static_cast<GLuint>(i), 2, properties, 2, nullptr, results);

			// skip uniforms in blocks
			if (results[1] != -1)
				continue;

			uniformLocations[i] = results[0];
		}
	}

	return true;
}


void kengine::TransformProgram::setUniformMatrix(const int location, kengine::matrix& m)
{
	if (uniformLocations != nullptr && location >= 0)
	{
		glUniformMatrix4fv(uniformLocations[location], 1, GL_FALSE, m.value());
	}
}


void kengine::TransformProgram::print() const
{
	// getting the list of active attribute variables
	GLint numAttribs;
	glGetProgramInterfaceiv(programID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	if (numAttribs > 0)
	{
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

		std::cout << "> TransformProgram object info [" << this << "]\n"
			<< "  active attribute variables:\n";

		for (auto i = 0; i < numAttribs; i++)
		{
			GLint results[3];
			glGetProgramResourceiv(programID, GL_PROGRAM_INPUT, static_cast<GLuint>(i), 3, properties, 3, nullptr, results);

			GLint nameBuffSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBuffSize)];

			glGetProgramResourceName(programID, GL_PROGRAM_INPUT, static_cast<GLuint>(i), nameBuffSize, nullptr, name);

			std::cout << "    - name: " << name << "\n    - type: " << results[1] << "\n    - location: " << results[2] << "\n\n";
			delete[] name;
		}

		std::cout << "\n  active uniform variables:\n";
	}

	GLint numUniforms = 0;
	glGetProgramInterfaceiv(programID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	if (numUniforms > 0)
	{
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

		for (auto i = 0; i < numUniforms; i++)
		{
			GLint results[4];
			glGetProgramResourceiv(programID, GL_UNIFORM, static_cast<GLuint>(i), 4, properties, 4, nullptr, results);

			GLsizei nameBuffSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBuffSize)];

			glGetProgramResourceName(programID, GL_UNIFORM, static_cast<GLuint>(i), nameBuffSize, nullptr, name);

			std::cout << "    - name: " << name << "\n    - type: " << results[1] << "\n    - location: " << results[2] << "\n    - block index: " << results[3] << "\n\n";
			delete[] name;
		}

		std::cout << std::endl;
	}
}


// ------------------------------------------------------------------------
//  (!) kengine::meshnode class
// ------------------------------------------------------------------------
kengine::mesh_node::mesh_node(const kengine::mesh& m)
	: vao{ 0 }, vbo{ 0 }, count{ 0 }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::modelnode constructor with argument - [" << this << "]")

	glCreateBuffers(VBO_COUNT, vbo);

	if (glGetError() == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create VBO : " << glGetError())
	}

	// store indices in VBO
	const kengine::vattrib<unsigned int>* indices = m.getIndices();
	glNamedBufferStorage(vbo[0], static_cast<GLsizeiptr>(indices->getSizeinBytes()), indices->attributeArray, 0);

	GLuint error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (index): " << glGetError())
	}

	//  store vertex attributes in VBO
	const kengine::vattrib<float>* positionPTR = m.getPosition();
	const kengine::vattrib<float>* colorsPTR = m.getColors();
	const kengine::vattrib<float>* texCoordsPTR = m.getTexCoords();

	GLsizeiptr sizeInBytes = static_cast<GLsizeiptr>(positionPTR->getSizeinBytes() + colorsPTR->getSizeinBytes() + texCoordsPTR->getSizeinBytes());
	glNamedBufferStorage(vbo[1], sizeInBytes, nullptr, GL_DYNAMIC_STORAGE_BIT);

	error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (vertex attributes): " << glGetError())
	}
	
	glNamedBufferSubData(vbo[1], 0, static_cast<GLsizeiptr>(positionPTR->getSizeinBytes()), positionPTR->attributeArray);
	glNamedBufferSubData(vbo[1], static_cast<GLsizeiptr>(positionPTR->getSizeinBytes()), static_cast<GLsizeiptr>(colorsPTR->getSizeinBytes()), colorsPTR->attributeArray);
	glNamedBufferSubData(vbo[1], static_cast<GLsizeiptr>(positionPTR->getSizeinBytes() + colorsPTR->getSizeinBytes()), static_cast<GLsizeiptr>(texCoordsPTR->getSizeinBytes()), texCoordsPTR->attributeArray);

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

	//  shader plumbing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)positionPTR->getSizeinBytes());
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(positionPTR->getSizeinBytes() + colorsPTR->getSizeinBytes()));

	count = static_cast<GLsizei>(m.getIndices()->getSize());
}


kengine::mesh_node::~mesh_node()
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::modelnode destructor - [" << this << "]")

	glInvalidateBufferData(vbo[0]);
	glInvalidateBufferData(vbo[1]);
	glDeleteBuffers(VBO_COUNT, vbo);
	glDeleteVertexArrays(1, &vao);
}


void kengine::mesh_node::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}


// ----------------------------------------------------------------------------
//  kengine::instanced_mesh_node class
// ----------------------------------------------------------------------------
kengine::instanced_mesh_node::instanced_mesh_node(int size, kengine::mesh& m)
	: mesh_node{ m }, max_size{ size }, modelview_vbo{ 0 }
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


kengine::instanced_mesh_node::~instanced_mesh_node()
{
	glInvalidateBufferData(modelview_vbo);
	glDeleteBuffers(1, &modelview_vbo);
}


void kengine::instanced_mesh_node::update(const long long int size, float* data) const
{
	glNamedBufferSubData(modelview_vbo, 0, static_cast<GLsizeiptr>(size * 16LL * sizeof(GLfloat)), data);
}


void kengine::instanced_mesh_node::draw(int size) const
{
	if (size < 0)
		size = 0;

	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, size);
}


// ------------------------------------------------------------------------
//  (!) kengine::instanced_uv_mesh_node class
// ------------------------------------------------------------------------
kengine::instanced_uv_mesh_node::instanced_uv_mesh_node(const size_t size, const kengine::mesh& m)
	: max_size{ size }, vao { 0 }, vbo{ { 0 } }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::batch constructor with argument - [" << this << "]")

	glCreateBuffers(TOTAL_VBO, vbo);

	if (glGetError() == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create VBO : " << glGetError())
	}

	// store indices in VBO
	const kengine::vattrib<unsigned int>* indices = m.getIndices();
	glNamedBufferStorage(vbo[0], static_cast<GLsizeiptr>(indices->getSizeinBytes()), indices->attributeArray, 0);

	GLuint error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (index): " << glGetError())
	}

	// store vertex attributes in VBO
	const kengine::vattrib<float>* positionPTR = m.getPosition();

	GLsizeiptr sizeInBytes = static_cast<GLsizeiptr>(positionPTR->getSizeinBytes());
	glNamedBufferStorage(vbo[1], sizeInBytes, nullptr, GL_DYNAMIC_STORAGE_BIT);

	error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (vertex attributes): " << glGetError())
	}

	glNamedBufferSubData(vbo[1], 0, static_cast<GLsizeiptr>(positionPTR->getSizeinBytes()), positionPTR->attributeArray);

	// store vertex attributes for texture coordinates and modelview matrix
	GLsizeiptr matrixSize = static_cast<GLsizeiptr>(max_size * 16LL * sizeof(GLfloat));
	GLsizeiptr uvSize = static_cast<GLsizeiptr>(max_size * (8 * sizeof(GLfloat)));
	glNamedBufferStorage(vbo[2], matrixSize + uvSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

	// creating VAO
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

	//  shader plumbing
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	for (unsigned int i = 0; i < 4; i++)
	{
		glVertexAttribPointer(3UL + i, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizeiptr>(16 * sizeof(GLfloat)), (const GLvoid*)(uvSize + sizeof(float) * 4 * i));
		glEnableVertexAttribArray(3UL + i);
		glVertexAttribDivisor(3UL + i, 1);
	}

	count = static_cast<GLsizei>(m.getIndices()->getSize());
}


kengine::instanced_uv_mesh_node::~instanced_uv_mesh_node()
{
	glInvalidateBufferData(vbo[0]);
	glInvalidateBufferData(vbo[1]);
	glInvalidateBufferData(vbo[2]);
	glDeleteBuffers(TOTAL_VBO, vbo);
	glDeleteVertexArrays(1, &vao);
}


void kengine::instanced_uv_mesh_node::updateModelView(size_t size, float* modelview) const
{
	if (size > max_size)
		size = max_size;

	GLsizeiptr uvSize = static_cast<GLsizeiptr>(max_size * (8 * sizeof(float)));
	glNamedBufferSubData(vbo[2], uvSize, static_cast<GLsizeiptr>(size * 16 * sizeof(GLfloat)), modelview);
}


void kengine::instanced_uv_mesh_node::updateUV(size_t size, float* uv) const
{
	if (size > max_size)
		size = max_size;

	glNamedBufferSubData(vbo[2], 0, static_cast<GLsizeiptr>(size * (8 * sizeof(float))), uv);
}


void kengine::instanced_uv_mesh_node::draw(int size) const
{
	if (size < 0)
		size = 0;

	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, size);
}


// ---------------------------------------------------------------------------
// 
//  (!) kengine::primitive_mesh_batch class
//
//      - member class definition
// 
// ---------------------------------------------------------------------------
kengine::primitive_mesh_batch::primitive_mesh_batch(const size_t size, const PRIMITIVE_TYPE primType, const float* color)
	: batchSize{ size }, mode{ 0 }, vbo{ 0 }, vao{ 0 }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::primitive_mesh_batch constructor with argument - [" << this << "]");

	glCreateBuffers(TOTAL_VBO, vbo);

	GLuint error = glGetError();

	if (error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "[primitive mesh batch] It was not possible to create VBO: " << glGetError())
	}

	// ---------------------------------------------------------------------------

	GLsizeiptr coordsSize = static_cast<GLsizeiptr>(batchSize * sizeof(GLfloat));
	GLsizeiptr colorSize = static_cast<GLsizeiptr>(4LL * sizeof(GLfloat));

	glNamedBufferStorage(vbo[0], coordsSize + colorSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

	error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "[primitive mesh batch] It was not possible to allocate space for VBO: " << glGetError())
	}

	glNamedBufferSubData(vbo[0], coordsSize, colorSize, color);

	// ---------------------------------------------------------------------------

	glCreateVertexArrays(1, &vao);

	if (glGetError() == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "[primitive mesh batch] It was not possible to create vao: " << glGetError())
	}

	glBindVertexArray(vao);

	if (glGetError() == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "[primitive mesh batch] It was not possible to bind VAO : " << glGetError())
	}

	// ---------------------------------------------------------------------------

	// shader plumbing
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)coordsSize);
	glVertexAttribDivisor(1, 1);

	// ---------------------------------------------------------------------------

	switch (primType)
	{
	case PRIMITIVE_TYPE::PRIMITIVE_POINT:
		mode = GL_POINTS;
		break;

	case PRIMITIVE_TYPE::PRIMITIVE_LINE:
		mode = GL_LINES;
		break;

	case PRIMITIVE_TYPE::PRIMITIVE_LINE_LOOP:
		mode = GL_LINE_LOOP;
		break;
	}
}


kengine::primitive_mesh_batch::~primitive_mesh_batch()
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::primitive_mesh_batch destructor - [" << this << "]");
	glInvalidateBufferData(vbo[0]);
	glDeleteBuffers(TOTAL_VBO, vbo);
	glDeleteVertexArrays(1, &vao);
}


void kengine::primitive_mesh_batch::setPointSize(const float pointSize)
{
	glPointSize(pointSize);
}


void kengine::primitive_mesh_batch::setLineWidth(const float width)
{
	glLineWidth(width);
}


void kengine::primitive_mesh_batch::update(size_t size, float* data) const
{
	if (size > batchSize)
		size = batchSize;

	glNamedBufferSubData(vbo[0], 0, static_cast<GLsizeiptr>(size * sizeof(float)), data);
}


void kengine::primitive_mesh_batch::draw(int size) const
{
	if (size < 0)
	{
		return;
	}

	glBindVertexArray(vao);
	glDrawArrays(mode, 0, size);
}


// ---------------------------------------------------------------------------
//  kengine::texture class
// ---------------------------------------------------------------------------
kengine::texture::texture(const kengine::raw_img& img, GLuint textureUnit)
	: id{ 0 }, texUnit{ textureUnit }
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

	glBindTextureUnit(texUnit, id);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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


void kengine::texture::bindTexture(int texture)
{
	if (texture)
		glBindTextureUnit(texUnit, id);
	else
		glBindTextureUnit(texUnit, 0);

	GLenum error = glGetError();

	if (error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to bind texture unit: " << error)
	}
}


// ---------------------------------------------------------------------------
//  kengine::atlas class
// ---------------------------------------------------------------------------
kengine::atlas::atlas(std::string filename, const size_t frames, GLuint textureUnit)
	: tex{ nullptr }, uv{ nullptr }
{
	kengine::raw_img img;

	if (img.loadfile(filename))
	{
		tex = new kengine::texture(img, textureUnit);

		uv = new float[8 * frames];

		float x_start_offset = 0.0f;
		float x_offset = 1.0f / static_cast<float>(frames);

		for (size_t i = 0; i < frames; i++)
		{
			uv[(i * 8) + 0] = x_start_offset;
			uv[(i * 8) + 1] = 0.0f;

			uv[(i * 8) + 2] = x_start_offset;
			uv[(i * 8) + 3] = 1.0f;

			uv[(i * 8) + 4] = x_start_offset + x_offset;
			uv[(i * 8) + 5] = 1.0f;

			uv[(i * 8) + 6] = x_start_offset + x_offset;
			uv[(i * 8) + 7] = 0.0f;

			x_start_offset += x_offset;
		}
	}
}


kengine::atlas::~atlas()
{
	delete tex;
	delete uv;
}


void kengine::atlas::bindTexture(int texture)
{
	tex->bindTexture(texture);
}


void kengine::atlas::copyFrame(const int frame, float* buffer)
{
	*(buffer + 0) = uv[frame * 8 + 0];
	*(buffer + 1) = uv[frame * 8 + 1];
	*(buffer + 2) = uv[frame * 8 + 2];
	*(buffer + 3) = uv[frame * 8 + 3];
	*(buffer + 4) = uv[frame * 8 + 4];
	*(buffer + 5) = uv[frame * 8 + 5];
	*(buffer + 6) = uv[frame * 8 + 6];
	*(buffer + 7) = uv[frame * 8 + 7];
}


// ------------------------------------------------------------------------
//  kengine::viewing_window member class definition
// ------------------------------------------------------------------------
kengine::viewing_window::viewing_window()
	:
		_near{ -1.0f },
		_far{ 1.0f },
		window{ 0.0f, 0.0f, 0.0f, 0.0f },
		projection{ 1 }
{
}


kengine::viewing_window::~viewing_window()
{
}


// ----------------------------------------------------------------------------
//  kengine::rendersystem class
// ----------------------------------------------------------------------------
kengine::renderingsystem::renderingsystem(kengine::win32wrapper* w)
	:
		renderContext{ kengine::RENDER_CONTEXT::RENDER_CONTEXT_2D },
		api{ w },
		viewingWindow{}
{
}


kengine::renderingsystem::renderingsystem(kengine::win32wrapper* w, RENDER_CONTEXT context)
	:
		renderContext { context },
		api{ w },
		viewingWindow{}
{
}


kengine::renderingsystem::~renderingsystem()
{
}


int kengine::renderingsystem::startup()
{
	if (!api)
	{
		return 0;
	}

	api->initializeRenderingSystem();
	getProcedureAddress();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 1;
}


void kengine::renderingsystem::printInfo() const
{
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* version = glGetString(GL_VERSION);
	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	int maxVertexAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);

	int maxTextureImageUnits;
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureImageUnits);

	int maxCombinedTexImageUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTexImageUnits);

	int maxTextureBufferSize;
	glGetIntegerv(GL_MAX_TEXTURE_BUFFER_SIZE, &maxTextureBufferSize);

	int maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

	std::cout << "> Rendering System: OpenGL\n"
		<< "  renderer: " << renderer << "\n"
		<< "  vendor: " << vendor << "\n"
		<< "  version: " << version << "\n"
		<< "  GLSL Version: " << glslVersion << "\n"
		<< "  max vertex attribs: " << maxVertexAttribs << "\n"
		<< "  max texture units: " << maxTextureImageUnits << "\n"
		<< "  max combined texture units: " << maxCombinedTexImageUnits << "\n"
		<< "  max texture buffer size: " << maxTextureBufferSize << "\n"
		<< "  max texture size: " << maxTextureSize << "\n"
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


void kengine::renderingsystem::setDepthTest(int mode) const
{
	if (mode)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}


void kengine::renderingsystem::setBlendingTest(int mode) const
{
	if (mode)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}


void kengine::renderingsystem::setViewport(int x, int y, int width, int height) const
{
	glViewport(x, y, width, height);
}


void kengine::renderingsystem::setViewingWindow(int width, int height, float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	viewingWindow.window.left = left;
	viewingWindow.window.right = right;
	viewingWindow.window.bottom = bottom;
	viewingWindow.window.top = top;
	viewingWindow._near = nearPlane;
	viewingWindow._far = farPlane;

	float aspectRatio = 1.0f;
	kengine::rect window = viewingWindow.window;

	if (width <= height)
	{
		aspectRatio = static_cast<float>(height) / static_cast<float>(width);
		window.bottom *= aspectRatio;
		window.top *= aspectRatio;
	}
	else
	{
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		window.left *= aspectRatio;
		window.right *= aspectRatio;
	}

	if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_2D)
	{
		viewingWindow.projection = kengine::ortho(
			window.left,
			window.right,
			window.bottom,
			window.top,
			-1.0f,
			1.0f);
	}
	else if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_3D_ORTHO)
	{
		viewingWindow.projection = kengine::ortho(
			window.left,
			window.right,
			window.bottom,
			window.top,
			viewingWindow._near,
			viewingWindow._far);
	}
	else if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_3D_FRUSTUM)
	{
		viewingWindow.projection = kengine::frustum(
			window.left,
			window.right,
			window.bottom,
			window.top,
			viewingWindow._near,
			viewingWindow._far);
	}
}


void kengine::renderingsystem::setViewingWindow(int width, int height)
{
	setViewingWindow(
		width,
		height,
		viewingWindow.window.left,
		viewingWindow.window.right,
		viewingWindow.window.bottom,
		viewingWindow.window.top,
		viewingWindow._near,
		viewingWindow._far);
}


const kengine::matrix& kengine::renderingsystem::getProjection() const
{
	return viewingWindow.projection;
}