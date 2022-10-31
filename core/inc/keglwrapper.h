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

#ifndef K_ENGINE_OPENGL_WRAPPER_H
#define K_ENGINE_OPENGL_WRAPPER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <gl/gl.h>
#include <gl/glu.h>
#include <GLEXT/wglext.h>
#include <GLEXT/glext.h>


/*
* 
*  runtime function to get opengl procedures extension
* 
*/
void getProcedureAddress();


/*
* 
*  OpenGL Procedures Extension for Win32
* 
*/
extern PFNWGLSWAPINTERVALEXTPROC							wglSwapIntervalEXT;
extern PFNGLCLEARBUFFERFVPROC								glClearBufferfv;
extern PFNGLCREATEBUFFERSPROC								glCreateBuffers;
extern PFNGLGENBUFFERSPROC									glGenBuffers;
extern PFNGLDELETEBUFFERSARBPROC							glDeleteBuffers;
extern PFNGLISBUFFERPROC									glIsBuffer;
extern PFNGLBUFFERDATAPROC									glBufferData;
extern PFNGLNAMEDBUFFERSTORAGEPROC							glNamedBufferStorage;
extern PFNGLBUFFERSUBDATAPROC								glBufferSubData;
extern PFNGLNAMEDBUFFERSUBDATAPROC							glNamedBufferSubData;
extern PFNGLCLEARNAMEDBUFFERDATAPROC						glClearNamedBufferData;
extern PFNGLCLEARNAMEDBUFFERSUBDATAPROC						glClearNamedBufferSubData;
extern PFNGLCOPYNAMEDBUFFERSUBDATAPROC						glCopyNamedBufferSubData;
extern PFNGLGETBUFFERSUBDATAPROC							glGetBufferSubData;
extern PFNGLGETNAMEDBUFFERSUBDATAPROC						glGetNamedBufferSubData;
extern PFNGLCREATEVERTEXARRAYSPROC							glCreateVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC								glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC							glDeleteVertexArrays;
extern PFNGLISVERTEXARRAYPROC								glIsVertexArray;
extern PFNGLBINDVERTEXARRAYPROC								glBindVertexArray;
extern PFNGLBINDBUFFERPROC									glBindBuffer;
extern PFNGLVERTEXATTRIBPOINTERPROC							glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC						glEnableVertexAttribArray;
extern PFNGLBINDVERTEXBUFFERPROC							glBindVertexBuffer;
extern PFNGLVERTEXATTRIBFORMATPROC							glVertexAttribFormat;
extern PFNGLVERTEXATTRIBBINDINGPROC							glVertexAttribBinding;
extern PFNGLBINDATTRIBLOCATIONPROC							glBindAttribLocation;
extern PFNGLBINDFRAGDATALOCATIONPROC						glBindFragDataLocation;
extern PFNGLCREATESHADERPROC								glCreateShader;
extern PFNGLSHADERSOURCEPROC								glShaderSource;
extern PFNGLCOMPILESHADERPROC								glCompileShader;
extern PFNGLCREATEPROGRAMPROC								glCreateProgram;
extern PFNGLATTACHSHADERPROC								glAttachShader;
extern PFNGLDETACHSHADERPROC								glDetachShader;
extern PFNGLDELETESHADERPROC								glDeleteShader;
extern PFNGLISSHADERPROC									glIsShader;
extern PFNGLLINKPROGRAMPROC									glLinkProgram;
extern PFNGLUSEPROGRAMPROC									glUseProgram;
extern PFNGLDELETEPROGRAMPROC								glDeleteProgram;
extern PFNGLISPROGRAMPROC									glIsProgram;
extern PFNGLGETSHADERIVPROC									glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC							glGetShaderInfoLog;
extern PFNGLGETPROGRAMIVPROC								glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC							glGetProgramInfoLog;
extern PFNGLGETPROGRAMINTERFACEIVPROC						glGetProgramInterfaceiv;
extern PFNGLGETPROGRAMRESOURCEIVPROC						glGetProgramResourceiv;
extern PFNGLGETPROGRAMRESOURCENAMEPROC						glGetProgramResourceName;
extern PFNGLGETACTIVEATTRIBPROC								glGetActiveAttrib;
extern PFNGLGETATTRIBLOCATIONPROC							glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC							glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC							glUniformMatrix4fv;
extern PFNGLGETACTIVEUNIFORMSIVPROC							glGetActiveUniformsiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC						glGetActiveUniformName;
extern PFNGLGETUNIFORMBLOCKINDEXPROC						glGetUniformBlockIndex;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC						glGetActiveUniformBlockiv;
extern PFNGLGETUNIFORMINDICESPROC							glGetUniformIndices;
extern PFNGLBINDBUFFERBASEPROC								glBindBufferBase;
extern PFNGLBINDBUFFERRANGEPROC								glBindBufferRange;
extern PFNGLUNIFORMBLOCKBINDINGPROC							glUniformBlockBinding;
extern PFNGLMAPBUFFERPROC									glMapBuffer;
extern PFNGLUNMAPBUFFERPROC									glUnmapBuffer;
extern PFNGLMAPBUFFERRANGEPROC								glMapBufferRange;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC						glFlushMappedBufferRange;
extern PFNGLINVALIDATEBUFFERDATAPROC						glInvalidateBufferData;
extern PFNGLINVALIDATEBUFFERSUBDATAPROC						glInvalidadeBufferSubData;
extern PFNGLVERTEXATTRIBIPOINTERPROC						glVertexAttribIPointer;
extern PFNGLVERTEXATTRIBLPOINTERPROC						glVertexAttribLPointer;
extern PFNGLDRAWELEMENTSBASEVERTEXPROC						glDrawElementsBaseVertex;
extern PFNGLDRAWRANGEELEMENTSPROC							glDrawRangeElements;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC					glDrawRangeElementsBaseVertex;
extern PFNGLDRAWARRAYSINSTANCEDPROC							glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC						glDrawElementsInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC				glDrawElementsInstancedBaseVertex;
extern PFNGLDRAWARRAYSINDIRECTPROC							glDrawArraysIndirect;
extern PFNGLDRAWELEMENTSINDIRECTPROC						glDrawElementIndirect;
extern PFNGLMULTIDRAWARRAYSPROC								glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC							glMultiDrawElements;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC					glMultiDrawElementsBaseVertex;
extern PFNGLMULTIDRAWARRAYSINDIRECTPROC						glMultiDrawArraysIndirect;
extern PFNGLMULTIDRAWELEMENTSINDIRECTPROC					glMultiDrawElementsIndirect;
extern PFNGLPRIMITIVERESTARTINDEXPROC						glPrimitiveRestartIndex;
extern PFNGLVERTEXATTRIBDIVISORPROC							glVertexAttribDivisor;
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC				glDrawArraysInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC			glDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC	glDrawElementsInstancedBaseVertexInstance;
extern PFNGLCREATETEXTURESPROC								glCreateTextures;
extern PFNGLTEXTURESTORAGE1DPROC							glTextureStorage1D;
extern PFNGLTEXTURESTORAGE2DPROC							glTextureStorage2D;
extern PFNGLTEXTURESTORAGE3DPROC							glTextureStorage3D;
extern PFNGLTEXTURESUBIMAGE1DPROC							glTextureSubImage1D;
extern PFNGLTEXTURESUBIMAGE2DPROC							glTextureSubImage2D;
extern PFNGLTEXTURESUBIMAGE3DPROC							glTextureSubImage3D;
extern PFNGLBINDTEXTUREUNITPROC								glBindTextureUnit;
extern PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC					glTextureStorage2DMultisample;
extern PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC					glTextureStorage3DMultisample;
extern PFNGLGETTEXTUREIMAGEPROC								glGetTextureImage;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC						glCompressedTextureSubImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC						glCompressedTextureSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC						glCompressedTextureSubImage3D;
extern PFNGLCREATESAMPLERSPROC								glCreateSamplers;
extern PFNGLBINDSAMPLERPROC									glBindSampler;
extern PFNGLBINDSAMPLERSPROC								glBindSamplers;
extern PFNGLISSAMPLERPROC									glIsSampler;
extern PFNGLDELETESAMPLERSPROC								glDeleteSamplers;
extern PFNGLACTIVETEXTUREPROC								glActiveTexture;
extern PFNGLTEXSTORAGE2DPROC								glTexStorage2D;
extern PFNGLBLENDFUNCSEPARATEPROC							glBlendFuncSeparate;
extern PFNGLBLENDFUNCIPROC									glBlendFunci;
extern PFNGLBLENDFUNCSEPARATEIPROC							glBlendFuncSeparatei;
extern PFNGLBLENDEQUATIONPROC								glBlendEquation;
extern PFNGLBLENDEQUATIONSEPARATEPROC						glBlendEquationSeparate;
extern PFNGLBLENDEQUATIONIPROC								glBlendEquationi;
extern PFNGLBLENDEQUATIONSEPARATEIPROC						glBlendEquationSeparatei;
extern PFNGLTEXTUREPARAMETERFVPROC							glTextureParameterfv;
extern PFNGLTEXTUREPARAMETERIPROC							glTextureParameteri;
extern PFNGLGETSTRINGIPROC									glGetStringi;
extern PFNGLGETSUBROUTINEINDEXPROC							glGetSubroutineIndex;
extern PFNGLUNIFORMSUBROUTINESUIVPROC						glUniformSubroutinesuiv;
extern PFNGLUNIFORM1IPROC									glUniform1i;
extern PFNGLUNIFORM3FVPROC									glUniform3fv;
extern PFNGLUNIFORM3FPROC									glUniform3f;
extern PFNGLUNIFORM1FPROC									glUniform1f;

#endif