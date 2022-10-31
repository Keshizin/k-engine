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
#include <vector>


/*
* 
*  Helper function to compile GLSL shader.
* 
*/
GLuint kengine::compileShader(GLuint shader_type, std::string filename)
{
	GLuint shaderObject = glCreateShader(shader_type);

	if (!shaderObject)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create a vertex shader!")
		return 0;
	}

	std::string sourceString = readFromFile(filename);

	if (sourceString[0] == '\0')
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "This file " << filename << " cannot be opened.")
		glDeleteShader(shaderObject);
		return 0;
	}

	const GLchar* source = sourceString.c_str();

	glShaderSource(shaderObject, 1, &source, nullptr);
	glCompileShader(shaderObject);

	GLint status = 0;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		std::string log = "It was not possible to compile a shader [" + getShaderType(shader_type) + "]";

		GLint logLength;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0)
		{
			std::string infoLog(static_cast<unsigned int>(logLength), ' ');
			GLsizei logWrittenLength;

			glGetShaderInfoLog(shaderObject, logLength, &logWrittenLength, &infoLog[0]);
			log += ": " + infoLog;

		}

		glDeleteShader(shaderObject);
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, log);
		return 0;
	}

	return shaderObject;
}


std::string kengine::getShaderType(GLuint shader_type)
{
	switch (shader_type)
	{
	case GL_VERTEX_SHADER:
		return "GL_VERTEX_SHADER";

	case GL_GEOMETRY_SHADER:
		return "GL_GEOMETRY_SHADER";

	case GL_TESS_EVALUATION_SHADER:
		return "GL_TESS_EVALUATION_SHADER";

	case GL_TESS_CONTROL_SHADER:
		return "GL_TESS_CONTROL_SHADER";

	case GL_COMPUTE_SHADER:
		return "GL_COMPUTE_SHADER";

	case GL_FRAGMENT_SHADER:
		return "GL_FRAGMENT_SHADER";

	default:
		return "";
	}
}


/*
*
*  kengine::GLSLprogram class - member class definition
*
*/
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
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create a shader program!");
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

		GLint status;
		glGetProgramiv(programID, GL_LINK_STATUS, &status);

		if (status == GL_FALSE)
		{
			std::string log = "It was not possible to link a shader program";

			GLint logLength;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

			if (logLength > 0)
			{
				std::string infoLog(static_cast<unsigned int>(logLength), ' ');
				GLsizei logWrittenLength;
				glGetProgramInfoLog(programID, logLength, &logWrittenLength, &infoLog[0]);
				log += ": " + infoLog;
			}

			K_DEBUG_OUTPUT(K_DEBUG_ERROR, log);
		}

		// clean up shader objects
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


void kengine::GLSLprogram::setUniform(std::string name, kengine::matrix& m)
{
	GLint location = glGetUniformLocation(programID, name.c_str());

	if (location == -1)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::matrix): " << name.c_str());
		return;
	}
	
	glUniformMatrix4fv(location, 1, GL_FALSE, m.value());
}


void kengine::GLSLprogram::setUniform(std::string name, int value)
{
	GLint location = glGetUniformLocation(programID, name.c_str());

	if (location == -1)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (int): " << name.c_str());
		return;
	}
	
	glUniform1i(location, value);
}


void kengine::GLSLprogram::setUniform(std::string name, const kengine::phong_light& l)
{
	GLint location;

	location = glGetUniformLocation(programID, (name + ".source_type").c_str());

	if (location != -1)
	{
		GLint i = static_cast<int>(l.type);
		glUniform1i(location, i);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".source_type").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".position").c_str());

	if (location != -1)
	{
		glUniform3f(location, l.position.x, l.position.y, l.position.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".position").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".La").c_str());

	if (location != -1)
	{
		glUniform3f(location, l.La.x, l.La.y, l.La.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".La").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".Ld").c_str());

	if (location != -1)
	{
		glUniform3f(location, l.Ld.x, l.Ld.y, l.Ld.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".Ld").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".Ls").c_str());

	if (location != -1)
	{
		glUniform3f(location, l.Ls.x, l.Ls.y, l.Ls.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".Ls").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".spot_direction").c_str());

	if (location != -1)
	{
		glUniform3f(location, l.spot_direction.x, l.spot_direction.y, l.spot_direction.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".spot_direction").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".spot_cutoff").c_str());

	if (location != -1)
	{
		glUniform1f(location, l.spot_cutoff);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".spot_cutoff").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".spot_exponent").c_str());

	if (location != -1)
	{
		glUniform1f(location, l.spot_exponent);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_light): " << (name + ".spot_exponent").c_str());
	}
}


void kengine::GLSLprogram::setUniform(std::string name, const kengine::phong_material& m)
{
	GLint location = glGetUniformLocation(programID, (name + ".Ka").c_str());

	if (location != -1)
	{
		glUniform3f(location, m.Ka.x, m.Ka.y, m.Ka.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_material): " << (name + ".Ka").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".Kd").c_str());

	if (location != -1)
	{
		glUniform3f(location, m.Kd.x, m.Kd.y, m.Kd.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_material): " << (name + ".Kd").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".Ks").c_str());

	if (location != -1)
	{
		glUniform3f(location, m.Ks.x, m.Ks.y, m.Ks.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_material): " << (name + ".Ks").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".shininess").c_str());

	if (location != -1)
	{
		glUniform1f(location, m.shininess);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::phong_material): " << (name + ".shininess").c_str());
	}
}


void kengine::GLSLprogram::setUniform(std::string name, const kengine::lighting_model& lm)
{
	GLint location = glGetUniformLocation(programID, (name + ".non_local_viewer").c_str());

	if (location != -1)
	{
		glUniform1i(location, lm.nonLocalViewer);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::lighting_model): " << (name + ".non_local_viewer").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".is_two_sided").c_str());

	if (location != -1)
	{
		glUniform1i(location, lm.twoSidedShading);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::lighting_model): " << (name + ".is_two_sided").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".flat_shading").c_str());

	if (location != -1)
	{
		glUniform1i(location, lm.flatShading);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::lighting_model): " << (name + ".flat_shading").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".model_type").c_str());

	if (location != -1)
	{
		GLint type = static_cast<GLint>(lm.modelType);
		glUniform1i(location, type);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::lighting_model): " << (name + ".model_type").c_str());
	}
}


void kengine::GLSLprogram::setUniform(std::string name, const kengine::toon_shading_info& tsi)
{
	GLint location = glGetUniformLocation(programID, (name + ".levels").c_str());

	if (location != -1)
	{
		glUniform1i(location, tsi.levels);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::toon_shading_info): " << (name + ".levels").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".scale_factor").c_str());

	if (location != -1)
	{
		glUniform1f(location, tsi.scaleFactor);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::toon_shading_info): " << (name + ".scale_factor").c_str());
	}
}


void kengine::GLSLprogram::setUniform(std::string name, const kengine::fog_info& fi)
{
	GLint location = glGetUniformLocation(programID, (name + ".max_distance").c_str());

	if (location != -1)
	{
		glUniform1f(location, fi.max_distance);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::fog_info): " << (name + ".max_distance").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".min_distance").c_str());

	if (location != -1)
	{
		glUniform1f(location, fi.min_distance);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::fog_info): " << (name + ".min_distance").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".color").c_str());

	if (location != -1)
	{
		glUniform3f(location, fi.color.x, fi.color.y, fi.color.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::fog_info): " << (name + ".color").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".enable").c_str());

	if (location != -1)
	{
		glUniform1i(location, fi.enable);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::fog_info): " << (name + ".enable").c_str());
	}

}


void kengine::GLSLprogram::setUniform(std::string name, const kengine::pbr_light& l)
{
	GLint location = glGetUniformLocation(programID, (name + ".position").c_str());

	if (location != -1)
	{
		glUniform3f(location, l.position.x, l.position.y, l.position.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::pbr_light): " << (name + ".position").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".L").c_str());

	if (location != -1)
	{
		glUniform3f(location, l.L.x, l.L.y, l.L.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::pbr_light): " << (name + ".L").c_str());
	}
}


void kengine::GLSLprogram::setUniform(std::string name, const kengine::pbr_material& m)
{
	GLint location = glGetUniformLocation(programID, (name + ".rough").c_str());

	if (location != -1)
	{
		glUniform1f(location, m.rough);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::pbr_material): " << (name + ".rough").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".metal").c_str());

	if (location != -1)
	{
		glUniform1i(location, m.metal);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::pbr_material): " << (name + ".metal").c_str());
	}

	location = glGetUniformLocation(programID, (name + ".color").c_str());

	if (location != -1)
	{
		glUniform3f(location, m.color.x, m.color.y, m.color.z);
	}
	else
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (kengine::pbr_material): " << (name + ".color").c_str());
	}
}


void kengine::GLSLprogram::setUniform(std::string name, GLsizei size, float* data)
{
	GLint location = glGetUniformLocation(programID, name.c_str());

	if (location == -1)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "Failed to set uniform (float*): " << name.c_str());
		return;
	}
	
	glUniform3fv(location, size, data);
}


void kengine::GLSLprogram::print() const
{
	std::cout << "> GLSLprogram object info [" << this << "]\n\n";

	// getting the list of active attribute variables
	GLint numAttribs = 0;
	glGetProgramInterfaceiv(programID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	if (numAttribs > 0)
	{
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

		std::cout << "  active attribute variables:\n";

		for (auto i = 0; i < numAttribs; i++)
		{
			GLint results[3];
			glGetProgramResourceiv(programID, GL_PROGRAM_INPUT, static_cast<GLuint>(i), 3, properties, 3, nullptr, results);

			GLint nameBuffSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBuffSize)];

			glGetProgramResourceName(programID, GL_PROGRAM_INPUT, static_cast<GLuint>(i), nameBuffSize, nullptr, name);

			std::cout
				<< "	- name: "		<< name					<< "\n"
				<< "	- type: "		<< getType(results[1])	<< "\n"
				<< "	- location: "	<< results[2]			<< "\n"
				<< std::endl;

			delete[] name;
		}
	}

	// getting a list of active uniform variables
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(programID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	if (numUniforms > 0)
	{
		std::cout << "\n  active uniform variables:\n";
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

		for (auto i = 0; i < numUniforms; i++)
		{
			GLint results[4];
			glGetProgramResourceiv(programID, GL_UNIFORM, static_cast<GLuint>(i), 4, properties, 4, nullptr, results);

			GLsizei nameBuffSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBuffSize)];

			glGetProgramResourceName(programID, GL_UNIFORM, static_cast<GLuint>(i), nameBuffSize, nullptr, name);

			std::cout
				<< "	- name: " << name << "\n"
				<< "	- type: " << getType(results[1]) << "\n"
				<< "	- location: " << results[2] << "\n"
				<< "	- block index : " << results[3] << "\n"
				<< std::endl;

			delete[] name;
		}

		std::cout << std::endl;
	}
}



void kengine::GLSLprogram::useProgram()
{
	if (programID)
		glUseProgram(programID);
}


std::string kengine::GLSLprogram::getType(GLint type) const
{
	switch (type)
	{
	case GL_INT:
		return "int";

	case GL_FLOAT:
		return "float";

	case GL_FLOAT_VEC2:
		return "vec2";

	case GL_FLOAT_VEC3:
		return "vec3";

	case GL_FLOAT_VEC4:
		return "vec4";

	case GL_FLOAT_MAT4:
		return "mat4";

	case GL_SAMPLER_2D:
		return "sampler2D";

	default:
		return "" + type;
	}
}


/*
*
*  kengine::mesh_node class - member class definition
*
*/
kengine::mesh_node::mesh_node()
	:
		max_size{ 1 },
		vao{ 0 },
		vbo{ 0 },
		count{ 0 },
		offset_to_modelview_buffer{ 0 }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::modelnode defult constructor - [" << this << "]")
}


kengine::mesh_node::mesh_node(const kengine::mesh& m)
	: max_size{ 1 }, vao{ 0 }, vbo{ 0 }, count{ 0 }, offset_to_modelview_buffer{ 0 }
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::modelnode constructor with argument - [" << this << "]")
	load(m, max_size);
}


kengine::mesh_node::~mesh_node()
{
	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::modelnode destructor - [" << this << "]");

	for (int i = 0; i < VBO_COUNT; i++)
	{
		glInvalidateBufferData(vbo[i]);
	}

	glDeleteBuffers(VBO_COUNT, vbo);
	glDeleteVertexArrays(1, &vao);
}


void kengine::mesh_node::load(const kengine::mesh& m, size_t size)
{
	if (m.getPosition()->attributeArray == nullptr)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to load mesh_node: mesh is empty.");
		return;
	}

	max_size = size;

	glCreateBuffers(VBO_COUNT, vbo);

	if (glGetError() == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create VBO : " << glGetError())
	}

	// ---------------------------------------------------------------------------
	//  storing vertex attributes
	// ---------------------------------------------------------------------------

	// store indices in VBO
	const kengine::vattrib<unsigned int>* indices = m.getIndices();

	if (indices->attributeArray != nullptr)
	{
		glNamedBufferStorage(vbo[0], static_cast<GLsizeiptr>(indices->getSizeinBytes()), indices->attributeArray, 0);

		GLuint error = glGetError();

		if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
		{
			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (index): " << glGetError())
		}

		count = static_cast<GLsizei>(m.getIndices()->getSize());
	}

	//  store vertex attributes in VBO
	const kengine::vattrib<float>* positions = m.getPosition();
	const kengine::vattrib<float>* colors = m.getColors();
	const kengine::vattrib<float>* texCoords = m.getTexCoords();
	const kengine::vattrib<float>* normals = m.getNormals();

	GLsizeiptr sizeInBytes = static_cast<GLsizeiptr>(positions->getSizeinBytes() + colors->getSizeinBytes() + texCoords->getSizeinBytes() + normals->getSizeinBytes() + (max_size * 16LL * sizeof(GLfloat)));
	glNamedBufferStorage(vbo[1], sizeInBytes, nullptr, GL_DYNAMIC_STORAGE_BIT);

	GLuint error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (vertex attributes): " << glGetError())
	}

	if (positions->attributeArray != nullptr)
		glNamedBufferSubData(vbo[1], 0, static_cast<GLsizeiptr>(positions->getSizeinBytes()), positions->attributeArray);

	if (colors->attributeArray != nullptr)
		glNamedBufferSubData(vbo[1], static_cast<GLsizeiptr>(positions->getSizeinBytes()), static_cast<GLsizeiptr>(colors->getSizeinBytes()), colors->attributeArray);

	if (texCoords->attributeArray != nullptr)
		glNamedBufferSubData(vbo[1], static_cast<GLsizeiptr>(positions->getSizeinBytes() + colors->getSizeinBytes()), static_cast<GLsizeiptr>(texCoords->getSizeinBytes()), texCoords->attributeArray);

	if(normals->attributeArray != nullptr)
		glNamedBufferSubData(vbo[1], static_cast<GLsizeiptr>(positions->getSizeinBytes() + colors->getSizeinBytes() + texCoords->getSizeinBytes()), static_cast<GLsizeiptr>(normals->getSizeinBytes()), normals->attributeArray);

	offset_to_modelview_buffer = static_cast<GLsizeiptr>(positions->getSizeinBytes() + colors->getSizeinBytes() + texCoords->getSizeinBytes() + normals->getSizeinBytes());

	GLfloat* modelviewData = new float[max_size * 16LL];

	for (size_t i = 0; i < max_size; i++)
	{
		modelviewData[i +  0] = 1.0f;
		modelviewData[i +  1] = 0.0f;
		modelviewData[i +  2] = 0.0f;
		modelviewData[i +  3] = 0.0f;
		modelviewData[i +  4] = 0.0f;
		modelviewData[i +  5] = 1.0f;
		modelviewData[i +  6] = 0.0f;
		modelviewData[i +  7] = 0.0f;
		modelviewData[i +  8] = 0.0f;
		modelviewData[i +  9] = 0.0f;
		modelviewData[i + 10] = 1.0f;
		modelviewData[i + 11] = 0.0f;
		modelviewData[i + 12] = 0.0f;
		modelviewData[i + 13] = 0.0f;
		modelviewData[i + 14] = 0.0f;
		modelviewData[i + 15] = 1.0f;
	}

	glNamedBufferSubData(vbo[1], offset_to_modelview_buffer, static_cast<GLsizeiptr>(size * 16LL * sizeof(GLfloat)), modelviewData);
	delete[] modelviewData;


	// ------------------------------------------------------------------------
	//  creating Vertex Array Object (VAO)
	// ------------------------------------------------------------------------
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


	// ------------------------------------------------------------------------
	//  shader plumbing
	// ------------------------------------------------------------------------
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	if (colors->attributeArray != nullptr)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)positions->getSizeinBytes());
	}

	if (texCoords->attributeArray != nullptr)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(positions->getSizeinBytes() + colors->getSizeinBytes()));
	}

	if (normals->attributeArray != nullptr)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(positions->getSizeinBytes() + colors->getSizeinBytes() + texCoords->getSizeinBytes()));
	}

	for (unsigned int i = 0; i < 4; i++)
	{
		glVertexAttribPointer(4UL + i, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizeiptr>(16 * sizeof(GLfloat)),
			(const GLvoid*)(offset_to_modelview_buffer + (sizeof(float) * 4 * i)));

		glEnableVertexAttribArray(4UL + i);
		glVertexAttribDivisor(4UL + i, 1);
	}

	if (indices->attributeArray == nullptr)
		count = static_cast<GLsizei>(positions->getSize());
}


void kengine::mesh_node::updateModelMatrix(size_t size, const float* data) const
{
	if (size > max_size)
		size = max_size;

	glNamedBufferSubData(vbo[1], offset_to_modelview_buffer, static_cast<GLsizeiptr>(size * 16LL * sizeof(GLfloat)), data);
}


void kengine::mesh_node::drawArrays() const
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, count);
}


void kengine::mesh_node::drawElements() const
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}



void kengine::mesh_node::drawArraysInstanced(GLsizei size) const
{
	glBindVertexArray(vao);
	glDrawArraysInstanced(GL_TRIANGLES, 0, count, size);
}


void kengine::mesh_node::drawElementsInstanced(GLsizei size) const
{
	glBindVertexArray(vao);
	glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, size);
}


// ------------------------------------------------------------------------
//  (!) kengine::instanced_uv_mesh_node class
// ------------------------------------------------------------------------
//kengine::instanced_uv_mesh_node::instanced_uv_mesh_node(const size_t size, const kengine::mesh& m)
//	: max_size{ size }, vao { 0 }, vbo{ { 0 } }
//{
//	//K_DEBUG_OUTPUT(K_DEBUG_WARNING, "kengine::batch constructor with argument - [" << this << "]")
//	GLuint error;
//
//	glCreateBuffers(TOTAL_VBO, vbo);
//
//	if (glGetError() == GL_INVALID_VALUE)
//	{
//		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create VBO : " << glGetError())
//	}
//
//	// store indices in VBO
//	const kengine::vattrib<unsigned int>* indices = m.getIndices();
//
//	if (indices->attributeArray != nullptr)
//	{
//		glNamedBufferStorage(vbo[0], static_cast<GLsizeiptr>(indices->getSizeinBytes()), indices->attributeArray, 0);
//
//		error = glGetError();
//
//		if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
//		{
//			K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (index): " << glGetError())
//		}
//
//		count = static_cast<GLsizei>(m.getIndices()->getSize());
//	}
//
//	// store vertex attributes in VBO
//	const kengine::vattrib<float>* positionPTR = m.getPosition();
//
//	GLsizeiptr sizeInBytes = static_cast<GLsizeiptr>(positionPTR->getSizeinBytes());
//	glNamedBufferStorage(vbo[1], sizeInBytes, nullptr, GL_DYNAMIC_STORAGE_BIT);
//
//	error = glGetError();
//
//	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
//	{
//		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to allocate space for VBO (vertex attributes): " << glGetError())
//	}
//
//	glNamedBufferSubData(vbo[1], 0, static_cast<GLsizeiptr>(positionPTR->getSizeinBytes()), positionPTR->attributeArray);
//
//	// store vertex attributes for texture coordinates and modelview matrix
//	GLsizeiptr matrixSize = static_cast<GLsizeiptr>(max_size * 16LL * sizeof(GLfloat));
//	GLsizeiptr uvSize = static_cast<GLsizeiptr>(max_size * (8 * sizeof(GLfloat)));
//	glNamedBufferStorage(vbo[2], matrixSize + uvSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
//
//	// creating VAO
//	glCreateVertexArrays(1, &vao);
//
//	if (glGetError() == GL_INVALID_VALUE)
//	{
//		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create vao: " << glGetError())
//	}
//
//	glBindVertexArray(vao);
//
//	if (glGetError() == GL_INVALID_OPERATION)
//	{
//		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to bind VAO : " << glGetError())
//	}
//
//	//  shader plumbing
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
//
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
//
//	glEnableVertexAttribArray(2);
//	glEnableVertexAttribArray(3);
//
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
//
//	for (unsigned int i = 0; i < 4; i++)
//	{
//		glVertexAttribPointer(3UL + i, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizeiptr>(16 * sizeof(GLfloat)), (const GLvoid*)(uvSize + sizeof(float) * 4 * i));
//		glEnableVertexAttribArray(3UL + i);
//		glVertexAttribDivisor(3UL + i, 1);
//	}
//}
//
//
//kengine::instanced_uv_mesh_node::~instanced_uv_mesh_node()
//{
//	glInvalidateBufferData(vbo[0]);
//	glInvalidateBufferData(vbo[1]);
//	glInvalidateBufferData(vbo[2]);
//	glDeleteBuffers(TOTAL_VBO, vbo);
//	glDeleteVertexArrays(1, &vao);
//}
//
//
//void kengine::instanced_uv_mesh_node::updateModelView(size_t size, float* modelview) const
//{
//	if (size > max_size)
//		size = max_size;
//
//	GLsizeiptr uvSize = static_cast<GLsizeiptr>(max_size * (8 * sizeof(float)));
//	glNamedBufferSubData(vbo[2], uvSize, static_cast<GLsizeiptr>(size * 16 * sizeof(GLfloat)), modelview);
//}
//
//
//void kengine::instanced_uv_mesh_node::updateUV(size_t size, float* uv) const
//{
//	if (size > max_size)
//		size = max_size;
//
//	glNamedBufferSubData(vbo[2], 0, static_cast<GLsizeiptr>(size * (8 * sizeof(float))), uv);
//}
//
//
//void kengine::instanced_uv_mesh_node::draw(int size) const
//{
//	if (size < 0)
//		size = 0;
//
//	glBindVertexArray(vao);
//	glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, size);
//}


/*
* 
*  kengine::prim_mesh_node class - member class definition
* 
*/
kengine::prim_mesh_node::prim_mesh_node()
	:
		max_size{ 0 },
		vao{ 0 },
		vbo{ 0 },
		mode{ 0 }
{
}


kengine::prim_mesh_node::prim_mesh_node(const size_t size, const PRIMITIVE_TYPE primType, const float* color)
	:
		max_size{ 0 },
		vao{ 0 },
		vbo{ 0 },
		mode{ 0 }
{
	load(size, primType, color);
}


kengine::prim_mesh_node::~prim_mesh_node()
{
	glInvalidateBufferData(vbo[0]);
	glDeleteBuffers(TOTAL_VBO, vbo);
	glDeleteVertexArrays(1, &vao);
}


void kengine::prim_mesh_node::load(const size_t size, const PRIMITIVE_TYPE primType, const float* color)
{
	if (!size)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "[prim_mesh_node mesh batch::load] It was not possible to load - size is: " << size);
		return;
	}

	max_size = size;

	glCreateBuffers(TOTAL_VBO, vbo);

	GLuint error = glGetError();

	if (error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "[primitive mesh batch::load] It was not possible to create VBO: " << glGetError());
	}

	GLsizeiptr coordsSize = static_cast<GLsizeiptr>(max_size * 3LL * sizeof(GLfloat));
	GLsizeiptr colorSize = static_cast<GLsizeiptr>(4LL * sizeof(GLfloat));

	glNamedBufferStorage(vbo[0], coordsSize + colorSize, nullptr, GL_DYNAMIC_STORAGE_BIT);

	error = glGetError();

	if (error == GL_OUT_OF_MEMORY || error == GL_INVALID_VALUE || error == GL_INVALID_OPERATION)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "[primitive mesh batch] It was not possible to allocate space for VBO: " << glGetError())
	}

	glNamedBufferSubData(vbo[0], coordsSize, colorSize, color);

	// creating VAO
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

	// shader plumbing
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)coordsSize);
	glVertexAttribDivisor(1, 1);

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


void kengine::prim_mesh_node::setPointSize(const float pointSize)
{
	glPointSize(pointSize);
}


void kengine::prim_mesh_node::setLineWidth(const float width)
{
	glLineWidth(width);
}


void kengine::prim_mesh_node::update(size_t size, float* data) const
{
	if (size > max_size)
		size = max_size;

	glNamedBufferSubData(vbo[0], 0, static_cast<GLsizeiptr>(size * 3ULL * sizeof(float)), data);
}


void kengine::prim_mesh_node::draw(int size) const
{
	if (size < 0)
	{
		return;
	}

	glBindVertexArray(vao);
	glDrawArrays(mode, 0, size);
}


/*
*
*  kengine::texture class
*
*/
kengine::texture::texture()
	: id{ 0 }, texUnit{ 0 }
{
}

kengine::texture::texture(const kengine::raw_img& img, GLuint textureUnit)
	: id{ 0 }, texUnit{ textureUnit }
{
	load(img, texUnit);
}


kengine::texture::~texture()
{
	glDeleteTextures(1, &id);

	GLenum error = glGetError();

	if (error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to delete texture object: " << error);
	}
}


void kengine::texture::load(const kengine::raw_img& img, GLuint textureUnit)
{
	load(img.getWidth(), img.getHeight(), img.getPixels(), textureUnit);
}


void kengine::texture::load(const int width, const int height, const unsigned char* pixels, GLuint textureUnit)
{
	texUnit = textureUnit;

	glCreateTextures(GL_TEXTURE_2D, 1, &id);

	GLenum error = glGetError();

	if (error == GL_INVALID_ENUM || error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to create texture object: " << error)
	}

	glTextureStorage2D(id, 1, GL_RGBA8, width, height);

	error = glGetError();

	if (error == GL_INVALID_OPERATION || error == GL_INVALID_ENUM || error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to storage texture object: " << error)
	}

	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	if (error == GL_INVALID_OPERATION || error == GL_INVALID_ENUM || error == GL_INVALID_VALUE)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to specify texture subimage: " << error)
	}

	glBindTextureUnit(texUnit, id);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//glBindTextureUnit(texUnit, 0);
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


/*
*
*  kengine::atlas class
*
*/
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


/*
*
*  kengine::renderingsystem class
*
*/
kengine::renderingsystem::renderingsystem(kengine::win32wrapper* w)
	: api{ w }
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


void kengine::renderingsystem::clearBuffers() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void kengine::renderingsystem::setViewport(int x, int y, int width, int height) const
{
	glViewport(x, y, width, height);
}