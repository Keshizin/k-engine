/*
	K-Engine Rendering System
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

#include <rendering_system.hpp>
#include <os_api_wrapper.hpp>

#include <cassert>
#include <sstream>

/*
	kengine::rendering_system class - member class definition
*/

kengine::rendering_system::rendering_system()
	: type{ RENDERING_TYPE::OPENGL }
{
}

kengine::rendering_system::~rendering_system()
{
	delete context;
}

int kengine::rendering_system::init(window* win, RENDERING_TYPE renderingType, const compatibility_profile& profile)
{
	type = renderingType;

	if (context != nullptr)
		delete context;

	context = kengine::renderingContextInstance(win);
	context->create(profile);
	
//#if defined(__ANDROID__)
//	context->create();
//	context->makeCurrent(true);
//#endif

	return 1;
}

void kengine::rendering_system::finish()
{
	//context->makeCurrent(false);
	delete context;
	context = nullptr;
}

int kengine::rendering_system::swapBuffers()
{
	if (context == nullptr)
		return 0;

	context->swapBuffers();
	return 1;
}

void kengine::rendering_system::clearBuffers()
{
	if (context == nullptr)
		return;

	context->clearBuffers();
}

std::string kengine::rendering_system::info(bool extension)
{
	std::string info;

	if (context) {
		std::stringstream renderingInfo;

		const GLubyte* vendor = glGetString(GL_VENDOR);
		renderingInfo << "> GL vendor: " << vendor << "\n";

		const GLubyte* renderer = glGetString(GL_RENDERER);
		renderingInfo << "> GL renderer: " << renderer << "\n";

		const GLubyte* version = glGetString(GL_VERSION);
		renderingInfo << "> GL version: " << version << "\n";

		const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		renderingInfo << "> GLSL version: " << glslVersion << "\n";

		GLint maxVertexAttribs;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);

		renderingInfo << "\n> Max GL vertex attributes: " << maxVertexAttribs << "\n";

		if (extension) {
			GLint numExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
			renderingInfo << "\n> GL_NUM_EXTENSIONS: " << numExtensions << "\n";

			for (GLint i = 0; i < numExtensions; i++) {
				renderingInfo << "> " << glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)) << "\n";
			}
		}

		info = renderingInfo.str();
	}

	return info;
}