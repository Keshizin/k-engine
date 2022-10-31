/*
   K-Engine OpenGL Wrapper
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

#include <keglwrapper.h>
#include <keaux.h>


/*
*
*  OpenGL Procedures Extension for Win32
*
*/
PFNWGLSWAPINTERVALEXTPROC								wglSwapIntervalEXT = 0;
PFNGLCLEARBUFFERFVPROC									glClearBufferfv = 0;
PFNGLCREATEBUFFERSPROC									glCreateBuffers = 0;
PFNGLGENBUFFERSPROC										glGenBuffers = 0;
PFNGLDELETEBUFFERSARBPROC								glDeleteBuffers = 0;
PFNGLISBUFFERPROC										glIsBuffer = 0;
PFNGLBUFFERDATAPROC										glBufferData = 0;
PFNGLNAMEDBUFFERSTORAGEPROC								glNamedBufferStorage = 0;
PFNGLNAMEDBUFFERSUBDATAPROC								glNamedBufferSubData = 0;
PFNGLCLEARNAMEDBUFFERDATAPROC							glClearNamedBufferData = 0;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC						glClearNamedBufferSubData = 0;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC							glCopyNamedBufferSubData = 0;
PFNGLGETBUFFERSUBDATAPROC								glGetBufferSubData = 0;
PFNGLGETNAMEDBUFFERSUBDATAPROC							glGetNamedBufferSubData = 0;
PFNGLBUFFERSUBDATAPROC									glBufferSubData = 0;
PFNGLCREATEVERTEXARRAYSPROC								glCreateVertexArrays = 0;
PFNGLGENVERTEXARRAYSPROC								glGenVertexArrays = 0;
PFNGLDELETEVERTEXARRAYSPROC								glDeleteVertexArrays = 0;
PFNGLISVERTEXARRAYPROC									glIsVertexArray = 0;
PFNGLBINDVERTEXARRAYPROC								glBindVertexArray = 0;
PFNGLBINDBUFFERPROC										glBindBuffer = 0;
PFNGLVERTEXATTRIBPOINTERPROC							glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC						glEnableVertexAttribArray = 0;
PFNGLBINDVERTEXBUFFERPROC								glBindVertexBuffer = 0;
PFNGLVERTEXATTRIBFORMATPROC								glVertexAttribFormat = 0;
PFNGLVERTEXATTRIBBINDINGPROC							glVertexAttribBinding = 0;
PFNGLBINDATTRIBLOCATIONPROC								glBindAttribLocation = 0;
PFNGLBINDFRAGDATALOCATIONPROC							glBindFragDataLocation = 0;
PFNGLCREATESHADERPROC									glCreateShader = 0;
PFNGLSHADERSOURCEPROC									glShaderSource = 0;
PFNGLCOMPILESHADERPROC									glCompileShader = 0;
PFNGLCREATEPROGRAMPROC									glCreateProgram = 0;
PFNGLATTACHSHADERPROC									glAttachShader = 0;
PFNGLDETACHSHADERPROC									glDetachShader = 0;
PFNGLDELETESHADERPROC									glDeleteShader = 0;
PFNGLISSHADERPROC										glIsShader = 0;
PFNGLLINKPROGRAMPROC									glLinkProgram = 0;
PFNGLUSEPROGRAMPROC										glUseProgram = 0;
PFNGLDELETEPROGRAMPROC									glDeleteProgram = 0;
PFNGLISPROGRAMPROC										glIsProgram = 0;
PFNGLGETSHADERIVPROC									glGetShaderiv = 0;
PFNGLGETSHADERINFOLOGPROC								glGetShaderInfoLog = 0;
PFNGLGETPROGRAMIVPROC									glGetProgramiv = 0;
PFNGLGETPROGRAMINFOLOGPROC								glGetProgramInfoLog = 0;
PFNGLGETPROGRAMINTERFACEIVPROC							glGetProgramInterfaceiv = 0;
PFNGLGETPROGRAMRESOURCEIVPROC							glGetProgramResourceiv = 0;
PFNGLGETPROGRAMRESOURCENAMEPROC							glGetProgramResourceName = 0;
PFNGLGETACTIVEATTRIBPROC								glGetActiveAttrib = 0;
PFNGLGETATTRIBLOCATIONPROC								glGetAttribLocation = 0;
PFNGLGETUNIFORMLOCATIONPROC								glGetUniformLocation = 0;
PFNGLUNIFORMMATRIX4FVPROC								glUniformMatrix4fv = 0;
PFNGLGETACTIVEUNIFORMSIVPROC							glGetActiveUniformsiv = 0;
PFNGLGETACTIVEUNIFORMNAMEPROC							glGetActiveUniformName = 0;
PFNGLGETUNIFORMBLOCKINDEXPROC							glGetUniformBlockIndex = 0;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC						glGetActiveUniformBlockiv = 0;
PFNGLGETUNIFORMINDICESPROC								glGetUniformIndices = 0;
PFNGLBINDBUFFERBASEPROC									glBindBufferBase = 0;
PFNGLBINDBUFFERRANGEPROC								glBindBufferRange = 0;
PFNGLUNIFORMBLOCKBINDINGPROC							glUniformBlockBinding = 0;
PFNGLMAPBUFFERPROC										glMapBuffer = 0;
PFNGLUNMAPBUFFERPROC									glUnmapBuffer = 0;
PFNGLMAPBUFFERRANGEPROC									glMapBufferRange = 0;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC							glFlushMappedBufferRange = 0;
PFNGLINVALIDATEBUFFERDATAPROC							glInvalidateBufferData = 0;
PFNGLINVALIDATEBUFFERSUBDATAPROC						glInvalidadeBufferSubData = 0;
PFNGLVERTEXATTRIBIPOINTERPROC							glVertexAttribIPointer = 0;
PFNGLVERTEXATTRIBLPOINTERPROC							glVertexAttribLPointer = 0;
PFNGLDRAWELEMENTSBASEVERTEXPROC							glDrawElementsBaseVertex = 0;
PFNGLDRAWRANGEELEMENTSPROC								glDrawRangeElements = 0;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC					glDrawRangeElementsBaseVertex = 0;
PFNGLDRAWARRAYSINSTANCEDPROC							glDrawArraysInstanced = 0;
PFNGLDRAWELEMENTSINSTANCEDPROC							glDrawElementsInstanced = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC				glDrawElementsInstancedBaseVertex = 0;
PFNGLDRAWARRAYSINDIRECTPROC								glDrawArraysIndirect = 0;
PFNGLDRAWELEMENTSINDIRECTPROC							glDrawElementIndirect = 0;
PFNGLMULTIDRAWARRAYSPROC								glMultiDrawArrays = 0;
PFNGLMULTIDRAWELEMENTSPROC								glMultiDrawElements = 0;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC					glMultiDrawElementsBaseVertex = 0;
PFNGLMULTIDRAWARRAYSINDIRECTPROC						glMultiDrawArraysIndirect = 0;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC						glMultiDrawElementsIndirect = 0;
PFNGLPRIMITIVERESTARTINDEXPROC							glPrimitiveRestartIndex = 0;
PFNGLVERTEXATTRIBDIVISORPROC							glVertexAttribDivisor = 0;
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC				glDrawArraysInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC				glDrawElementsInstancedBaseInstance = 0;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC	glDrawElementsInstancedBaseVertexInstance = 0;
PFNGLCREATETEXTURESPROC									glCreateTextures = 0;
PFNGLTEXTURESTORAGE1DPROC								glTextureStorage1D = 0;
PFNGLTEXTURESTORAGE2DPROC								glTextureStorage2D = 0;
PFNGLTEXTURESTORAGE3DPROC								glTextureStorage3D = 0;
PFNGLTEXTURESUBIMAGE1DPROC								glTextureSubImage1D = 0;
PFNGLTEXTURESUBIMAGE2DPROC								glTextureSubImage2D = 0;
PFNGLTEXTURESUBIMAGE3DPROC								glTextureSubImage3D = 0;
PFNGLBINDTEXTUREUNITPROC								glBindTextureUnit = 0;
PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC					glTextureStorage2DMultisample = 0;
PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC					glTextureStorage3DMultisample = 0;
PFNGLGETTEXTUREIMAGEPROC								glGetTextureImage = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC						glCompressedTextureSubImage1D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC						glCompressedTextureSubImage2D = 0;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC						glCompressedTextureSubImage3D = 0;
PFNGLCREATESAMPLERSPROC									glCreateSamplers = 0;
PFNGLBINDSAMPLERPROC									glBindSampler = 0;
PFNGLBINDSAMPLERSPROC									glBindSamplers = 0;
PFNGLISSAMPLERPROC										glIsSampler = 0;
PFNGLDELETESAMPLERSPROC									glDeleteSamplers = 0;
PFNGLACTIVETEXTUREPROC									glActiveTexture = 0;
PFNGLTEXSTORAGE2DPROC									glTexStorage2D = 0;
PFNGLBLENDFUNCSEPARATEPROC								glBlendFuncSeparate = 0;
PFNGLBLENDFUNCIPROC										glBlendFunci = 0;
PFNGLBLENDFUNCSEPARATEIPROC								glBlendFuncSeparatei = 0;
PFNGLBLENDEQUATIONPROC									glBlendEquation = 0;
PFNGLBLENDEQUATIONSEPARATEPROC							glBlendEquationSeparate = 0;
PFNGLBLENDEQUATIONIPROC									glBlendEquationi = 0;
PFNGLBLENDEQUATIONSEPARATEIPROC							glBlendEquationSeparatei = 0;
PFNGLTEXTUREPARAMETERFVPROC								glTextureParameterfv = 0;
PFNGLTEXTUREPARAMETERIPROC								glTextureParameteri = 0;
PFNGLGETSTRINGIPROC										glGetStringi = 0;
PFNGLGETSUBROUTINEINDEXPROC								glGetSubroutineIndex = 0;
PFNGLUNIFORMSUBROUTINESUIVPROC							glUniformSubroutinesuiv = 0;
PFNGLUNIFORM1IPROC										glUniform1i = 0;
PFNGLUNIFORM3FVPROC										glUniform3fv = 0;
PFNGLUNIFORM3FPROC										glUniform3f = 0;
PFNGLUNIFORM1FPROC										glUniform1f = 0;


/*
*
*  runtime function to get opengl procedures extension
*
*/
void getProcedureAddress()
{
#pragma warning(disable: 4191)

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	glClearBufferfv = (PFNGLCLEARBUFFERFVPROC)wglGetProcAddress("glClearBufferfv");
	glCreateBuffers = (PFNGLCREATEBUFFERSPROC)wglGetProcAddress("glCreateBuffers");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffers");
	glIsBuffer = (PFNGLISBUFFERPROC)wglGetProcAddress("glIsBuffer");
	glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glNamedBufferStorage = (PFNGLNAMEDBUFFERSTORAGEPROC)wglGetProcAddress("glNamedBufferStorage");
	glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC)wglGetProcAddress("glNamedBufferSubData");
	glClearNamedBufferData = (PFNGLCLEARNAMEDBUFFERDATAPROC)wglGetProcAddress("glClearNamedBufferData");
	glClearNamedBufferSubData = (PFNGLCLEARNAMEDBUFFERSUBDATAPROC)wglGetProcAddress("glClearNamedBufferSubData");
	glCopyNamedBufferSubData = (PFNGLCOPYNAMEDBUFFERSUBDATAPROC)wglGetProcAddress("glCopyNamedBufferSubData");
	glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)wglGetProcAddress("glGetBufferSubData");
	glGetNamedBufferSubData = (PFNGLGETNAMEDBUFFERSUBDATAPROC)wglGetProcAddress("glGetNamedBufferSubData");
	glCreateVertexArrays = (PFNGLCREATEVERTEXARRAYSPROC)wglGetProcAddress("glCreateVertexArrays");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
	glIsVertexArray = (PFNGLISVERTEXARRAYPROC)wglGetProcAddress("glIsVertexArray");
	glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glBindVertexBuffer = (PFNGLBINDVERTEXBUFFERPROC)wglGetProcAddress("glBindVertexBuffer");
	glVertexAttribFormat = (PFNGLVERTEXATTRIBFORMATPROC)wglGetProcAddress("glVertexAttribFormat");
	glVertexAttribBinding = (PFNGLVERTEXATTRIBBINDINGPROC)wglGetProcAddress("glVertexAttribBinding");
	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
	glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
	glIsShader = (PFNGLISSHADERPROC)wglGetProcAddress("glIsShader");
	glIsProgram = (PFNGLISPROGRAMPROC)wglGetProcAddress("glIsProgram");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glBindFragDataLocation = (PFNGLBINDFRAGDATALOCATIONPROC)wglGetProcAddress("glBindFragDataLocation");
	glGetProgramInterfaceiv = (PFNGLGETPROGRAMINTERFACEIVPROC)wglGetProcAddress("glGetProgramInterfaceiv");
	glGetProgramResourceiv = (PFNGLGETPROGRAMRESOURCEIVPROC)wglGetProcAddress("glGetProgramResourceiv");
	glGetProgramResourceName = (PFNGLGETPROGRAMRESOURCENAMEPROC)wglGetProcAddress("glGetProgramResourceName");
	glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)wglGetProcAddress("glGetActiveAttrib");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glGetActiveUniformsiv = (PFNGLGETACTIVEUNIFORMSIVPROC)wglGetProcAddress("glGetActiveUniformsiv");
	glGetActiveUniformName = (PFNGLGETACTIVEUNIFORMNAMEPROC)wglGetProcAddress("glGetActiveUniformName");
	glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)wglGetProcAddress("glGetUniformBlockIndex");
	glGetActiveUniformBlockiv = (PFNGLGETACTIVEUNIFORMBLOCKIVPROC)wglGetProcAddress("glGetActiveUniformBlockiv");
	glGetUniformIndices = (PFNGLGETUNIFORMINDICESPROC)wglGetProcAddress("glGetUniformIndices");
	glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)wglGetProcAddress("glBindBufferBase");
	glBindBufferRange = (PFNGLBINDBUFFERRANGEPROC)wglGetProcAddress("glBindBufferRange");
	glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)wglGetProcAddress("glUniformBlockBinding");
	glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	glMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)wglGetProcAddress("glMapBufferRange");
	glFlushMappedBufferRange = (PFNGLFLUSHMAPPEDBUFFERRANGEPROC)wglGetProcAddress("glFlushMappedBufferRange");
	glInvalidateBufferData = (PFNGLINVALIDATEBUFFERDATAPROC)wglGetProcAddress("glInvalidateBufferData");
	glInvalidadeBufferSubData = (PFNGLINVALIDATEBUFFERSUBDATAPROC)wglGetProcAddress("glInvalidadeBufferSubData");
	glVertexAttribIPointer = (PFNGLVERTEXATTRIBIPOINTERPROC)wglGetProcAddress("glVertexAttribIPointer");
	glVertexAttribLPointer = (PFNGLVERTEXATTRIBLPOINTERPROC)wglGetProcAddress("glVertexAttribLPointer");
	glDrawElementsBaseVertex = (PFNGLDRAWELEMENTSBASEVERTEXPROC)wglGetProcAddress("glDrawElementsBaseVertex");
	glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC)wglGetProcAddress("glDrawRangeElements");
	glDrawRangeElementsBaseVertex = (PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)wglGetProcAddress("glDrawRangeElementsBaseVertex");
	glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)wglGetProcAddress("glDrawArraysInstanced");
	glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)wglGetProcAddress("glDrawElementsInstanced");
	glDrawElementsInstancedBaseVertex = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC)wglGetProcAddress("glDrawElementsInstancedBaseVertex");
	glDrawArraysIndirect = (PFNGLDRAWARRAYSINDIRECTPROC)wglGetProcAddress("glDrawArraysIndirect");
	glDrawElementIndirect = (PFNGLDRAWELEMENTSINDIRECTPROC)wglGetProcAddress("glDrawElementIndirect");
	glMultiDrawArrays = (PFNGLMULTIDRAWARRAYSPROC)wglGetProcAddress("glMultiDrawArrays");
	glMultiDrawElements = (PFNGLMULTIDRAWELEMENTSPROC)wglGetProcAddress("glMultiDrawElements");
	glMultiDrawElementsBaseVertex = (PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)wglGetProcAddress("glMultiDrawElementsBaseVertex");
	glMultiDrawArraysIndirect = (PFNGLMULTIDRAWARRAYSINDIRECTPROC)wglGetProcAddress("glMultiDrawArraysIndirect");
	glMultiDrawElementsIndirect = (PFNGLMULTIDRAWELEMENTSINDIRECTPROC)wglGetProcAddress("glMultiDrawElementsIndirect");
	glPrimitiveRestartIndex = (PFNGLPRIMITIVERESTARTINDEXPROC)wglGetProcAddress("glPrimitiveRestartIndex");
	glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");
	glDrawArraysInstancedBaseInstance = (PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC)wglGetProcAddress("glDrawArraysInstancedBaseInstance");
	glDrawElementsInstancedBaseInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC)wglGetProcAddress("glDrawElementsInstancedBaseInstance");
	glDrawElementsInstancedBaseVertexInstance = (PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC)wglGetProcAddress("glDrawElementsInstancedBaseVertexInstance");
	glCreateTextures = (PFNGLCREATETEXTURESPROC)wglGetProcAddress("glCreateTextures");
	glTextureStorage1D = (PFNGLTEXTURESTORAGE1DPROC)wglGetProcAddress("glTextureStorage1D");
	glTextureStorage2D = (PFNGLTEXTURESTORAGE2DPROC)wglGetProcAddress("glTextureStorage2D");
	glTextureStorage3D = (PFNGLTEXTURESTORAGE3DPROC)wglGetProcAddress("glTextureStorage3D");
	glBindTextureUnit = (PFNGLBINDTEXTUREUNITPROC)wglGetProcAddress("glBindTextureUnit");
	glTextureStorage2DMultisample = (PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC)wglGetProcAddress("glTextureStorage2DMultisample");
	glTextureStorage3DMultisample = (PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC)wglGetProcAddress("glTextureStorage3DMultisample");
	glTextureSubImage1D = (PFNGLTEXTURESUBIMAGE1DPROC)wglGetProcAddress("glTextureSubImage1D");
	glTextureSubImage2D = (PFNGLTEXTURESUBIMAGE2DPROC)wglGetProcAddress("glTextureSubImage2D");
	glTextureSubImage3D = (PFNGLTEXTURESUBIMAGE3DPROC)wglGetProcAddress("glTextureSubImage3D");
	glGetTextureImage = (PFNGLGETTEXTUREIMAGEPROC)wglGetProcAddress("glGetTextureImage");
	glCompressedTextureSubImage1D = (PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC)wglGetProcAddress("glCompressedTextureSubImage1D");
	glCompressedTextureSubImage2D = (PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC)wglGetProcAddress("glCompressedTextureSubImage2D");
	glCompressedTextureSubImage3D = (PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC)wglGetProcAddress("glCompressedTextureSubImage3D");
	glCreateSamplers = (PFNGLCREATESAMPLERSPROC)wglGetProcAddress("glCreateSamplers");
	glBindSampler = (PFNGLBINDSAMPLERPROC)wglGetProcAddress("glBindSampler");
	glBindSamplers = (PFNGLBINDSAMPLERSPROC)wglGetProcAddress("glBindSamplers");
	glIsSampler = (PFNGLISSAMPLERPROC)wglGetProcAddress("glIsSampler");
	glDeleteSamplers = (PFNGLDELETESAMPLERSPROC)wglGetProcAddress("glDeleteSamplers");
	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	glTexStorage2D = (PFNGLTEXSTORAGE2DPROC)wglGetProcAddress("glTexStorage2D");
	glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)wglGetProcAddress("glBlendFuncSeparate");
	glBlendFunci = (PFNGLBLENDFUNCIPROC)wglGetProcAddress("glBlendFunci");
	glBlendFuncSeparatei = (PFNGLBLENDFUNCSEPARATEIPROC)wglGetProcAddress("glBlendFuncSeparatei");
	glBlendEquation = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquation");
	glBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)wglGetProcAddress("glBlendEquationSeparate");
	glBlendEquationi = (PFNGLBLENDEQUATIONIPROC)wglGetProcAddress("glBlendEquationi");
	glBlendEquationSeparatei = (PFNGLBLENDEQUATIONSEPARATEIPROC)wglGetProcAddress("glBlendEquationSeparatei");
	glTextureParameterfv = (PFNGLTEXTUREPARAMETERFVPROC)wglGetProcAddress("glTextureParameterfv");
	glTextureParameteri = (PFNGLTEXTUREPARAMETERIPROC)wglGetProcAddress("glTextureParameteri");
	glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");
	glGetSubroutineIndex = (PFNGLGETSUBROUTINEINDEXPROC)wglGetProcAddress("glGetSubroutineIndex");
	glUniformSubroutinesuiv = (PFNGLUNIFORMSUBROUTINESUIVPROC)wglGetProcAddress("glUniformSubroutinesuiv");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
	glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
	glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");

#pragma warning(default: 4191)

	if (!wglSwapIntervalEXT &&
		!glClearBufferfv &&
		!glCreateBuffers &&
		!glGenBuffers &&
		!glDeleteBuffers &&
		!glIsBuffer &&
		!glNamedBufferStorage &&
		!glNamedBufferSubData &&
		!glBufferData &&
		!glBufferSubData &&
		!glClearNamedBufferData &&
		!glClearNamedBufferSubData &&
		!glCopyNamedBufferSubData &&
		!glGetBufferSubData &&
		!glGetNamedBufferSubData &&
		!glCreateVertexArrays &&
		!glGenVertexArrays &&
		!glDeleteVertexArrays &&
		!glIsVertexArray &&
		!glBindVertexArray &&
		!glBindBuffer &&
		!glVertexAttribPointer &&
		!glEnableVertexAttribArray &&
		!glBindVertexBuffer &&
		!glVertexAttribFormat &&
		!glVertexAttribBinding &&
		!glBindAttribLocation &&
		!glBindFragDataLocation &&
		!glCreateShader &&
		!glShaderSource &&
		!glCompileShader &&
		!glCreateProgram &&
		!glAttachShader &&
		!glDetachShader &&
		!glDeleteShader &&
		!glIsShader &&
		!glLinkProgram &&
		!glUseProgram &&
		!glDeleteProgram &&
		!glIsProgram &&
		!glGetShaderiv &&
		!glGetShaderInfoLog &&
		!glGetProgramiv &&
		!glGetProgramInfoLog &&
		!glGetProgramInterfaceiv &&
		!glGetProgramResourceiv &&
		!glGetProgramResourceName &&
		!glGetActiveAttrib &&
		!glGetAttribLocation &&
		!glGetUniformLocation &&
		!glUniformMatrix4fv &&
		!glGetActiveUniformsiv &&
		!glGetActiveUniformName &&
		!glGetUniformBlockIndex &&
		!glGetActiveUniformBlockiv &&
		!glGetUniformIndices &&
		!glBindBufferBase &&
		!glBindBufferRange &&
		!glUniformBlockBinding &&
		!glMapBuffer &&
		!glUnmapBuffer &&
		!glMapBufferRange &&
		!glFlushMappedBufferRange &&
		!glInvalidateBufferData &&
		!glInvalidadeBufferSubData &&
		!glVertexAttribIPointer &&
		!glVertexAttribLPointer &&
		!glDrawElementsBaseVertex &&
		!glDrawRangeElements &&
		!glDrawRangeElementsBaseVertex &&
		!glDrawArraysInstanced &&
		!glDrawElementsInstanced &&
		!glDrawElementsInstancedBaseVertex &&
		!glDrawArraysIndirect &&
		!glDrawElementIndirect &&
		!glMultiDrawArrays &&
		!glMultiDrawElements &&
		!glMultiDrawElementsBaseVertex &&
		!glMultiDrawArraysIndirect &&
		!glMultiDrawElementsIndirect &&
		!glPrimitiveRestartIndex &&
		!glVertexAttribDivisor &&
		!glDrawArraysInstancedBaseInstance &&
		!glDrawElementsInstancedBaseInstance &&
		!glDrawElementsInstancedBaseVertexInstance &&
		!glCreateTextures &&
		!glTextureStorage1D &&
		!glTextureStorage2D &&
		!glTextureStorage3D &&
		!glTextureSubImage1D &&
		!glTextureSubImage2D &&
		!glTextureSubImage3D &&
		!glBindTextureUnit &&
		!glTextureStorage2DMultisample &&
		!glTextureStorage3DMultisample &&
		!glGetTextureImage &&
		!glCompressedTextureSubImage1D &&
		!glCompressedTextureSubImage2D &&
		!glCompressedTextureSubImage3D &&
		!glCreateSamplers &&
		!glBindSampler &&
		!glBindSamplers &&
		!glIsSampler &&
		!glDeleteSamplers &&
		!glActiveTexture &&
		!glTexStorage2D &&
		!glBlendFuncSeparate &&
		!glBlendFunci &&
		!glBlendFuncSeparatei &&
		!glBlendEquation &&
		!glBlendEquationSeparate &&
		!glBlendEquationi &&
		!glBlendEquationSeparatei &&
		!glTextureParameterfv &&
		!glTextureParameteri &&
		!glGetStringi &&
		!glGetSubroutineIndex &&
		!glUniformSubroutinesuiv &&
		!glUniform1i &&
		!glUniform3fv &&
		!glUniform3f &&
		!glUniform1f)
	{
		K_DEBUG_OUTPUT(K_DEBUG_ERROR, "It was not possible to load GL extension: " << glGetError());
	}
}