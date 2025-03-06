/*
	K-Engine Graphical User Interface
	This file provide a template for a game created with K-Engine.

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

#ifndef KENGINE_GUI_HPP
#define KENGINE_GUI_HPP

#include <imgui-1.91.8/imgui.h>
#include <imgui-1.91.8/backends/imgui_impl_win32.h>
#include <imgui-1.91.8/backends/imgui_impl_opengl3.h>

namespace KGUI {
	void init(void* windowHandle);
	void draw();
	void customWndProcHandler(void* param1, void* param2, void* param3, void* param4);
	void destroy();
}

#endif
