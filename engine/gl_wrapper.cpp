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

#include <gl_wrapper.hpp>
#include <logger.hpp>

#include <vector>
#include <fstream>
#include <cstring>

PFNGLCLEARBUFFERFVPROC glClearBufferfv = 0;
PFNGLCREATEBUFFERSPROC glCreateBuffers = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage = 0;
PFNGLBUFFERSTORAGEPROC glBufferStorage = 0;
PFNGLNAMEDBUFFERDATAPROC glNamedBufferData = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLISBUFFERPROC glIsBuffer = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLISVERTEXARRAYPROC glIsVertexArray = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib = 0;
PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib = 0;
PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat = 0;
PFNGLVERTEXARRAYVERTEXBUFFERSPROC glVertexArrayVertexBuffers = 0;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer = 0;
PFNGLVERTEXATTRIBFORMATPROC glVertexAtttribFormat = 0;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding = 0;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLGETSTRINGIPROC glGetStringi = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLDETACHSHADERPROC glDetachShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary = 0;
PFNGLPROGRAMBINARYPROC glProgramBinary = 0;
PFNGLSHADERBINARYPROC glShaderBinary = 0;
PFNGLSPECIALIZESHADERPROC glSpecializeShader = 0;
PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv = 0;
PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv = 0;
PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName = 0;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = 0;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = 0;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = 0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = 0;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = 0;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = 0;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = 0;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv = 0;
PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines = 0;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = 0;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = 0;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = 0;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f = 0;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram = 0;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = 0;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = 0;
PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl = 0;
PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup = 0;
PFNGLPOPDEBUGGROUPPROC glPopDebugGroup = 0;

bool kengine::getAllGLProcedures()
{
	glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)getGLFunctionAddress("glClearBufferfv");
	glCreateBuffers = (PFNGLCREATEBUFFERSPROC)getGLFunctionAddress("glCreateBuffers");
	glGenBuffers = (PFNGLGENBUFFERSPROC)getGLFunctionAddress("glGenBuffers");
	glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)getGLFunctionAddress("glNamedBufferStorage");
	glBufferStorage = (PFNGLBUFFERSTORAGEPROC)getGLFunctionAddress("glBufferStorage");
	glNamedBufferData = (PFNGLNAMEDBUFFERDATAPROC)getGLFunctionAddress("glNamedBufferData");
	glBufferData = (PFNGLBUFFERDATAPROC)getGLFunctionAddress("glBufferData");
	glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)getGLFunctionAddress("glNamedBufferSubData");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)getGLFunctionAddress("glBufferSubData");
	glBindBuffer = (PFNGLBINDBUFFERPROC)getGLFunctionAddress("glBindBuffer");
	glIsBuffer = (PFNGLISBUFFERPROC)getGLFunctionAddress("glIsBuffer");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)getGLFunctionAddress("glDeleteBuffers");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)getGLFunctionAddress("glGenVertexArrays");
	glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)getGLFunctionAddress("glCreateVertexArrays");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)getGLFunctionAddress("glBindVertexArray");
	glIsVertexArray = (PFNGLISVERTEXARRAYPROC)getGLFunctionAddress("glIsVertexArray");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)getGLFunctionAddress("glDeleteVertexArrays");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)getGLFunctionAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)getGLFunctionAddress("glEnableVertexAttribArray");
	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)getGLFunctionAddress("glDisableVertexAttribArray");
	glEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC)getGLFunctionAddress("glEnableVertexArrayAttrib");
	glDisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIBPROC)getGLFunctionAddress("glDisableVertexArrayAttrib");
	glVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC)getGLFunctionAddress("glVertexArrayAttribFormat");
	glVertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERSPROC)getGLFunctionAddress("glVertexArrayVertexBuffers");
	glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)getGLFunctionAddress("glBindVertexBuffer");
	glVertexAtttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)getGLFunctionAddress("glVertexAtttribFormat");
	glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)getGLFunctionAddress("glVertexAttribBinding");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)getGLFunctionAddress("glBindAttribLocation");
	glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)getGLFunctionAddress("glDrawArraysInstancedBaseInstance");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)getGLFunctionAddress("glUniformMatrix4fv");
	glGetStringi = (PFNGLGETSTRINGIPROC)getGLFunctionAddress("glGetStringi");
	glCreateShader = (PFNGLCREATESHADERPROC)getGLFunctionAddress("glCreateShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)getGLFunctionAddress("glDeleteShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)getGLFunctionAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)getGLFunctionAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)getGLFunctionAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)getGLFunctionAddress("glGetShaderInfoLog");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)getGLFunctionAddress("glCreateProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)getGLFunctionAddress("glDeleteProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)getGLFunctionAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)getGLFunctionAddress("glDetachShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)getGLFunctionAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)getGLFunctionAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)getGLFunctionAddress("glGetProgramInfoLog");
	glUseProgram = (PFNGLUSEPROGRAMPROC)getGLFunctionAddress("glUseProgram");
	glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)getGLFunctionAddress("glGetProgramBinary");
	glProgramBinary = (PFNGLPROGRAMBINARYPROC)getGLFunctionAddress("glProgramBinary");
	glShaderBinary = (PFNGLSHADERBINARYPROC)getGLFunctionAddress("glShaderBinary");
	glSpecializeShader = (PFNGLSPECIALIZESHADERPROC)getGLFunctionAddress("glSpecializeShader");
	glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)getGLFunctionAddress("glGetProgramInterfaceiv");
	glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)getGLFunctionAddress("glGetProgramResourceiv");
	glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)getGLFunctionAddress("glGetProgramResourceName");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)getGLFunctionAddress("glGetActiveAttrib");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)getGLFunctionAddress("glGetAttribLocation");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)getGLFunctionAddress("glGetUniformLocation");
	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)getGLFunctionAddress("glGetActiveUniform");
	glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)getGLFunctionAddress("glGetActiveUniformName");
	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)getGLFunctionAddress("glGetUniformBlockIndex");
	glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)getGLFunctionAddress("glGetActiveUniformBlockiv");
	glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)getGLFunctionAddress("glGetUniformIndices");
	glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)getGLFunctionAddress("glGetActiveUniformsiv");
	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)getGLFunctionAddress("glBindBufferBase");
	glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)getGLFunctionAddress("glCreateShaderProgramv");
	glCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC)getGLFunctionAddress("glCreateProgramPipelines");
	glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)getGLFunctionAddress("glDeleteProgramPipelines");
	glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)getGLFunctionAddress("glUseProgramStages");
	glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)getGLFunctionAddress("glBindProgramPipeline");
	glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)getGLFunctionAddress("glProgramUniform3f");
	glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)getGLFunctionAddress("glActiveShaderProgram");
	glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)getGLFunctionAddress("glProgramParameteri");
	glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)getGLFunctionAddress("glDebugMessageCallback");
	glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)getGLFunctionAddress("glDebugMessageControl");
	glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)getGLFunctionAddress("glPushDebugGroup");
	glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)getGLFunctionAddress("glPopDebugGroup");

	if (glClearBufferfv == nullptr ||
		glCreateBuffers == nullptr ||
		glGenBuffers == nullptr ||
		glNamedBufferStorage == nullptr ||
		glBufferStorage == nullptr ||
		glNamedBufferData == nullptr ||
		glBufferData == nullptr ||
		glNamedBufferSubData == nullptr ||
		glBufferSubData == nullptr ||
		glBindBuffer == nullptr ||
		glIsBuffer == nullptr ||
		glDeleteBuffers == nullptr ||
		glGenVertexArrays == nullptr ||
		glCreateVertexArrays == nullptr ||
		glBindVertexArray == nullptr ||
		glIsVertexArray == nullptr ||
		glDeleteVertexArrays == nullptr ||
		glVertexAttribPointer == nullptr ||
		glEnableVertexAttribArray == nullptr ||
		glDisableVertexAttribArray == nullptr ||
		glEnableVertexArrayAttrib == nullptr ||
		glDisableVertexArrayAttrib == nullptr ||
		glVertexArrayAttribFormat == nullptr ||
		glVertexArrayVertexBuffers == nullptr ||
		glBindVertexBuffer == nullptr ||
		glVertexAtttribFormat == nullptr ||
		glVertexAttribBinding == nullptr ||
		glBindAttribLocation == nullptr ||
		glDrawArraysInstancedBaseInstance == nullptr ||
		glUniformMatrix4fv == nullptr ||
		glGetStringi == nullptr ||
		glCreateShader == nullptr ||
		glDeleteShader == nullptr ||
		glShaderSource == nullptr ||
		glCompileShader == nullptr ||
		glGetShaderiv == nullptr ||
		glGetShaderInfoLog == nullptr ||
		glCreateProgram == nullptr ||
		glDeleteProgram == nullptr ||
		glAttachShader == nullptr ||
		glDetachShader == nullptr ||
		glLinkProgram == nullptr ||
		glGetProgramiv == nullptr ||
		glGetProgramInfoLog == nullptr ||
		glUseProgram == nullptr ||
		glGetProgramBinary == nullptr ||
		glProgramBinary == nullptr ||
		glShaderBinary == nullptr ||
		glSpecializeShader == nullptr ||
		glGetProgramInterfaceiv == nullptr ||
		glGetProgramResourceiv == nullptr ||
		glGetProgramResourceName == nullptr ||
		glGetActiveAttrib == nullptr ||
		glGetAttribLocation == nullptr ||
		glGetUniformLocation == nullptr ||
		glGetActiveUniform == nullptr ||
		glGetActiveUniformName == nullptr ||
		glGetUniformBlockIndex == nullptr ||
		glGetActiveUniformBlockiv == nullptr ||
		glGetUniformIndices == nullptr ||
		glGetActiveUniformsiv == nullptr ||
		glBindBufferBase == nullptr ||
		glCreateShaderProgramv == nullptr ||
		glCreateProgramPipelines == nullptr ||
		glDeleteProgramPipelines == nullptr ||
		glUseProgramStages == nullptr ||
		glBindProgramPipeline == nullptr ||
		glProgramUniform3f == nullptr ||
		glActiveShaderProgram == nullptr ||
		glProgramParameteri == nullptr ||
		glDebugMessageCallback == nullptr ||
		glDebugMessageControl == nullptr ||
		glPushDebugGroup == nullptr ||
		glPopDebugGroup == nullptr)
	{
		return false;
	}

	return true;
}

/*
	GLSLprogram class - member class definition
*/

kengine::GLSLprogram::~GLSLprogram()
{
	glDeleteProgram(programID);
}

bool kengine::GLSLprogram::loadShaders(const ShaderInfo* shaderInfo)
{
	bool ret = false;

	if (shaderInfo)
	{
		if (programID) {
			glDeleteProgram(programID);
			uniformMap.clear();
		}

		GLuint shaders[6] = { 0 };

		for (int index = 0; shaderInfo[index].type != GL_NONE && index < 6; index++) {
			shaders[index] = compileShader(shaderInfo[index].type, shaderInfo[index].filename);
		}

		programID = glCreateProgram();

		if (programID) {
			ret = true;

			for (int index = 0; index < 6; index++) {
				if (shaders[index])
					glAttachShader(programID, shaders[index]);
			}

			glLinkProgram(programID);

			GLint status;
			glGetProgramiv(programID, GL_LINK_STATUS, &status);

			if (status == GL_TRUE) {
				// query the location of a uniform variable
				GLint numUniforms = 0;
				glGetProgramInterfaceiv(programID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
				GLenum properties[] = { GL_NAME_LENGTH, GL_LOCATION };

				for (GLint i = 0; i < numUniforms; i++) {
					GLint results[2];
					glGetProgramResourceiv(programID, GL_UNIFORM, static_cast<GLuint>(i), 2, properties, 2, nullptr, results);
					GLint nameBufferSize = results[0] + 1;
					char* name = new char[static_cast<size_t>(nameBufferSize)];
					glGetProgramResourceName(programID, GL_UNIFORM, static_cast<GLuint>(i), nameBufferSize, nullptr, name);
					uniformMap[name] = results[1];
				}
			} else {
				std::string log = "It was not possible to link a shader program";

				GLint logLength;
				glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

				if (logLength > 0) {
					std::string infoLog(static_cast<unsigned int>(logLength), ' ');
					GLsizei logWrittenLength;
					glGetProgramInfoLog(programID, logLength, &logWrittenLength, &infoLog[0]);
					log += ": " + infoLog;
				}

				K_LOG_OUTPUT_RAW(log);
				ret = false;
			}
		}
		else
			K_LOG_OUTPUT_RAW("It was not possible to create a shader program!");

		// clean up shader objects
		for (int index = 0; index < 6; index++) {
			if (shaders[index]) {
				glDetachShader(programID, shaders[index]);
				glDeleteShader(shaders[index]);
			}
		}
	}

	return ret;
}

void kengine::GLSLprogram::useProgram()
{
	glUseProgram(programID);
}

void kengine::GLSLprogram::saveBinary(const std::string& name)
{
	if (programID == 0)
		return;

	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	if (formats < 1) {
		K_LOG_OUTPUT_RAW("Driver does not support any binary formats.");
		return;
	}

	GLint length = 0;
	glGetProgramiv(programID, GL_PROGRAM_BINARY_LENGTH, &length);

	std::vector<GLubyte> buffer(static_cast<size_t>(length));
	glGetProgramBinary(programID, length, nullptr, &binaryFormat, buffer.data());

	std::ofstream shaderBinary(name.c_str(), std::ios::binary);
	shaderBinary.write(reinterpret_cast<char*>(buffer.data()), length);
	shaderBinary.close();
}

void kengine::GLSLprogram::loadBinary(const std::string& name)
{
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	if (formats < 1) {
		K_LOG_OUTPUT_RAW("Driver does not support any binary formats.");
		return;
	}

	if (programID)
	{
		uniformMap.clear();
		glDeleteProgram(programID);
	}

	programID = glCreateProgram();

	std::ifstream shaderBinary(name, std::ios::binary);
	std::istreambuf_iterator<char> iter(shaderBinary), endIter;
	std::vector<char> buffer(iter, endIter);
	shaderBinary.close();

	glProgramBinary(programID, binaryFormat, buffer.data(), static_cast<GLsizei>(buffer.size()));

	GLint status;
	glGetProgramiv(programID, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		std::string log = "It was not possible to link a binary shader program";

		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::string infoLog(static_cast<unsigned int>(logLength), ' ');
			GLsizei logWrittenLength;
			glGetProgramInfoLog(programID, logLength, &logWrittenLength, &infoLog[0]);
			log += ": " + infoLog;
		}

		K_LOG_OUTPUT_RAW(log);
	}
}

void kengine::GLSLprogram::setUniform(std::string name, kengine::matrix& m)
{
	/*
		It is inefficient to query the location of a uniform variable in each frame [Wolf, 2018]
	*/
	// GLint location = glGetUniformLocation(programID, name.c_str());

	GLint location = uniformMap[name];

	if (location == -1) {
		K_LOG_OUTPUT_RAW("Failed to set uniform (kengine::matrix): " << name);
		return;
	}

	glUniformMatrix4fv(location, 1, GL_FALSE, m.value());
}

void kengine::GLSLprogram::print() const
{
	K_LOG_OUTPUT_RAW("> kengine::GLSLprogram object info [" << this << "]\n\n");
	
	// getting the list of active attribute variables
	GLint numAttribs = 0;
	glGetProgramInterfaceiv(programID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);
	
	if (numAttribs > 0)
	{
		K_LOG_OUTPUT_RAW("  active attribute variables:\n");

		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

		for (auto i = 0; i < numAttribs; i++)
		{
			GLint results[3];
			glGetProgramResourceiv(programID, GL_PROGRAM_INPUT, static_cast<GLuint>(i), 3, properties, 3, nullptr, results);

			GLint nameBufferSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBufferSize)];

			glGetProgramResourceName(programID, GL_PROGRAM_INPUT, static_cast<GLuint>(i), nameBufferSize, nullptr, name);

			K_LOG_OUTPUT_RAW("	- name: " << name << "\n" << "	- type: " << getGLSLType(results[1]) << "\n" << "	- location: " << results[2] << "\n");
			delete[] name;
		}
	}

	// getting a list of active uniform variables
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(programID, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	if (numUniforms > 0)
	{
		K_LOG_OUTPUT_RAW("\n  active uniform variables:\n");
		
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

		for (auto i = 0; i < numUniforms; i++)
		{
			GLint results[4];
			glGetProgramResourceiv(programID, GL_UNIFORM, static_cast<GLuint>(i), 4, properties, 4, nullptr, results);

			GLsizei nameBuffSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBuffSize)];

			glGetProgramResourceName(programID, GL_UNIFORM, static_cast<GLuint>(i), nameBuffSize, nullptr, name);

			K_LOG_OUTPUT_RAW(
				"	- name: " << name << "\n" <<
				"	- type: " << getGLSLType(results[1]) << "\n" <<
				"	- location: " << results[2] << "\n" <<
				"	- block index : " << results[3] << "\n");

			delete[] name;
		}

		K_LOG_OUTPUT_RAW("\n");
	}
}

/*
	kengine::mesh_node class - member class definition
*/

void kengine::mesh_node::load(kengine::mesh& m, size_t size)
{
	clear(); // if this mesh_node was already loaded, it must be cleaned before
	//max_size = size;

	glGenBuffers(MAX_VBO, m_vbo);

	GLsizeiptr totalSizeInBytes = static_cast<GLsizeiptr>(m.getSizeInBytes());
	const float* const data = m.getInterleavedData();

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferStorage(GL_ARRAY_BUFFER, totalSizeInBytes, data, 0);

	//if (m.m_indices.attributeArray != nullptr) {
	//	glNamedBufferStorage(vbo[2], static_cast<GLsizeiptr>(m.m_indices.getSizeInBytes()), m.m_indices.attributeArray, GL_DYNAMIC_STORAGE_BIT);
	//	countElement = static_cast<GLsizei>(m.m_indices.arraySize);
	//}

	//if (hasModelMatrix)
	//{
	//	GLsizeiptr modelMatrix_size = static_cast<GLsizeiptr>(max_size * 16LL * sizeof(GLfloat));
	//	glNamedBufferStorage(vbo[1], modelMatrix_size, nullptr, GL_DYNAMIC_STORAGE_BIT);

	//	GLfloat* modelviewData = new float[max_size * 16LL];

	//	for (size_t i = 0; i < max_size; i++)
	//	{
	//		modelviewData[0 + (i * 16)] = 1.0f;
	//		modelviewData[1 + (i * 16)] = 0.0f;
	//		modelviewData[2 + (i * 16)] = 0.0f;
	//		modelviewData[3 + (i * 16)] = 0.0f;
	//		modelviewData[4 + (i * 16)] = 0.0f;
	//		modelviewData[5 + (i * 16)] = 1.0f;
	//		modelviewData[6 + (i * 16)] = 0.0f;
	//		modelviewData[7 + (i * 16)] = 0.0f;
	//		modelviewData[8 + (i * 16)] = 0.0f;
	//		modelviewData[9 + (i * 16)] = 0.0f;
	//		modelviewData[10 + (i * 16)] = 1.0f;
	//		modelviewData[11 + (i * 16)] = 0.0f;
	//		modelviewData[12 + (i * 16)] = 0.0f;
	//		modelviewData[13 + (i * 16)] = 0.0f;
	//		modelviewData[14 + (i * 16)] = 0.0f;
	//		modelviewData[15 + (i * 16)] = 1.0f;
	//	}

	//	glNamedBufferSubData(vbo[1], 0, modelMatrix_size, modelviewData);
	//	delete[] modelviewData;
	//}

	/*
		Creating vertex array object (VAO)
	*/

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	/*
		shader plumbing

		Note:
			Note that the "type" parameter for glVertexAttribPointer will be converted to floating-point by OpenGL in order to load it into floating-point vertex attributes.
			The way this conversion is performed is controlled by the normalize parameter.
			When normalize is GL_FALSE, integer data is simply typecast into floating-point format before being passed to the vertex shader.
			When normalize is GL_TRUE, the data is normalized before being passed to the vertex shader.
			To do this, OpenGL divides each element by a fixed constant that depends on the incoming data type.
	*/

	//if (m.m_indices.attributeArray != nullptr)
	//{
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[2]);
	//}

	
	/*
		Mapping the vertex data stored in m_vbo[0] to the vertex attributes declared in vertex shader
	*/

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	size_t offset = 0;
	m_count = static_cast<GLsizei>(m.m_vattributesMap[0].arraySize);

	for (auto it : m.m_vattributesMap) {
		auto location = it.first;

		glEnableVertexAttribArray(static_cast<GLuint>(location));

		glVertexAttribPointer(
			static_cast<GLuint>(location),
			static_cast<GLint>(it.second.count),
			GL_FLOAT,
			GL_FALSE,
			static_cast<GLsizei>(m.m_interleavedStride),
			(const GLvoid*)offset);

		offset += it.second.count * sizeof(float);
		
	}

	//for (GLuint location = 0; location < m.m_bitset.size(); location++)
	//{
	//	if (m.m_bitset[location])
	//	{
	//		glEnableVertexAttribArray(location);
	//		glVertexAttribPointer(location, static_cast<GLint>(m.m_vattributes[location].count), GL_FLOAT, GL_FALSE, static_cast<GLsizei>(m.m_interleavedStride * sizeof(float)), (const GLvoid*)m.m_interleavedOffsets[location]);
	//		countArray = static_cast<GLsizei>(m.m_vattributes[location].getSize());
	//	}
	//}

	//if (hasModelMatrix)
	//{
	//	// map index for model matrix
	//	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//	for (unsigned int i = 0; i < 4; i++)
	//	{
	//		glVertexAttribPointer(3UL + i, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizeiptr>(16 * sizeof(GLfloat)), (const GLvoid*)((sizeof(GLfloat) * 4 * i)));
	//		glEnableVertexAttribArray(3UL + i);
	//		glVertexAttribDivisor(3UL + i, 1);
	//	}
	//}
}

void kengine::mesh_node::clear()
{
	glDeleteBuffers(MAX_VBO, m_vbo);
	glDeleteVertexArrays(1, &m_vao);
	m_count = 0;

	//max_size = 0;
	//countElement = 0;
}

void kengine::mesh_node::drawArrays() const
{
	glBindVertexArray(m_vao);
	glDrawArrays(m_mode, 0, m_count);
}

/*
	Helper function to compile GLSL shader
*/
GLuint kengine::compileShader(GLuint shaderType, std::string filename)
{
	GLuint shaderObject = glCreateShader(shaderType);

	if (!shaderObject) {
		K_LOG_OUTPUT_RAW("It was not possible to create [" << getShaderType(shaderType) << "] shader!");
		return 0;
	}

	std::string sourceString = readFromFile(filename);

	if (sourceString[0] == '\0') {
		K_LOG_OUTPUT_RAW("This file " << filename << " cannot be opened.")
		glDeleteShader(shaderObject);
		return 0;
	}

	const GLchar* source = sourceString.c_str();

	glShaderSource(shaderObject, 1, &source, nullptr);
	glCompileShader(shaderObject);

	GLint status = 0;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		std::string log = "It was not possible to compile a shader [" + getShaderType(shaderType) + "]";

		GLint logLength;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::string infoLog(static_cast<unsigned int>(logLength), ' ');
			GLsizei logWrittenLength;

			glGetShaderInfoLog(shaderObject, logLength, &logWrittenLength, &infoLog[0]);
			log += ": " + infoLog;
		}

		glDeleteShader(shaderObject);
		K_LOG_OUTPUT_RAW(log);
		return 0;
	}

	return shaderObject;
}


GLuint kengine::compileSPIRVShader(GLuint shaderType, const std::string& name)
{
	GLuint shaderObject = glCreateShader(shaderType);

	std::ifstream shaderBinary(name, std::ios::binary);
	std::istreambuf_iterator<char> startIt(shaderBinary), endIt;
	std::vector<char> buffer(startIt, endIt);
	shaderBinary.close();

	glShaderBinary(1, &shaderObject, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(), static_cast<GLsizei>(buffer.size()));
	glSpecializeShader(shaderObject, "main", 0, 0, 0);

	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		std::string log = "It was not possible to compile a SPIRV shader";

		GLint logLength;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::string infoLog(static_cast<unsigned int>(logLength), ' ');
			GLsizei logWrittenLength;
			glGetProgramInfoLog(shaderObject, logLength, &logWrittenLength, &infoLog[0]);
			log += ": " + infoLog;
		}

		glDeleteShader(shaderObject);
		K_LOG_OUTPUT_RAW(log);
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

std::string kengine::getGLSLType(GLint type)
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

std::string kengine::readFromFile(std::string filename)
{
	std::ifstream filestream(filename, std::ios::in | std::ios::binary);

	if (!filestream)
		return "";

	filestream.seekg(0, std::ios::end);
	std::streamoff fileSize = filestream.tellg();
	filestream.seekg(0, std::ios::beg);
	size_t size = static_cast<size_t>(fileSize) + 1; // including space for '/0'

	char* source = new char[size];
	memset(source, 0, size);
	filestream.read(reinterpret_cast<char*>(source), fileSize);

	std::string s = source;
	delete[] source;
	return s;
}

/*
	OpenGL callback function for debug messages
*/
void kengine::debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param)
{
	std::string messageString = "> [opengl debug message]: ";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		messageString += "\n\t- source: GL_DEBUG_SOURCE_API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		messageString += "\n\t- source: GL_DEBUG_SOURCE_WINDOW_SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		messageString += "\n\t- source: GL_DEBUG_SOURCE_SHADER_COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		messageString += "\n\t- source: L_DEBUG_SOURCE_THIRD_PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		messageString += "\n\t- source: GL_DEBUG_SOURCE_APPLICATION";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		messageString += "\n\t- source: GL_DEBUG_SOURCE_OTHER";
		break;
	}

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		messageString += "\n\t- type: GL_DEBUG_TYPE_ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		messageString += "\n\t- type: GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		messageString += "\n\t- type: GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		messageString += "\n\t- type: GL_DEBUG_TYPE_PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		messageString += "\n\t- type: GL_DEBUG_TYPE_PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_MARKER:
		messageString += "\n\t- type: GL_DEBUG_TYPE_MARKER";
		break;

	case GL_DEBUG_TYPE_PUSH_GROUP:
		messageString += "\n\t- type: GL_DEBUG_TYPE_PUSH_GROUP";
		break;

	case GL_DEBUG_TYPE_POP_GROUP:
		messageString += "\n\t- type: GL_DEBUG_TYPE_POP_GROUP";
		break;

	case GL_DEBUG_TYPE_OTHER:
		messageString += "\n\t- type: GL_DEBUG_TYPE_OTHER";
		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		messageString += "\n\t- severity: GL_DEBUG_SEVERITY_HIGH";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		messageString += "\n\t- severity: GL_DEBUG_SEVERITY_MEDIUM";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		messageString += "\n\t- severity: GL_DEBUG_SEVERITY_LOW";
		break;
		//return;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		messageString += "\n\t- severity: GL_DEBUG_SEVERITY_NOTIFICATION";
		return;
	}

	messageString += "\n\t- id: " + std::to_string(id);
	std::cout << messageString << "\n\n\t" << message << "\n" << std::endl;
}