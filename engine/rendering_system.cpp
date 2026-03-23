/*
	K-Engine Rendering System
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

#include <rendering_system.hpp>
#include <os_api_wrapper.hpp>

#include <cassert>

/*
	kengine::rendering_system class - member class definition
*/

kengine::rendering_system::rendering_system(window* win)
{
	m_context = kengine::renderingContextInstance(win);
}

kengine::rendering_system::~rendering_system()
{
}

int kengine::rendering_system::init(RENDERING_TYPE renderingType, const compatibility_profile& profile)
{
	m_type = renderingType;
	return m_context->create(profile);

//#if defined(__ANDROID__)
//	context->create();
//	context->makeCurrent(true);
//#endif
}

void kengine::rendering_system::finish()
{
	m_context.reset();
}

void kengine::rendering_system::swapBuffers()
{
	assert(!(m_context == nullptr)); // remove branching code in the release version
	m_context->swapBuffers();
}
