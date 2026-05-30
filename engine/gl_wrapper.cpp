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

#include <gl_wrapper.hpp>
#include <logger.hpp>

#include <vector>
#include <fstream>
#include <cstring>
#include <sstream>
#include <cassert>
#include <iomanip>

/*
	OpenGL extension: GLSL shaders
*/

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
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = 0;
PFNGLUNIFORM1FPROC glUniform1f = 0;
PFNGLUNIFORM3FPROC glUniform3f = 0;
PFNGLUNIFORM4FPROC glUniform4f = 0;
PFNGLUNIFORM4FVPROC glUniform4fv = 0;
PFNGLUNIFORM1IPROC glUniform1i = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLBINDBUFFERBASEPROC glBindBufferBase = 0;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri = 0;
PFNGLGETACTIVEUNIFORMNAMEPROC glGetActiveUniformName = 0;
PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex = 0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC glGetActiveUniformBlockiv = 0;
PFNGLGETUNIFORMINDICESPROC glGetUniformIndices = 0;
PFNGLGETACTIVEUNIFORMSIVPROC glGetActiveUniformsiv = 0;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary = 0;
PFNGLPROGRAMBINARYPROC glProgramBinary = 0;
PFNGLSHADERBINARYPROC glShaderBinary = 0;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv = 0;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages = 0;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline = 0;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f = 0;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram = 0;
PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv = 0;
PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv = 0;
PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName = 0;
PFNGLCREATEPROGRAMPIPELINESPROC glCreateProgramPipelines = 0;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines = 0;
PFNGLSPECIALIZESHADERPROC glSpecializeShader = 0;

/*
	OpenGL extension: buffers objects
*/

PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLISBUFFERPROC glIsBuffer = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
PFNGLGETBUFFERSUBDATAPROC glGetBufferSubData = 0;
PFNGLMAPBUFFERPROC glMapBuffer = 0;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = 0;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange = 0;
PFNGLCOPYBUFFERSUBDATAPROC glCopyBufferSubData = 0;
PFNGLCLEARBUFFERDATAPROC glClearBufferData = 0;
PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData = 0;
PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData = 0;
PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData = 0;
PFNGLBUFFERSTORAGEPROC glBufferStorage = 0;
PFNGLCREATEBUFFERSPROC glCreateBuffers = 0;
PFNGLNAMEDBUFFERDATAPROC glNamedBufferData = 0;
PFNGLNAMEDBUFFERSTORAGEPROC glNamedBufferStorage = 0;
PFNGLNAMEDBUFFERSUBDATAPROC glNamedBufferSubData = 0;
PFNGLCLEARNAMEDBUFFERDATAPROC glClearNamedBufferData = 0;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC glClearNamedBufferSubData = 0;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC glCopyNamedBufferSubData = 0;
PFNGLGETNAMEDBUFFERSUBDATAPROC glGetNamedBufferSubData = 0;
PFNGLMAPNAMEDBUFFERPROC glMapNamedBuffer = 0;
PFNGLUNMAPNAMEDBUFFERPROC glUnmapNamedBuffer = 0;
PFNGLMAPNAMEDBUFFERRANGEPROC glMapNamedBufferRange = 0;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC glFlushMappedNamedBufferRange = 0;

/*
	OpenGL extension: Vertex Array Objects
*/

PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation = 0;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = 0;
PFNGLPROVOKINGVERTEXPROC glProvokingVertex = 0;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = 0;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer = 0;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer = 0;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat = 0;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding = 0;
PFNGLCREATEVERTEXARRAYSPROC glCreateVertexArrays = 0;

/*
	OpenGL extensions: OpenGL drawing comands
*/
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = 0;
PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced = 0;

/*
	OpenGL extension: texture objects
*/
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = 0;
PFNGLCREATETEXTURESPROC glCreateTextures = 0;

/*
	OpenGL extension: OpenGL debug messages
*/
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = 0;
PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl = 0;
PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup = 0;
PFNGLPOPDEBUGGROUPPROC glPopDebugGroup = 0;

/*
	OpenGL extension: transform feedback objects
*/
PFNGLGENTRANSFORMFEEDBACKSPROC glGenTransformFeedbacks = 0;
PFNGLBINDTRANSFORMFEEDBACKPROC glBindTransformFeedback = 0;
PFNGLISTRANSFORMFEEDBACKPROC glIsTransformFeedback = 0;
PFNGLDELETETRANSFORMFEEDBACKSPROC glDeleteTransformFeedbacks = 0;
PFNGLCREATETRANSFORMFEEDBACKSPROC glCreateTransformFeedbacks = 0;
PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC glTransformFeedbackBufferBase = 0;
PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC glTransformFeedbackBufferRange = 0;

/*
*/
PFNGLDEPTHRANGEFPROC glDepthRangef = 0;
PFNGLBINDBUFFERSRANGEPROC glBindBuffersRange = 0;

//
PFNGLCLEARBUFFERFVPROC glClearBufferfv = 0;
PFNGLISVERTEXARRAYPROC glIsVertexArray = 0;
PFNGLENABLEVERTEXARRAYATTRIBPROC glEnableVertexArrayAttrib = 0;
PFNGLDISABLEVERTEXARRAYATTRIBPROC glDisableVertexArrayAttrib = 0;
PFNGLVERTEXARRAYATTRIBFORMATPROC glVertexArrayAttribFormat = 0;
PFNGLVERTEXARRAYVERTEXBUFFERSPROC glVertexArrayVertexBuffers = 0;
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance = 0;
PFNGLGETSTRINGIPROC glGetStringi = 0;
PFNGLPRIMITIVERESTARTINDEXPROC glPrimitiveRestartIndex = 0;
PFNGLTEXSTORAGE1DPROC glTexStorage1D = 0;
PFNGLTEXTURESTORAGE1DPROC glTextureStorage1D = 0;
PFNGLTEXSTORAGE2DPROC glTexStorage2D = 0;
PFNGLTEXTURESTORAGE2DPROC glTextureStorage2D = 0;
PFNGLTEXSTORAGE3DPROC glTexStorage3D = 0;
PFNGLTEXTURESTORAGE3DPROC glTextureStorage3D = 0;
PFNGLTEXTURESUBIMAGE1DPROC glTextureSubImage1D = 0;
PFNGLTEXTURESUBIMAGE2DPROC glTextureSubImage2D = 0;
//PFNGLTEXSUBIMAGE3DPROC glTexSubImage3D = 0;
PFNGLTEXTURESUBIMAGE3DPROC glTextureSubImage3D = 0;
PFNGLBINDTEXTUREUNITPROC glBindTextureUnit = 0;
PFNGLTEXBUFFERPROC glTexBuffer = 0;
PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding = 0;

#if defined(_WIN32)
	PFNGLCLIENTACTIVETEXTUREPROC glActiveTexture = 0;
#endif

bool kengine::getGLProcedures()
{
	glCreateShader = (PFNGLCREATESHADERPROC)getGLFunctionAddress("glCreateShader");
	if (glCreateShader == nullptr) return false;

	glDeleteShader = (PFNGLDELETESHADERPROC)getGLFunctionAddress("glDeleteShader");
	if (glDeleteShader == nullptr) return false;

	glShaderSource = (PFNGLSHADERSOURCEPROC)getGLFunctionAddress("glShaderSource");
	if (glShaderSource == nullptr) return false;

	glCompileShader = (PFNGLCOMPILESHADERPROC)getGLFunctionAddress("glCompileShader");
	if (glCompileShader == nullptr) return false;

	glGetShaderiv = (PFNGLGETSHADERIVPROC)getGLFunctionAddress("glGetShaderiv");
	if (glGetShaderiv == nullptr) return false;

	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)getGLFunctionAddress("glGetShaderInfoLog");
	if (glGetShaderInfoLog == nullptr) return false;

	glCreateProgram = (PFNGLCREATEPROGRAMPROC)getGLFunctionAddress("glCreateProgram");
	if (glCreateProgram == nullptr) return false;

	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)getGLFunctionAddress("glDeleteProgram");
	if (glDeleteProgram == nullptr) return false;

	glAttachShader = (PFNGLATTACHSHADERPROC)getGLFunctionAddress("glAttachShader");
	if (glAttachShader == nullptr) return false;

	glDetachShader = (PFNGLDETACHSHADERPROC)getGLFunctionAddress("glDetachShader");
	if (glDetachShader == nullptr) return false;

	glLinkProgram = (PFNGLLINKPROGRAMPROC)getGLFunctionAddress("glLinkProgram");
	if (glLinkProgram == nullptr) return false;

	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)getGLFunctionAddress("glGetProgramiv");
	if (glGetProgramiv == nullptr) return false;

	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)getGLFunctionAddress("glGetProgramInfoLog");
	if (glGetProgramInfoLog == nullptr) return false;

	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)getGLFunctionAddress("glGetActiveAttrib");
	if (glGetActiveAttrib == nullptr) return false;

	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)getGLFunctionAddress("glGetAttribLocation");
	if (glGetAttribLocation == nullptr) return false;

	glUseProgram = (PFNGLUSEPROGRAMPROC)getGLFunctionAddress("glUseProgram");
	if (glUseProgram == nullptr) return false;

	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)getGLFunctionAddress("glGetUniformLocation");
	if (glGetUniformLocation == nullptr) return false;

	glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)getGLFunctionAddress("glGetActiveUniform");
	if (glGetActiveUniform == nullptr) return false;

	glUniform1f = (PFNGLUNIFORM1FPROC)getGLFunctionAddress("glUniform1f");
	if (glUniform1f == nullptr) return false;

	glUniform3f = (PFNGLUNIFORM3FPROC)getGLFunctionAddress("glUniform3f");
	if (glUniform3f == nullptr) return false;

	glUniform4f = (PFNGLUNIFORM4FPROC)getGLFunctionAddress("glUniform4f");
	if (glUniform4f == nullptr) return false;

	glUniform4fv = (PFNGLUNIFORM4FVPROC)getGLFunctionAddress("glUniform4fv");
	if (glUniform4fv == nullptr) return false;

	glUniform1i = (PFNGLUNIFORM1IPROC)getGLFunctionAddress("glUniform1i");
	if (glUniform1i == nullptr) return false;

	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)getGLFunctionAddress("glUniformMatrix4fv");
	if (glUniformMatrix4fv == nullptr) return false;

	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)getGLFunctionAddress("glBindBufferBase");
	if (glBindBufferBase == nullptr) return false;

	glProgramParameteri = (PFNGLPROGRAMPARAMETERIPROC)getGLFunctionAddress("glProgramParameteri");
	if (glProgramParameteri == nullptr) return false;

	glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)getGLFunctionAddress("glGetActiveUniformName");
	if (glGetActiveUniformName == nullptr) return false;

	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)getGLFunctionAddress("glGetUniformBlockIndex");
	if (glGetUniformBlockIndex == nullptr) return false;

	glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)getGLFunctionAddress("glGetActiveUniformBlockiv");
	if (glGetActiveUniformBlockiv == nullptr) return false;

	glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)getGLFunctionAddress("glGetUniformIndices");
	if (glGetUniformIndices == nullptr) return false;

	glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)getGLFunctionAddress("glGetActiveUniformsiv");
	if (glGetActiveUniformsiv == nullptr) return false;
	
	glGetProgramBinary = (PFNGLGETPROGRAMBINARYPROC)getGLFunctionAddress("glGetProgramBinary");
	if (glGetProgramBinary == nullptr) return false;

	glProgramBinary = (PFNGLPROGRAMBINARYPROC)getGLFunctionAddress("glProgramBinary");
	if (glProgramBinary == nullptr) return false;

	glShaderBinary = (PFNGLSHADERBINARYPROC)getGLFunctionAddress("glShaderBinary");
	if (glShaderBinary == nullptr) return false;

	glCreateShaderProgramv = (PFNGLCREATESHADERPROGRAMVPROC)getGLFunctionAddress("glCreateShaderProgramv");
	if (glCreateShaderProgramv == nullptr) return false;

	glUseProgramStages = (PFNGLUSEPROGRAMSTAGESPROC)getGLFunctionAddress("glUseProgramStages");
	if (glUseProgramStages == nullptr) return false;

	glBindProgramPipeline = (PFNGLBINDPROGRAMPIPELINEPROC)getGLFunctionAddress("glBindProgramPipeline");
	if (glBindProgramPipeline == nullptr) return false;

	glProgramUniform3f = (PFNGLPROGRAMUNIFORM3FPROC)getGLFunctionAddress("glProgramUniform3f");
	if (glProgramUniform3f == nullptr) return false;

	glActiveShaderProgram = (PFNGLACTIVESHADERPROGRAMPROC)getGLFunctionAddress("glActiveShaderProgram");
	if (glActiveShaderProgram == nullptr) return false;

	glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)getGLFunctionAddress("glGetProgramInterfaceiv");
	if (glGetProgramInterfaceiv == nullptr) return false;

	glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)getGLFunctionAddress("glGetProgramResourceiv");
	if (glGetProgramResourceiv == nullptr) return false;

	glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)getGLFunctionAddress("glGetProgramResourceName");
	if (glGetProgramResourceName == nullptr) return false;

	glCreateProgramPipelines = (PFNGLCREATEPROGRAMPIPELINESPROC)getGLFunctionAddress("glCreateProgramPipelines");
	if (glCreateProgramPipelines == nullptr) return false;

	glDeleteProgramPipelines = (PFNGLDELETEPROGRAMPIPELINESPROC)getGLFunctionAddress("glDeleteProgramPipelines");
	if (glDeleteProgramPipelines == nullptr) return false;

	glSpecializeShader = (PFNGLSPECIALIZESHADERPROC)getGLFunctionAddress("glSpecializeShader");
	if (glSpecializeShader == nullptr) return false;

	glGenBuffers = (PFNGLGENBUFFERSPROC)getGLFunctionAddress("glGenBuffers");
	if (glGenBuffers == nullptr) return false;

	glIsBuffer = (PFNGLISBUFFERPROC)getGLFunctionAddress("glIsBuffer");
	if (glIsBuffer == nullptr) return false;

	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)getGLFunctionAddress("glDeleteBuffers");
	if (glDeleteBuffers == nullptr) return false;

	glBindBuffer = (PFNGLBINDBUFFERPROC)getGLFunctionAddress("glBindBuffer");
	if (glBindBuffer == nullptr) return false;

	glBufferData = (PFNGLBUFFERDATAPROC)getGLFunctionAddress("glBufferData");
	if (glBufferData == nullptr) return false;

	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)getGLFunctionAddress("glBufferSubData");
	if (glBufferSubData == nullptr) return false;

	glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)getGLFunctionAddress("glGetBufferSubData");
	if (glGetBufferSubData == nullptr) return false;

	glMapBuffer = (PFNGLMAPBUFFERPROC)getGLFunctionAddress("glMapBuffer");
	if (glMapBuffer == nullptr) return false;

	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)getGLFunctionAddress("glUnmapBuffer");
	if (glUnmapBuffer == nullptr) return false;

	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)getGLFunctionAddress("glMapBufferRange");
	if (glMapBufferRange == nullptr) return false;

	glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)getGLFunctionAddress("glFlushMappedBufferRange");
	if (glFlushMappedBufferRange == nullptr) return false;

	glCopyBufferSubData = (PFNGLCOPYBUFFERSUBDATAPROC)getGLFunctionAddress("glCopyBufferSubData");
	if (glCopyBufferSubData == nullptr) return false;

	glClearBufferData = (PFNGLCLEARBUFFERDATAPROC)getGLFunctionAddress("glClearBufferData");
	if (glClearBufferData == nullptr) return false;

	glClearBufferSubData = (PFNGLCLEARBUFFERSUBDATAPROC)getGLFunctionAddress("glClearBufferSubData");
	if (glClearBufferSubData == nullptr) return false;

	glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)getGLFunctionAddress("glInvalidateBufferData");
	if (glInvalidateBufferData == nullptr) return false;

	glInvalidateBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)getGLFunctionAddress("glInvalidateBufferSubData");
	if (glInvalidateBufferSubData == nullptr) return false;

	glBufferStorage = (PFNGLBUFFERSTORAGEPROC)getGLFunctionAddress("glBufferStorage");
	if (glBufferStorage == nullptr) return false;

	glCreateBuffers = (PFNGLCREATEBUFFERSPROC)getGLFunctionAddress("glCreateBuffers");
	if (glCreateBuffers == nullptr) return false;

	glNamedBufferData = (PFNGLNAMEDBUFFERDATAPROC)getGLFunctionAddress("glNamedBufferData");
	if (glNamedBufferData == nullptr) return false;

	glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)getGLFunctionAddress("glNamedBufferStorage");
	if (glNamedBufferStorage == nullptr) return false;

	glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)getGLFunctionAddress("glNamedBufferSubData");
	if (glNamedBufferSubData == nullptr) return false;

	glClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATAPROC)getGLFunctionAddress("glClearNamedBufferData");
	if (glClearNamedBufferData == nullptr) return false;

	glClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC)getGLFunctionAddress("glClearNamedBufferSubData");
	if (glClearNamedBufferSubData == nullptr) return false;

	glCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC)getGLFunctionAddress("glCopyNamedBufferSubData");
	if (glCopyNamedBufferSubData == nullptr) return false;

	glGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC)getGLFunctionAddress("glGetNamedBufferSubData");
	if (glGetNamedBufferSubData == nullptr) return false;

	glMapNamedBuffer = (PFNGLMAPNAMEDBUFFERPROC)getGLFunctionAddress("glMapNamedBuffer");
	if (glMapNamedBuffer == nullptr) return false;

	glUnmapNamedBuffer = (PFNGLUNMAPNAMEDBUFFERPROC)getGLFunctionAddress("glUnmapNamedBuffer");
	if (glUnmapNamedBuffer == nullptr) return false;

	glMapNamedBufferRange = (PFNGLMAPNAMEDBUFFERRANGEPROC)getGLFunctionAddress("glMapNamedBufferRange");
	if (glMapNamedBufferRange == nullptr) return false;

	glFlushMappedNamedBufferRange = (PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC)getGLFunctionAddress("glFlushMappedNamedBufferRange");
	if (glFlushMappedNamedBufferRange == nullptr) return false;

	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)getGLFunctionAddress("glEnableVertexAttribArray");
	if (glEnableVertexAttribArray == nullptr) return false;

	glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)getGLFunctionAddress("glDisableVertexAttribArray");
	if (glDisableVertexAttribArray == nullptr) return false;

	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)getGLFunctionAddress("glVertexAttribPointer");
	if (glVertexAttribPointer == nullptr) return false;

	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)getGLFunctionAddress("glBindAttribLocation");
	if (glBindAttribLocation == nullptr) return false;

	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)getGLFunctionAddress("glGenVertexArrays");
	if (glGenVertexArrays == nullptr) return false;

	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)getGLFunctionAddress("glDeleteVertexArrays");
	if (glDeleteVertexArrays == nullptr) return false;

	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)getGLFunctionAddress("glBindVertexArray");
	if (glBindVertexArray == nullptr) return false;

	glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)getGLFunctionAddress("glBindFragDataLocation");
	if (glBindFragDataLocation == nullptr) return false;

	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)getGLFunctionAddress("glVertexAttribIPointer");
	if (glVertexAttribIPointer == nullptr) return false;

	glProvokingVertex = (PFNGLPROVOKINGVERTEXPROC)getGLFunctionAddress("glProvokingVertex");
	if (glProvokingVertex == nullptr) return false;

	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)getGLFunctionAddress("glVertexAttribDivisor");
	if (glVertexAttribDivisor == nullptr) return false;

	glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)getGLFunctionAddress("glVertexAttribLPointer");
	if (glVertexAttribLPointer == nullptr) return false;

	glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)getGLFunctionAddress("glBindVertexBuffer");
	if (glBindVertexBuffer == nullptr) return false;

	glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)getGLFunctionAddress("glVertexAttribFormat");
	if (glVertexAttribFormat == nullptr) return false;

	glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)getGLFunctionAddress("glVertexAttribBinding");
	if (glVertexAttribBinding == nullptr) return false;

	glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)getGLFunctionAddress("glCreateVertexArrays");
	if (glCreateVertexArrays == nullptr) return false;

	glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)getGLFunctionAddress("glDrawArraysInstanced");
	if (glDrawArraysInstanced == nullptr) return false;

	glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)getGLFunctionAddress("glDrawElementsInstanced");
	if (glDrawElementsInstanced == nullptr) return false;

	glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)getGLFunctionAddress("glGenerateMipmap");
	if (glGenerateMipmap == nullptr) return false;

	glCreateTextures = (PFNGLCREATETEXTURESPROC)getGLFunctionAddress("glCreateTextures");
	if (glCreateTextures == nullptr) return false;

	glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)getGLFunctionAddress("glDebugMessageCallback");
	if (glDebugMessageCallback == nullptr) return false;

	glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)getGLFunctionAddress("glDebugMessageControl");
	if (glDebugMessageControl == nullptr) return false;

	glPushDebugGroup = (PFNGLPUSHDEBUGGROUPPROC)getGLFunctionAddress("glPushDebugGroup");
	if (glPushDebugGroup == nullptr) return false;

	glPopDebugGroup = (PFNGLPOPDEBUGGROUPPROC)getGLFunctionAddress("glPopDebugGroup");
	if (glPopDebugGroup == nullptr) return false;

	glGenTransformFeedbacks = (PFNGLGENTRANSFORMFEEDBACKSPROC)getGLFunctionAddress("glGenTransformFeedbacks");
	if (glGenTransformFeedbacks == nullptr) return false;

	glBindTransformFeedback = (PFNGLBINDTRANSFORMFEEDBACKPROC)getGLFunctionAddress("glBindTransformFeedback");
	if (glBindTransformFeedback == nullptr) return false;

	glIsTransformFeedback = (PFNGLISTRANSFORMFEEDBACKPROC)getGLFunctionAddress("glIsTransformFeedback");
	if (glIsTransformFeedback == nullptr) return false;

	glDeleteTransformFeedbacks = (PFNGLDELETETRANSFORMFEEDBACKSPROC)getGLFunctionAddress("glDeleteTransformFeedbacks");
	if (glDeleteTransformFeedbacks == nullptr) return false;
	
	glCreateTransformFeedbacks = (PFNGLCREATETRANSFORMFEEDBACKSPROC)getGLFunctionAddress("glCreateTransformFeedbacks");
	if (glCreateTransformFeedbacks == nullptr) return false;

	glTransformFeedbackBufferBase = (PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC)getGLFunctionAddress("glTransformFeedbackBufferBase");
	if (glTransformFeedbackBufferBase == nullptr) return false;

	glTransformFeedbackBufferRange = (PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC)getGLFunctionAddress("glTransformFeedbackBufferRange");
	if (glTransformFeedbackBufferRange == nullptr) return false;
	
	glDepthRangef = (PFNGLDEPTHRANGEFPROC)getGLFunctionAddress("glDepthRangef");
	if (glDepthRangef == nullptr) return false;

	glBindBuffersRange = (PFNGLBINDBUFFERSRANGEPROC)getGLFunctionAddress("glBindBuffersRange");
	if (glBindBuffersRange == nullptr) return false;

	// ***************************************************************************

	glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)getGLFunctionAddress("glClearBufferfv");
	if (glClearBufferfv == nullptr) return false;

	glIsVertexArray = (PFNGLISVERTEXARRAYPROC)getGLFunctionAddress("glIsVertexArray");
	if (glIsVertexArray == nullptr) return false;

	glEnableVertexArrayAttrib = (PFNGLENABLEVERTEXARRAYATTRIBPROC)getGLFunctionAddress("glEnableVertexArrayAttrib");
	if (glEnableVertexArrayAttrib == nullptr) return false;

	glDisableVertexArrayAttrib = (PFNGLDISABLEVERTEXARRAYATTRIBPROC)getGLFunctionAddress("glDisableVertexArrayAttrib");
	if (glDisableVertexArrayAttrib == nullptr) return false;

	glVertexArrayAttribFormat = (PFNGLVERTEXARRAYATTRIBFORMATPROC)getGLFunctionAddress("glVertexArrayAttribFormat");
	if (glVertexArrayAttribFormat == nullptr) return false;

	glVertexArrayVertexBuffers = (PFNGLVERTEXARRAYVERTEXBUFFERSPROC)getGLFunctionAddress("glVertexArrayVertexBuffers");
	if (glVertexArrayVertexBuffers == nullptr) return false;

	glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)getGLFunctionAddress("glDrawArraysInstancedBaseInstance");
	if (glDrawArraysInstancedBaseInstance == nullptr) return false;

	glGetStringi = (PFNGLGETSTRINGIPROC)getGLFunctionAddress("glGetStringi");
	if (glGetStringi == nullptr) return false;

	glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)getGLFunctionAddress("glPrimitiveRestartIndex");
	glTexStorage1D = (PFNGLTEXSTORAGE1DPROC)getGLFunctionAddress("glTexStorage1D");
	glTextureStorage1D = (PFNGLTEXTURESTORAGE1DPROC)getGLFunctionAddress("glTextureStorage1D");
	glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)getGLFunctionAddress("glTexStorage2D");
	glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)getGLFunctionAddress("glTextureStorage2D");
	glTexStorage3D = (PFNGLTEXSTORAGE3DPROC)getGLFunctionAddress("glTexStorage3D");
	glTextureStorage3D = (PFNGLTEXTURESTORAGE3DPROC)getGLFunctionAddress("glTextureStorage3D");
	glTextureSubImage1D = (PFNGLTEXTURESUBIMAGE1DPROC)getGLFunctionAddress("glTextureSubImage1D");
	glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)getGLFunctionAddress("glTextureSubImage2D");
	//glTexSubImage3D = (PFNGLTEXSUBIMAGE3DPROC)getGLFunctionAddress("glTexSubImage3D");
	glTextureSubImage3D = (PFNGLTEXTURESUBIMAGE3DPROC)getGLFunctionAddress("glTextureSubImage3D");
	glBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC)getGLFunctionAddress("glBindTextureUnit");	
	glTexBuffer = (PFNGLTEXBUFFERPROC)getGLFunctionAddress("glTexBuffer");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)getGLFunctionAddress("glUniformBlockBinding");

#if defined(_WIN32)
	glActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC)getGLFunctionAddress("glActiveTexture");
#endif

	if (glPrimitiveRestartIndex == nullptr ||
		glTexStorage1D == nullptr ||
		glTextureStorage1D == nullptr ||
		glTexStorage2D == nullptr ||
		glTextureStorage2D == nullptr ||
		glTexStorage3D == nullptr ||
		glTextureStorage3D == nullptr ||
		glTextureSubImage1D == nullptr ||
		glTextureSubImage2D == nullptr ||
		//glTexSubImage3D == nullptr ||
		glTextureSubImage3D == nullptr ||
		glBindTextureUnit == nullptr ||
		//glActiveTexture == nullptr ||
		glTexBuffer == nullptr ||
		glUniformBlockBinding == nullptr ||
		glGenerateMipmap == nullptr)
	{
		return false;
	}

	return true;
}

/*
	Get OpenGL info
*/
std::string kengine::getGLInfo(bool extension)
{
	std::stringstream renderingInfo;

	const GLubyte* vendor = glGetString(GL_VENDOR);
	renderingInfo << "> GL vendor: " << vendor << "\n";

	const GLubyte* renderer = glGetString(GL_RENDERER);
	renderingInfo << "> GL renderer: " << renderer << "\n";

	const GLubyte* version = glGetString(GL_VERSION);
	renderingInfo << "> GL version: " << version << "\n";

	const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	renderingInfo << "> GLSL version: " << glslVersion << "\n";

	GLint shaderProgramBinaryformats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &shaderProgramBinaryformats);
	renderingInfo << "> Shader program binary supported formats: " << shaderProgramBinaryformats << "\n";

	GLint maxVertexAttribs;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
	renderingInfo << "\n> Max vertex attributes: " << maxVertexAttribs;

	GLint maxVertexTextureImageUnits;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextureImageUnits);
	renderingInfo << "\n> Max vertex texture image units: " << maxVertexTextureImageUnits;

	GLint maxCombinedTextureImageUnits;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxCombinedTextureImageUnits);
	renderingInfo << "\n> Max combined texture image units: " << maxCombinedTextureImageUnits;

	GLint maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	renderingInfo << "\n> Max texture size: " << maxTextureSize;

	GLint maxUBOBindings;
	glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &maxUBOBindings);
	renderingInfo << "\n> Max uniform buffer bindings: " << maxUBOBindings;

	GLint maxTransformFeedbackBuffers;
	glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_BUFFERS, &maxTransformFeedbackBuffers);
	renderingInfo << "\n> Max transform feedback buffers: " << maxTransformFeedbackBuffers << "\n";

	if (extension) {
		GLint numExtensions;
		glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
		renderingInfo << "\n> GL_NUM_EXTENSIONS: " << numExtensions << "\n";

		for (GLint i = 0; i < numExtensions; i++) {
			renderingInfo << "> " << glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)) << "\n";
		}
	}

	return renderingInfo.str();
}

/*
	This function reads a text file and return in the std::string form
*/
std::string kengine::readFromFile(const std::string& filename)
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
	Get the shader type as string
*/
std::string kengine::getShaderType(GLuint shaderType)
{
	switch (shaderType)
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

size_t kengine::getGLTypeSize(GLenum type)
{
	switch (type)
	{
	case GL_BYTE:
		return sizeof(GLbyte);
		break;

	case GL_UNSIGNED_BYTE:
		return sizeof(GLubyte);
		break;

	case GL_SHORT:
		return sizeof(GLshort);
		break;

	case GL_UNSIGNED_SHORT:
		return sizeof(GLushort);
		break;

	case GL_UNSIGNED_INT:
		return sizeof(GLuint);
		break;

	case GL_FLOAT:
		return sizeof(GLfloat);
		break;

	default:
		return 0;
	}
}

/*
	Get the number of components of the OpenGL data type (i.e. data type size of vec3, mat4, etc)
*/
size_t kengine::getGLTypeComponents(const std::string& type) {
	if (!type.compare("SCALAR")) return 1;
	if (!type.compare("VEC2")) return 2;
	if (!type.compare("VEC3")) return 3;
	if (!type.compare("VEC4")) return 4;
	if (!type.compare("MAT2")) return 4;
	if (!type.compare("MAT3")) return 9;
	if (!type.compare("MAT4")) return 16;
	return 0;
}

/*
	This function compile a GLSL shader
*/
GLuint kengine::compileShader(GLuint shaderType, const std::string& filename)
{
	GLuint shaderObject = glCreateShader(shaderType);

	if (!shaderObject) {
		K_LOG_OUTPUT_RAW("It was not possible to create [" << getShaderType(shaderType) << "] shader!");
		return 0;
	}

	std::string sourceString = readFromFile(filename);

	if (sourceString.empty()) {
		K_LOG_OUTPUT_RAW("This file " << filename << " cannot be opened.");
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

/*
	Helper function to compile SPIR-V shader
*/
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

/*
	kengine::GLSLprogram class - member class definition
*/

kengine::GLSLprogram::~GLSLprogram()
{
	if (glDeleteProgram)
		clear();
}

bool kengine::GLSLprogram::loadShaders(const ShaderInfo* shaderInfo)
{
	bool ret = false;

	if (shaderInfo) {
		if (m_programHandle) {
			clear();
		}

		// OpenGL rendering pipeline has six stages
		GLuint shaders[6] = { 0 };

		for (int index = 0; shaderInfo[index].type != GL_NONE && index < 6; index++) {
			shaders[index] = compileShader(shaderInfo[index].type, shaderInfo[index].filename);
		}

		m_programHandle = glCreateProgram();

		if (m_programHandle) {
			for (int index = 0; index < 6; index++) {
				if (shaders[index])
					glAttachShader(m_programHandle, shaders[index]);
			}

			glLinkProgram(m_programHandle);

			GLint status;
			glGetProgramiv(m_programHandle, GL_LINK_STATUS, &status);

			if (status == GL_TRUE) {
				/*
					(!) only in OpenGL 4.3 version - need to implement alternative version for previous versions
				*/

				// query the location of a uniform variable
				GLint numUniforms = 0;
				glGetProgramInterfaceiv(m_programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);
				GLenum properties[] = { GL_NAME_LENGTH, GL_LOCATION };

				for (GLint i = 0; i < numUniforms; i++) {
					GLint results[2];
					glGetProgramResourceiv(m_programHandle, GL_UNIFORM, static_cast<GLuint>(i), 2, properties, 2, nullptr, results);
					GLint nameBufferSize = results[0] + 1;
					char* name = new char[static_cast<size_t>(nameBufferSize)];
					glGetProgramResourceName(m_programHandle, GL_UNIFORM, static_cast<GLuint>(i), nameBufferSize, nullptr, name);
					m_uniformMap[name] = results[1];
					delete[] name;
				}

				ret = true;

			} else {
				std::string log = "It was not possible to link a shader program";

				GLint logLength;
				glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &logLength);

				if (logLength > 0) {
					std::string infoLog(static_cast<unsigned int>(logLength), ' ');
					GLsizei logWrittenLength;
					glGetProgramInfoLog(m_programHandle, logLength, &logWrittenLength, &infoLog[0]);
					log += ": " + infoLog;
				}

				K_LOG_OUTPUT_RAW(log);
			}
		}
		else
			K_LOG_OUTPUT_RAW("It was not possible to create a shader program!");

		// clean up shader objects
		for (int index = 0; index < 6; index++) {
			if (shaders[index]) {
				glDetachShader(m_programHandle, shaders[index]);
				glDeleteShader(shaders[index]);
			}
		}
	}

	return ret;
}

void kengine::GLSLprogram::useProgram()
{
	glUseProgram(m_programHandle);
}

void kengine::GLSLprogram::clear()
{
	glDeleteProgram(m_programHandle);
	m_programHandle = 0;
	m_uniformMap.clear();
}

/*
	shader program binary is supported only in 4.1 version (and Apple has deprecated OpenGL)
*/
void kengine::GLSLprogram::saveBinary(const std::string& filename)
{
	if (m_programHandle == 0)
		return;

	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	if (formats < 1) {
		K_LOG_OUTPUT_RAW("Driver does not support any binary formats.");
		return;
	}

	GLint length = 0;
	glGetProgramiv(m_programHandle, GL_PROGRAM_BINARY_LENGTH, &length);

	std::vector<GLubyte> buffer(static_cast<size_t>(length));

	GLenum binaryFormat;
	glGetProgramBinary(m_programHandle, length, nullptr, &binaryFormat, buffer.data());

	std::ofstream shaderBinary(filename.c_str(), std::ios::binary);
	shaderBinary.write(reinterpret_cast<char*>(&binaryFormat), sizeof(GLenum));
	shaderBinary.write(reinterpret_cast<char*>(buffer.data()), length);
	shaderBinary.close();
}

/*
	shader program binary is supported only in 4.1 version (and Apple has deprecated OpenGL)
*/
void kengine::GLSLprogram::loadBinary(const std::string& filename)
{
	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);

	if (formats < 1) {
		K_LOG_OUTPUT_RAW("Driver does not support any binary formats.");
		return;
	}

	if (m_programHandle) {
		clear();
	}

	m_programHandle = glCreateProgram();

	std::ifstream shaderBinary(filename, std::ios::binary);

	if (!shaderBinary) {
		K_LOG_OUTPUT_RAW("Error to load a binary program: " << filename);
		return;
	}

	shaderBinary.seekg(0, std::ios::end);
	std::streamoff fileSize = shaderBinary.tellg();
	shaderBinary.seekg(0, std::ios::beg);
	size_t size = static_cast<size_t>(fileSize) - sizeof(GLenum);

	GLenum binaryFormat;
	shaderBinary.read(reinterpret_cast<char*>(&binaryFormat), sizeof(GLenum));

	std::vector<char> buffer(size);
	shaderBinary.read(buffer.data(), static_cast<std::streamsize>(size));
	shaderBinary.close();

	glProgramBinary(m_programHandle, binaryFormat, buffer.data(), static_cast<GLsizei>(buffer.size()));

	GLint status;
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		std::string log = "It was not possible to link a binary shader program";

		GLint logLength;
		glGetProgramiv(m_programHandle, GL_INFO_LOG_LENGTH, &logLength);

		if (logLength > 0) {
			std::string infoLog(static_cast<unsigned int>(logLength), ' ');
			GLsizei logWrittenLength;
			glGetProgramInfoLog(m_programHandle, logLength, &logWrittenLength, &infoLog[0]);
			log += ": " + infoLog;
		}

		K_LOG_OUTPUT_RAW(log);
	}
}

/*
*/
void kengine::GLSLprogram::setUniform(const std::string& name, bool transpose, GLfloat* value) const
{
	auto it = m_uniformMap.find(name);

	if (it == m_uniformMap.end()) {
		K_LOG_OUTPUT_RAW("Failed to set glUniformMatrix4fv: " << name << " for [" << this << "]");
		return;
	}

	glUniformMatrix4fv(it->second, 1, transpose, value);
}

void kengine::GLSLprogram::setUniform(const std::string& name, GLsizei count, bool transpose, void* data)
{
	auto it = m_uniformMap.find(name);

	if (it == m_uniformMap.end()) {
		K_LOG_OUTPUT_RAW("Failed to set glUniformMatrix4fv: " << name << " for [" << this << "]");
		return;
	}

	glUniformMatrix4fv(it->second, count, transpose, reinterpret_cast<GLfloat*>(data));
}

void kengine::GLSLprogram::setUniform(const std::string& name, GLfloat value) const
{
	auto it = m_uniformMap.find(name);

	if (it == m_uniformMap.end()) {
		K_LOG_OUTPUT_RAW("Failed to set glUniform1f: " << name << " for [" << this << "]");
		return;
	}

	glUniform1f(it->second, value);	
}

void kengine::GLSLprogram::setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z) const
{
	auto it = m_uniformMap.find(name);

	if (it == m_uniformMap.end()) {
		K_LOG_OUTPUT_RAW("Failed to set glUniform3f: " << name << " for [" << this << "]");
		return;
	}

	glUniform3f(it->second, x, y, z);
}

void kengine::GLSLprogram::setUniform(const std::string& name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) const
{
	auto it = m_uniformMap.find(name);

	if (it == m_uniformMap.end()) {
		K_LOG_OUTPUT_RAW("Failed to set glUniform4f: " << name << " for [" << this << "]");
		return;
	}

	glUniform4f(it->second, x, y, z, w);
}

void kengine::GLSLprogram::setUniform(const std::string& name, int value) const
{
	auto it = m_uniformMap.find(name);

	if (it == m_uniformMap.end()) {
		K_LOG_OUTPUT_RAW("Failed to set glUniform1i: " << name << " for [" << this << "]");
		return;
	}

	glUniform1i(it->second, value);
}

void kengine::GLSLprogram::print() const
{
	K_LOG_OUTPUT_RAW("> kengine::GLSLprogram object info [" << this << "]\n");
	
	// (!) only in OpenGL 4.3 version
	GLint numAttribs = 0;
	glGetProgramInterfaceiv(m_programHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);
	
	if (numAttribs > 0) {
		K_LOG_OUTPUT_RAW("  active attribute variables:\n");

		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

		for (auto i = 0; i < numAttribs; i++) {
			GLint results[3];
			glGetProgramResourceiv(m_programHandle, GL_PROGRAM_INPUT, static_cast<GLuint>(i), 3, properties, 3, nullptr, results);
			GLint nameBufferSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBufferSize)];
			glGetProgramResourceName(m_programHandle, GL_PROGRAM_INPUT, static_cast<GLuint>(i), nameBufferSize, nullptr, name);
			K_LOG_OUTPUT_RAW("	- name: " << name << "\n" << "	- type: " << getGLSLType(results[1]) << "\n" << "	- location: " << results[2] << "\n");
			delete[] name;
		}
	}
	
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(m_programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	if (numUniforms > 0) {
		K_LOG_OUTPUT_RAW("\n  active uniform variables:\n");
		
		GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

		for (auto i = 0; i < numUniforms; i++) {
			GLint results[4];
			glGetProgramResourceiv(m_programHandle, GL_UNIFORM, static_cast<GLuint>(i), 4, properties, 4, nullptr, results);

			GLsizei nameBuffSize = results[0] + 1;
			char* name = new char[static_cast<size_t>(nameBuffSize)];

			glGetProgramResourceName(m_programHandle, GL_UNIFORM, static_cast<GLuint>(i), nameBuffSize, nullptr, name);

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
	kengine::uniform_buffer_object class - member class definition
*/

void kengine::uniform_buffer_object::load(GLuint program, const std::string& name, const std::vector<ubo_var>& uboVars)
{
	if (m_ubo)
		clear();

	GLuint blockIndex = glGetUniformBlockIndex(program, name.c_str());

	if (blockIndex == GL_INVALID_INDEX) {
		K_LOG_OUTPUT_RAW("Failed to get block index for " + name);
		return;
	}

	GLint blockSize = 0;
	glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

	if (blockSize > 0) {
		GLubyte* blockBuffer = new GLubyte[static_cast<size_t>(blockSize)];

		std::vector<const GLchar*> names;
		for (auto& var : uboVars) {
			names.push_back(var.name.c_str());
		}

		std::vector<GLuint> indices(uboVars.size(), -1);
		glGetUniformIndices(program, uboVars.size(), names.data(), indices.data());

		std::vector<GLint> offset(uboVars.size(), 0);
		glGetActiveUniformsiv(program, uboVars.size(), indices.data(), GL_UNIFORM_OFFSET, offset.data());

		for(size_t index = 0; index < uboVars.size(); index++) {
			memcpy(blockBuffer + offset[index], uboVars[index].data.data(), uboVars[index].data.size() * sizeof(GLfloat));
		}

		glGenBuffers(1, &m_ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
		delete[] blockBuffer;
	}
}

void kengine::uniform_buffer_object::update()
{
	//glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
	//float* data = (float*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
	//glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void kengine::uniform_buffer_object::clear()
{
	glDeleteBuffers(1, &m_ubo);
	m_ubo = 0;
}

/*
	kengine::vattrib class - member class definition
*/

kengine::vattrib::vattrib(const unsigned char* array, size_t arraySize, size_t count, size_t type, GLenum target)
{
	assert("kengine::vattrib constructor | array parameter is nullptr" && array != nullptr);

	if (arraySize > 0) {
		m_arraySize = arraySize;
		m_count = count;
		m_type = type;
		m_attributeArray = new unsigned char[m_arraySize];
		m_target = target;

		for (size_t i = 0; i < m_arraySize; i++) {
			m_attributeArray[i] = array[i];
		}
	}
}

// copy constructor
kengine::vattrib::vattrib(const vattrib& va)
{
	if (va.m_arraySize) {
		m_arraySize = va.m_arraySize;
		m_count = va.m_count;
		m_type = va.m_type;
		m_target = va.m_target;

		m_attributeArray = new unsigned char[m_arraySize];

		for (size_t i = 0; i < va.m_arraySize; i++) {
			m_attributeArray[i] = va.m_attributeArray[i];
		}
	}
}

// copy assignment = operator
kengine::vattrib& kengine::vattrib::operator=(const vattrib& va)
{
	clear();

	if (va.m_arraySize) {
		m_arraySize = va.m_arraySize;
		m_count = va.m_count;
		m_type = va.m_type;
		m_target = va.m_target;

		m_attributeArray = new unsigned char[m_arraySize];

		for (size_t i = 0; i < va.m_arraySize; i++) {
			m_attributeArray[i] = va.m_attributeArray[i];
		}
	}

	/* testar caso de copia invalida */
	return *this;
}

// move constructor
kengine::vattrib::vattrib(vattrib&& move) noexcept
	: m_attributeArray{ move.m_attributeArray },
	m_arraySize{ move.m_arraySize },
	m_count{ move.m_count },
	m_type{ move.m_type },
	m_target { move.m_target }
{
	move.m_attributeArray = nullptr;
	move.m_arraySize = 0;
	move.m_count = 0;
	move.m_type = 0;
	move.m_target = 0;
}

// move assignment = operator
kengine::vattrib& kengine::vattrib::operator=(vattrib&& move)
{
	clear();

	m_attributeArray = std::move(move.m_attributeArray);
	m_arraySize = std::move(move.m_arraySize);
	m_count = std::move(move.m_count);
	m_type = std::move(move.m_type);
	m_target = std::move(move.m_target);

	move.m_attributeArray = nullptr;
	move.m_arraySize = 0;
	move.m_count = 0;
	move.m_type = 0;
	move.m_target = 0;
	return *this;
}

// == operator overload
bool kengine::vattrib::operator==(const vattrib& op) const
{
	if (m_arraySize != op.m_arraySize || m_count != op.m_count || m_type != op.m_type || m_target != op.m_target)
		return false;

	for (size_t i = 0; i < m_arraySize; i++) {
		if (m_attributeArray[i] != op.m_attributeArray[i])
			return false;
	}

	return true;
}

void kengine::vattrib::clear()
{
	delete[] m_attributeArray;
	m_attributeArray = nullptr;
	m_arraySize = 0;
	m_count = 0;
	m_type = 0;
	m_target = 0;
}

void kengine::vattrib::dump()
{
	switch (m_type) {
	case GL_FLOAT:
		for (size_t i = 0; i < m_arraySize / sizeof(GLfloat); i++) {
			GLfloat data;
			std::memcpy(&data, m_attributeArray + i * sizeof(GLfloat), sizeof(GLfloat));
			K_LOG_OUTPUT_RAW("m_attributeArray[ " << i << "]: " << data);
		}
		break;
	}
}

/*
	kengine::gpu_buffer class - class members definition
*/

kengine::gpu_buffer::gpu_buffer(size_t size)
{
	assert("kengine::gpu_buffer constructor | OpenGL context was not created!" && glGenBuffers != nullptr);

	m_buffers.resize(size);
	glGenBuffers(size, m_buffers.data());
}

kengine::gpu_buffer::~gpu_buffer()
{
	clear();
}

kengine::gpu_buffer::gpu_buffer(gpu_buffer&& move) noexcept
{
	m_buffers = std::move(move.m_buffers);
	move.m_buffers.clear();
}

void kengine::gpu_buffer::addBuffers(size_t size)
{
	std::vector<GLuint> buffers(size);
	glGenBuffers(size, buffers.data());
	m_buffers.insert(m_buffers.end(), buffers.begin(), buffers.end());
}

void kengine::gpu_buffer::clear()
{
	if (m_buffers.size() > 0) {
		glDeleteBuffers(m_buffers.size(), m_buffers.data());
	}

	m_buffers.clear();
}

void kengine::gpu_buffer::load(size_t index, const kengine::vattrib& va)
{
	assert("kengine::gpu_buffer load | invalid vector index" && index < m_buffers.size());

	glBindBuffer(va.target(), m_buffers[index]);
	glBufferData(va.target(), va.size(), va.data(), GL_STATIC_DRAW);
}

void kengine::gpu_buffer::load(size_t index, GLenum target, size_t size, const void* data)
{
	assert("kengine::gpu_buffer load | invalid vector index" && index < m_buffers.size());

	glBindBuffer(target, m_buffers[index]);
	glBufferData(target, size, data, GL_STATIC_DRAW);
}

void kengine::gpu_buffer::update(size_t index, GLenum target, size_t size, const void* data)
{
	assert("kengine::gpu_buffer update | invalid vector index" && index < m_buffers.size());

	glBindBuffer(target, m_buffers[index]);
	void* buffer = glMapBuffer(target, GL_WRITE_ONLY);

	if (buffer) {
		std::memcpy(buffer, data, size);
		glUnmapBuffer(target);
	}
}

/*
	kengine::primitive class - member class definition
*/

kengine::primitive_render::primitive_render()
{
	assert("kengine::primitive_render constructor | OpenGL context was not created!" && glGenVertexArrays != nullptr);
	
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
}


kengine::primitive_render::primitive_render(std::unique_ptr<gpu_buffer>&& buffer)
	: m_buffer{ std::move(buffer) }
{
	assert("kengine::primitive_render constructor | OpenGL context was not created!" && glGenVertexArrays != nullptr);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
}

kengine::primitive_render::~primitive_render()
{
	clear();
}

void kengine::primitive_render::loadAttributeInfo(GLuint location, GLuint vbo, GLenum target, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset)
{
	glBindVertexArray(m_vao);
	glBindBuffer(target, vbo);
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(location, size, type, normalized, stride, offset);
}

void kengine::primitive_render::loadAttributeInfo(GLuint location, GLuint vbo, const kengine::vattrib& va)
{
	GLuint buffer = vbo;

	if (m_buffer)
		buffer = (*m_buffer)[vbo];

	loadAttributeInfo(location, buffer, va.target(), va.count(), va.type(), GL_FALSE, 0, nullptr);
}

void kengine::primitive_render::bindBuffer(GLenum target, GLuint vbo)
{
	GLuint buffer = vbo;

	if (m_buffer)
		buffer = (*m_buffer)[vbo];

	glBindVertexArray(m_vao);
	glBindBuffer(target, buffer);
}

void kengine::primitive_render::bindBuffer(GLuint vbo, const kengine::vattrib& va)
{
	GLuint buffer = vbo;

	if (m_buffer)
		buffer = (*m_buffer)[vbo];

	glBindVertexArray(m_vao);
	glBindBuffer(va.target(), buffer);
}

void kengine::primitive_render::drawArrays(GLint start, GLsizei count)
{
	glBindVertexArray(m_vao);
	glDrawArrays(m_mode, start, count);
}

void kengine::primitive_render::drawArraysInstanced(GLint start, GLsizei count, GLsizei instanceCount)
{
	glBindVertexArray(m_vao);
	glDrawArraysInstanced(m_mode, start, count, instanceCount);
}

void kengine::primitive_render::drawElements(GLsizei count, const void* offset)
{
	glBindVertexArray(m_vao);
	glDrawElements(m_mode, count, m_indexType, offset);
}

void kengine::primitive_render::draw()
{
	glBindVertexArray(m_vao);

	if (m_elements)
		glDrawElements(m_mode, m_elements, m_indexType, (const GLvoid*)m_elementsOffset);
	else
		glDrawArrays(m_mode, 0, m_count);
}

void kengine::primitive_render::drawInstanced(GLsizei instanceCount)
{
	glBindVertexArray(m_vao);

	if (m_elements)
		glDrawElementsInstanced(m_mode, m_elements, m_indexType, (const GLvoid*)m_elementsOffset, instanceCount);
	else
		glDrawArraysInstanced(m_mode, 0, m_count, instanceCount);

}

void kengine::primitive_render::clear()
{
	glDeleteVertexArrays(1, &m_vao);
	m_vao = 0;
	m_count = 0;
	m_elements = 0;
	m_mode = GL_TRIANGLES;
	m_indexType = GL_UNSIGNED_SHORT;
	m_buffer.reset();
}


/*
	kengine::texture class - member definition
*/

kengine::texture::~texture()
{
	clear();
}

void kengine::texture::create(TEXTURE_TYPE target, GLsizei levels, GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data)
{
	clear();

	switch (target)
	{
	case TEXTURE_TYPE::_2D:
		m_target = GL_TEXTURE_2D;
		break;
	}

	glGenTextures(1, &m_name);
	glBindTexture(m_target, m_name);

	glTexImage2D(m_target, levels, internalFormat, width, height, 0, format, type, data); // OpenGL 2.0+

	/*
		MIPMAP -> high resoltion to smaller scale
		- GL_NEAREST_MIPMAP_NEAREST
	*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void kengine::texture::update(GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data)
{
	if (m_target == GL_TEXTURE_2D)
		glTexSubImage2D(m_target, level, xoffset, yoffset, width, height, format, type, data);
}

void kengine::texture::bindTextureUnit(GLuint textureUnit)
{
	//glBindTextureUnit(textureUnit, m_name); // OpenGL 4.5+
	glActiveTexture(GL_TEXTURE0 + textureUnit); // OpenGL 2.0+
	glBindTexture(m_target, m_name);
}

void kengine::texture::bind()
{
	glBindTexture(m_target, m_name);
}

void kengine::texture::unbind()
{
	glBindTexture(m_target, 0);
}

void kengine::texture::setWrappingMode(GLint sParam, GLint tParam)
{
	glBindTexture(m_target, m_name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tParam);
}

void kengine::texture::clear()
{
	glDeleteTextures(1, &m_name);
	m_name = 0;
	m_target = 0;
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

/*
*/
std::unique_ptr<kengine::primitive_render> kengine::triangle(float size, std::unordered_map<std::string, GLuint>& shaderInfo)
{
	GLfloat half = size / 2.0f;

	GLfloat positions[] = {
		-half,  half, 0.0f,
		-half, -half, 0.0f,
		 half, -half, 0.0f
	};

	kengine::vattrib vPositions(VATTRIB_CAST(positions), 9 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat textureCoordinates[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	};

	kengine::vattrib vTextureCoordinates(VATTRIB_CAST(textureCoordinates), 6 * sizeof(GLfloat), 2, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	kengine::vattrib vNormals(VATTRIB_CAST(normals), 9 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	kengine::vattrib vColors(VATTRIB_CAST(colors), 12 * sizeof(GLfloat), 4, GL_FLOAT, GL_ARRAY_BUFFER);

	// creating the vbo
	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(4);
	vbo->load(0, vPositions);
	vbo->load(1, vTextureCoordinates);
	vbo->load(2, vNormals);
	vbo->load(3, vColors);

	auto prim = std::make_unique<kengine::primitive_render>(std::move(vbo));
	assert(!(shaderInfo.find("POSITION") == shaderInfo.end()) && "shaderInfor key [POSITION] not found!");
	prim->loadAttributeInfo(shaderInfo.at("POSITION"), 0, vPositions);
	prim->setCount(vPositions.count());

	assert(!(shaderInfo.find("TEXCOORD_0") == shaderInfo.end()) && "shaderInfor key [TEXCOORD_0] not found!");
	prim->loadAttributeInfo(shaderInfo.at("TEXCOORD_0"), 1, vTextureCoordinates);

	assert(!(shaderInfo.find("NORMAL") == shaderInfo.end()) && "shaderInfor key [NORMAL] not found!");
	prim->loadAttributeInfo(shaderInfo.at("NORMAL"), 2, vNormals);

	assert(!(shaderInfo.find("COLOR") == shaderInfo.end()) && "shaderInfor key [COLOR] not found!");
	prim->loadAttributeInfo(shaderInfo.at("COLOR"), 3, vColors);

	return prim;
}

std::unique_ptr<kengine::primitive_render> kengine::quad(float width, float height, std::unordered_map<std::string, GLuint>& shaderInfo)
{
	GLfloat halfX = width / 2.0f;
	GLfloat halfY = height / 2.0f;

	GLfloat positions[] = {
		-halfX,  halfY, 0.0f,
		-halfX, -halfY, 0.0f,
		 halfX, -halfY, 0.0f,
		 halfX,  halfY, 0.0f
	};

	kengine::vattrib vPositions(VATTRIB_CAST(positions), 12 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat textureCoordinates[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	kengine::vattrib vTexCoordinates(VATTRIB_CAST(textureCoordinates), 8 * sizeof(GLfloat), 2, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	kengine::vattrib vNormals(VATTRIB_CAST(normals), 12 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat colors[] = {
		1.0f, 0.0, 0.0, 1.0f,
		0.0f, 1.0, 0.0, 1.0f,
		0.0f, 0.0, 1.0, 1.0f,
		1.0f, 1.0, 0.0, 1.0f,
	};

	kengine::vattrib vColors(VATTRIB_CAST(colors), 16 * sizeof(GLfloat), 4, GL_FLOAT, GL_ARRAY_BUFFER);

	GLushort indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	kengine::vattrib vIndices(VATTRIB_CAST(indices), 6 * sizeof(GLushort), 1, GL_UNSIGNED_SHORT, GL_ARRAY_BUFFER);

	// creating the vbo
	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(5);
	vbo->load(0, vPositions);
	vbo->load(1, vTexCoordinates);
	vbo->load(2, vNormals);
	vbo->load(3, vColors);
	vbo->load(4, vIndices);

	auto prim = std::make_unique<kengine::primitive_render>(std::move(vbo));
	
	assert(!(shaderInfo.find("POSITION") == shaderInfo.end()) && "shaderInfor key [POSITION] not found!");
	prim->loadAttributeInfo(shaderInfo.at("POSITION"), 0, vPositions);
	prim->setCount(vPositions.getVertexCount());

	// TEXCOORD_0 is the default name in GLTF files
	assert(!(shaderInfo.find("TEXCOORD_0") == shaderInfo.end()) && "shaderInfor key [TEXCOORD_0] not found!");
	prim->loadAttributeInfo(shaderInfo.at("TEXCOORD_0"), 1, vTexCoordinates);

	assert(!(shaderInfo.find("NORMAL") == shaderInfo.end()) && "shaderInfor key [NORMAL] not found!");
	prim->loadAttributeInfo(shaderInfo.at("NORMAL"), 2, vTexCoordinates);

	assert(!(shaderInfo.find("COLOR") == shaderInfo.end()) && "shaderInfor key [COLOR] not found!");
	prim->loadAttributeInfo(shaderInfo.at("COLOR"), 3, vTexCoordinates);

	vIndices.setTarget(GL_ELEMENT_ARRAY_BUFFER);
	prim->bindBuffer(4, vIndices);
	prim->setElements(vIndices.getVertexCount());

	return prim;
}

std::unique_ptr<kengine::primitive_render> kengine::cube(float size, std::unordered_map<std::string, GLuint>& shaderInfo)
{
	GLfloat half = size / 2.0f;

	GLfloat positions[] = {
		-half,  half,  half, // front face - 0
		-half, -half,  half, // front face - 1
		 half, -half,  half, // front face - 2
		 half,  half,  half, // front face - 3
		-half, -half,  half, // down face - 4
		-half, -half, -half, // down face - 5
		 half, -half, -half, // down face - 6
		 half, -half,  half, // down face - 7
		-half, -half, -half, // back face - 8
		-half,  half, -half, // back face - 9
		 half,  half, -half, // back face - 10
		 half, -half, -half, // back face - 11
		-half,  half, -half, // top face - 12
		-half,  half,  half, // top face - 13
		 half,  half,  half, // top face - 14
		 half,  half, -half, // top face - 15
		 half,  half,  half, // right face - 16
		 half, -half,  half, // right face - 17
		 half, -half, -half, // right face - 18
		 half,  half, -half,  // right face - 19
		-half,  half, -half, // left face - 20
		-half, -half, -half, // left face - 21
		-half, -half,  half, // left face - 22
		-half,  half,  half  // left face - 23
	};

	kengine::vattrib vPositions(VATTRIB_CAST(positions), 24 * 3 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat textureCoordinates[] = {
		0.0f, 1.0f, // front face
		0.0f, 0.0f, // front face
		1.0f, 0.0f, // front face
		1.0f, 1.0f, // front face
		0.0f, 1.0f, // down face
		0.0f, 0.0f, // down face
		1.0f, 0.0f, // down face
		1.0f, 1.0f, // down face
		0.0f, 1.0f, // back face
		0.0f, 0.0f, // back face
		1.0f, 0.0f, // back face
		1.0f, 1.0f, // back face
		0.0f, 1.0f, // top face
		0.0f, 0.0f, // top face
		1.0f, 0.0f, // top face
		1.0f, 1.0f, // top face
		0.0f, 1.0f, // right face
		0.0f, 0.0f, // right face
		1.0f, 0.0f, // right face
		1.0f, 1.0f, // right face
		0.0f, 1.0f, // left face
		0.0f, 0.0f, // left face
		1.0f, 0.0f, // left face
		1.0f, 1.0f  // left face
	};

	kengine::vattrib vTextureCoordinates(VATTRIB_CAST(textureCoordinates), 24 * 2 * sizeof(GLfloat), 2, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat normals[] = {
		 0.0f,  0.0f,  1.0f, // front face
		 0.0f,  0.0f,  1.0f, // front face
		 0.0f,  0.0f,  1.0f, // front face
		 0.0f,  0.0f,  1.0f, // front face
		 0.0f, -1.0f,  0.0f, // down face
		 0.0f, -1.0f,  0.0f, // down face
		 0.0f, -1.0f,  0.0f, // down face
		 0.0f, -1.0f,  0.0f, // down face
		 0.0f,  0.0f, -1.0f, // back face
		 0.0f,  0.0f, -1.0f, // back face
		 0.0f,  0.0f, -1.0f, // back face
		 0.0f,  0.0f, -1.0f, // back face
		 0.0f,  1.0f,  0.0f, // top face
		 0.0f,  1.0f,  0.0f, // top face
		 0.0f,  1.0f,  0.0f, // top face
		 0.0f,  1.0f,  0.0f, // top face
		 1.0f,  0.0f,  0.0f, // right face
		 1.0f,  0.0f,  0.0f, // right face
		 1.0f,  0.0f,  0.0f, // right face
		 1.0f,  0.0f,  0.0f, // right face
		-1.0f,  0.0f, 0.0f, // left face
		-1.0f,  0.0f, 0.0f, // left face
		-1.0f,  0.0f, 0.0f, // left face
		-1.0f,  0.0f, 0.0f  // left face
	};

	kengine::vattrib vNormals(VATTRIB_CAST(normals), 24 * 3 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat colors[] = {
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};

	kengine::vattrib vColors(VATTRIB_CAST(colors), 24 * 4 * sizeof(GLfloat), 4, GL_FLOAT, GL_ARRAY_BUFFER);

	GLushort indices[] = {
		 0,  1,  2,  0,  2,  3, // front
		 4,  5,  6,  4,  6,  7, // bottom
		 8,  9, 10,  8, 10, 11, // back
		12, 13, 14, 12, 14, 15, // top
		16, 17, 18, 16, 18, 19, // right
		20, 21, 22, 20, 22, 23  // left
	};

	kengine::vattrib vIndices(VATTRIB_CAST(indices), 6 * 6 * sizeof(GLushort), 1, GL_UNSIGNED_SHORT, GL_ARRAY_BUFFER);

	// creating the vbo
	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(5);
	vbo->load(0, vPositions);
	vbo->load(1, vTextureCoordinates);
	vbo->load(2, vNormals);
	vbo->load(3, vColors);
	vbo->load(4, vIndices);

	auto prim = std::make_unique<kengine::primitive_render>(std::move(vbo));

	assert(!(shaderInfo.find("POSITION") == shaderInfo.end()) && "shaderInfor key [POSITION] not found!");
	prim->loadAttributeInfo(shaderInfo["POSITION"], 0, vPositions);
	prim->setCount(vPositions.getVertexCount());

	assert(!(shaderInfo.find("TEXCOORD_0") == shaderInfo.end()) && "shaderInfor key [TEXCOORD_0] not found!");
	prim->loadAttributeInfo(shaderInfo["TEXCOORD_0"], 1, vTextureCoordinates);

	assert(!(shaderInfo.find("NORMAL") == shaderInfo.end()) && "shaderInfor key [NORMAL] not found!");
	prim->loadAttributeInfo(shaderInfo["NORMAL"], 2, vNormals);

	assert(!(shaderInfo.find("COLOR") == shaderInfo.end()) && "shaderInfor key [COLOR] not found!");
	prim->loadAttributeInfo(shaderInfo["COLOR"], 3, vColors);

	vIndices.setTarget(GL_ELEMENT_ARRAY_BUFFER);
	prim->bindBuffer(4, vIndices);
	prim->setElements(vIndices.getVertexCount());

	return prim;
}

std::unique_ptr<kengine::primitive_render> kengine::circle(float radius, size_t points, std::unordered_map<std::string, GLuint>& shaderInfo)
{
	std::vector<GLfloat> positions;
	std::vector<GLfloat> normals;

	for (size_t i = 0; i < points; i++) {
		GLfloat theta = 2.0f * M_PI * i / points;
		GLfloat x = std::cos(theta);
		GLfloat y = std::sin(theta);

		positions.push_back(x * radius);
		positions.push_back(y * radius);
		positions.push_back(0.0f);

		normals.push_back(0.0f);
		normals.push_back(0.0f);
		normals.push_back(1.0f);
	}

	kengine::vattrib vPositions(VATTRIB_CAST(positions.data()), positions.size() * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);
	kengine::vattrib vNormals(VATTRIB_CAST(normals.data()), normals.size() * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(2);
	vbo->load(0, vPositions);
	vbo->load(1, vNormals);

	auto prim = std::make_unique<kengine::primitive_render>(std::move(vbo));

	prim->setMode(GL_LINE_LOOP);
	assert(!(shaderInfo.find("POSITION") == shaderInfo.end()) && "shaderInfor key [POSITION] not found!");
	prim->loadAttributeInfo(shaderInfo["POSITION"], 0, vPositions);
	prim->setCount(vPositions.getVertexCount());

	assert(!(shaderInfo.find("NORMAL") == shaderInfo.end()) && "shaderInfor key [NORMAL] not found!");
	prim->loadAttributeInfo(shaderInfo["NORMAL"], 1, vNormals);

	return prim;
}

std::unique_ptr<kengine::primitive_render> kengine::sphere(GLfloat radius, size_t slices, size_t points, std::unordered_map<std::string, GLuint>& shaderInfo)
{
	std::vector<GLfloat> positions;
	std::vector<GLfloat> textureCoordiantes;
	std::vector<GLfloat> normals;
	std::vector<GLfloat> colors;

	for (size_t i = 0; i <= slices; i++) {
		GLfloat theta = M_PI - (M_PI * i / slices);
		GLfloat r = std::sin(theta);
		GLfloat y = std::cos(theta);

		for (size_t j = 0; j <= points; j++) {
			GLfloat phi = 2.0f * M_PI * j / points;
			GLfloat x = r * std::cos(phi);
			GLfloat z = r * std::sin(phi);

			positions.push_back(radius * x);
			positions.push_back(radius * y);
			positions.push_back(radius * z);

			colors.push_back(0.1f);
			colors.push_back(1.0f);
			colors.push_back(0.1f);
			colors.push_back(1.0f);

			textureCoordiantes.push_back(static_cast<GLfloat>(j) / static_cast<GLfloat>(points));
			textureCoordiantes.push_back(static_cast<GLfloat>(i) / static_cast<GLfloat>(slices));

			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}
	}

	kengine::vattrib vPositions(VATTRIB_CAST(positions.data()), positions.size() * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);
	kengine::vattrib vTextureCoordinates(VATTRIB_CAST(textureCoordiantes.data()), textureCoordiantes.size() * sizeof(GLfloat), 2, GL_FLOAT, GL_ARRAY_BUFFER);
	kengine::vattrib vNormals(VATTRIB_CAST(normals.data()), normals.size() * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);
	kengine::vattrib vColors(VATTRIB_CAST(colors.data()), colors.size() * sizeof(GLfloat), 4, GL_FLOAT, GL_ARRAY_BUFFER);

	// calculating indices
	std::vector<GLushort> indices;

	for (int i = 0; i < slices; i++) {
		for (int j = 0; j < points; j++) {
			GLushort row = i * (points + 1);
			GLushort next_row = (i + 1) * (points + 1);

			// first triangle (CCW visto de fora)
			indices.push_back(row + j);
			indices.push_back(next_row + j);
			indices.push_back(row + j + 1);
			// second triangle
			indices.push_back(row + j + 1);
			indices.push_back(next_row + j);
			indices.push_back(next_row + j + 1);
		}
	}

	kengine::vattrib vIndices(VATTRIB_CAST(indices.data()), indices.size() * sizeof(GLushort), 1, GL_UNSIGNED_SHORT, GL_ARRAY_BUFFER);

	// creating the vbo
	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(5);
	vbo->load(0, vPositions);
	vbo->load(1, vTextureCoordinates);
	vbo->load(2, vNormals);
	vbo->load(3, vColors);
	vbo->load(4, vIndices);

	auto prim = std::make_unique<kengine::primitive_render>(std::move(vbo));

	prim->setMode(GL_TRIANGLES);
	assert(!(shaderInfo.find("POSITION") == shaderInfo.end()) && "shaderInfor key [POSITION] not found!");
	prim->loadAttributeInfo(shaderInfo["POSITION"], 0, vPositions);
	prim->setCount(vPositions.getVertexCount());

	assert(!(shaderInfo.find("TEXCOORD_0") == shaderInfo.end()) && "shaderInfor key [TEXCOORD_0] not found!");
	prim->loadAttributeInfo(shaderInfo["TEXCOORD_0"], 1, vTextureCoordinates);

	assert(!(shaderInfo.find("NORMAL") == shaderInfo.end()) && "shaderInfor key [NORMAL] not found!");
	prim->loadAttributeInfo(shaderInfo["NORMAL"], 2, vNormals);

	assert(!(shaderInfo.find("COLOR") == shaderInfo.end()) && "shaderInfor key [COLOR] not found!");
	prim->loadAttributeInfo(shaderInfo["COLOR"], 3, vColors);

	vIndices.setTarget(GL_ELEMENT_ARRAY_BUFFER);
	prim->bindBuffer(4, vIndices);
	prim->setElements(vIndices.getVertexCount());

	return prim;
}

std::unique_ptr<kengine::primitive_render> kengine::torus(size_t precision, GLfloat inner, GLfloat outer, std::unordered_map<std::string, GLuint>& shaderInfo)
{
	std::vector<GLfloat> positions;
	std::vector<GLfloat> textureCoordiantes;
	std::vector<GLfloat> tTangents;
	std::vector<GLfloat> sTangents;
	std::vector<GLfloat> normals;

	for (size_t i = 0; i < precision + 1; i++) {
		GLfloat angle = i * 360.0f / static_cast<GLfloat>(precision);
		kengine::matrix<GLfloat> r = kengine::rotate(0.0f, 0.0f, angle);

		kengine::vec3<GLfloat> startPos = r * kengine::vec3<GLfloat>(0.0f, outer, 0.0f);
		kengine::vec3<GLfloat> vertex(startPos + kengine::vec3<GLfloat>(inner, 0.0f, 0.0f));

		positions.push_back(vertex.x);
		positions.push_back(vertex.y);
		positions.push_back(vertex.z);

		textureCoordiantes.push_back(0.0f);
		textureCoordiantes.push_back(static_cast<GLfloat>(i) / static_cast<GLfloat>(precision));

		r = kengine::rotate(0.0f, 0.0f, angle + 90.0f);

		kengine::vec3<GLfloat> t = r * kengine::vec3<GLfloat>(0.0f, -1.0f, 0.0f);
		tTangents.push_back(t.x);
		tTangents.push_back(t.y);
		tTangents.push_back(t.z);

		kengine::vec3<GLfloat> s = kengine::vec3<GLfloat>(0.0f, 0.0f, -1.0f);
		sTangents.push_back(s.x);
		sTangents.push_back(s.y);
		sTangents.push_back(s.z);

		kengine::vec3<GLfloat> n = kengine::crossProduct(t, s);
		normals.push_back(n.x);
		normals.push_back(n.y);
		normals.push_back(n.z);
	}

	// rotate the first ring about Y to get the other rings
	for (size_t ring = 1; ring < precision + 1; ring++) {
		for (size_t vert = 0; vert < precision + 1; vert++) {
			GLfloat angle = static_cast<GLfloat>(ring * 360.0f / static_cast<GLfloat>(precision));
			kengine::matrix<GLfloat> r = kengine::rotate(0.0f, angle, 0.0f);

			kengine::vec3<GLfloat> vertex = r * vec3<GLfloat>(positions[vert * 3], positions[vert * 3 + 1], positions[vert * 3 + 2]);

			positions.push_back(vertex.x);
			positions.push_back(vertex.y);
			positions.push_back(vertex.z);

			float u = static_cast<GLfloat>(ring) * 2.0f / static_cast<GLfloat>(precision);
			float v = textureCoordiantes[vert * 2 + 1];

			textureCoordiantes.push_back(u);
			textureCoordiantes.push_back(v);

			r = kengine::rotate(0.0f, angle, 0.0f);
			kengine::vec3<GLfloat> sTangent = r * kengine::vec3<GLfloat>(sTangents[vert * 3], sTangents[vert * 3 + 1], sTangents[vert * 3 + 2]);
			sTangents.push_back(sTangent.x);
			sTangents.push_back(sTangent.y);
			sTangents.push_back(sTangent.z);

			kengine::vec3<GLfloat> tTangent = r * kengine::vec3<GLfloat>(tTangents[vert * 3], tTangents[vert * 3 + 1], tTangents[vert * 3 + 2]);
			tTangents.push_back(tTangent.x);
			tTangents.push_back(tTangent.y);
			tTangents.push_back(tTangent.z);

			kengine::vec3<GLfloat> n = r * kengine::vec3<GLfloat>(normals[vert * 3], normals[vert * 3 + 1], normals[vert * 3 + 2]);
			normals.push_back(n.x);
			normals.push_back(n.y);
			normals.push_back(n.z);
		}
	}

	kengine::vattrib vPositions(VATTRIB_CAST(positions.data()), positions.size() * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);
	kengine::vattrib vTextureCoordinates(VATTRIB_CAST(textureCoordiantes.data()), textureCoordiantes.size() * sizeof(GLfloat), 2, GL_FLOAT, GL_ARRAY_BUFFER);
	kengine::vattrib vNormals(VATTRIB_CAST(normals.data()), normals.size() * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	// calculating indices
	std::vector<GLushort> indices;

	for (int ring = 0; ring < precision; ring++) {
		for (int vert = 0; vert < precision; vert++) {

			GLushort row = ring * (precision + 1);
			GLushort next_row = (ring + 1) * (precision + 1);

			// first triangle (CCW visto de fora)
			indices.push_back(row + vert);
			indices.push_back(next_row + vert);
			indices.push_back(row + vert + 1);
			// second triangle
			indices.push_back(row + vert + 1);
			indices.push_back(next_row + vert);
			indices.push_back(next_row + vert + 1);
		}
	}

	kengine::vattrib vIndices(VATTRIB_CAST(indices.data()), indices.size() * sizeof(GLushort), 1, GL_UNSIGNED_SHORT, GL_ARRAY_BUFFER);

	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(4);
	vbo->load(0, vPositions);
	vbo->load(1, vTextureCoordinates);
	vbo->load(2, vNormals);
	vbo->load(3, vIndices);

	auto prim = std::make_unique<kengine::primitive_render>(std::move(vbo));

	prim->setMode(GL_TRIANGLES);
	assert(!(shaderInfo.find("POSITION") == shaderInfo.end()) && "shaderInfor key [POSITION] not found!");
	prim->loadAttributeInfo(shaderInfo["POSITION"], 0, vPositions);
	prim->setCount(vPositions.getVertexCount());

	assert(!(shaderInfo.find("TEXCOORD_0") == shaderInfo.end()) && "shaderInfor key [TEXCOORD_0] not found!");
	prim->loadAttributeInfo(shaderInfo["TEXCOORD_0"], 1, vTextureCoordinates);

	assert(!(shaderInfo.find("NORMAL") == shaderInfo.end()) && "shaderInfor key [NORMAL] not found!");
	prim->loadAttributeInfo(shaderInfo["NORMAL"], 2, vNormals);

	vIndices.setTarget(GL_ELEMENT_ARRAY_BUFFER);
	prim->bindBuffer(3, vIndices);
	prim->setElements(vIndices.getVertexCount());

	return prim;
}

std::unique_ptr<kengine::primitive_render> kengine::grid3D(std::unordered_map<std::string, GLuint>& shaderInfo)
{
	GLfloat positions[] = {
		-5000.0f, 0.0f, 0.0f,
		 5000.0f, 0.0f, 0.0f,
		 0.0f, -5000.0f, 0.0f,
		 0.0f, 5000.0f, 0.0f,
		 0.0f, 0.0f, -5000.0f,
		 0.0f, 0.0f, 5000.0f
	};

	kengine::vattrib vPositions(VATTRIB_CAST(positions), 18 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat gridColors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f
	};

	kengine::vattrib vColors(VATTRIB_CAST(gridColors), 24 * sizeof(GLfloat), 4, GL_FLOAT, GL_ARRAY_BUFFER);

	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(2);
	vbo->load(0, vPositions);
	vbo->load(1, vColors);

	auto prim = std::make_unique<kengine::primitive_render>(std::move(vbo));

	assert(!(shaderInfo.find("POSITION") == shaderInfo.end()) && "shaderInfor key [POSITION] not found!");
	prim->loadAttributeInfo(shaderInfo["POSITION"], 0, vPositions);
	prim->setCount(vPositions.getVertexCount());
	prim->setMode(GL_LINES);
	assert(!(shaderInfo.find("COLOR") == shaderInfo.end()) && "shaderInfor key [COLOR] not found!");
	prim->loadAttributeInfo(shaderInfo["COLOR"], 1, vColors);

	return prim;
}

std::unique_ptr<kengine::primitive_render> kengine::frustum3D(const projection_info<float>& projInfo, std::unordered_map<std::string, GLuint>& shaderInfo)
{
	GLfloat left = projInfo.left;
	GLfloat right = projInfo.right;
	GLfloat bottom = projInfo.bottom;
	GLfloat top = projInfo.top;
	GLfloat zNear = projInfo.zNear;
	GLfloat zFar = projInfo.zFar;
	GLfloat scale = projInfo.zFar / projInfo.zNear;
	GLfloat leftFar = projInfo.left * scale;
	GLfloat rightFar = projInfo.right * scale;
	GLfloat topFar = projInfo.top * scale;
	GLfloat bottomFar = projInfo.bottom * scale;

	GLfloat positions[] = {
		0.0f, 0.0f, 0.0f, // 0
		left,  top,    -zNear, // 1
		left,  bottom, -zNear, // 2
		right, bottom, -zNear, // 3
		right, top,    -zNear, // 4
		leftFar,  topFar,    -zFar, // 5
		leftFar,  bottomFar, -zFar, // 6
		rightFar, bottomFar, -zFar, // 7
		rightFar, topFar,    -zFar // 8
	};

	kengine::vattrib vPositions(VATTRIB_CAST(positions), 9 * 3 * sizeof(GLfloat), 3, GL_FLOAT, GL_ARRAY_BUFFER);

	GLfloat colors[] = {
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f,
		0.6f, 0.8f, 1.0f, 1.0f
	};

	kengine::vattrib vColors(VATTRIB_CAST(colors), 9 * 4 * sizeof(GLfloat), 4, GL_FLOAT, GL_ARRAY_BUFFER);

	GLushort indices[] = {
		0, 5,
		0, 6,
		0, 7,
		0, 8,
		1, 2,
		4, 3,
		1, 4,
		2, 3,
		5, 6,
		8, 7,
		5, 8,
		6, 7,

		0, 1, 2, 3, 4, 5, 6, 7, 8
	};

	kengine::vattrib vIndices(VATTRIB_CAST(indices), 33 * sizeof(GLushort), 1, GL_UNSIGNED_SHORT, GL_ELEMENT_ARRAY_BUFFER);

	std::unique_ptr<kengine::gpu_buffer> vbo = std::make_unique<kengine::gpu_buffer>(3);
	vbo->load(0, vPositions);
	vbo->load(1, vColors);
	vbo->load(2, vIndices);

	std::unique_ptr<primitive_render> prim = std::make_unique<kengine::primitive_render>(std::move(vbo));

	prim->loadAttributeInfo(shaderInfo["POSITION"], 0, vPositions);
	prim->setCount(vPositions.getVertexCount());

	prim->loadAttributeInfo(shaderInfo["COLOR"], 1, vColors);

	prim->bindBuffer(2, vIndices);
	prim->setElements(vIndices.getVertexCount());

	prim->setMode(GL_LINES);
	return prim;
}