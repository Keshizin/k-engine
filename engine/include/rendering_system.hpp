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

#include <gl_wrapper.hpp>

namespace kengine
{
	class rendering_context;
	class window;

	enum class RENDERING_TYPE
	{
		OPENGL,
		OPENGL_ES,
		VULKAN
	};

	/*
		kengine::rendering_system class

			- Escolher entre tipos de renderizador (OpenGL, Vulkan, DirectX, etc)
			- Cria��o do contexto de acordo o renderizador e a janela de aplica��o (por exemplo, OpenGL context)
			- Possibilidade de gerenciar m�ltiplos contextos? (future)
	*/
	class rendering_system
	{
	public:
		explicit rendering_system(window* win);
		~rendering_system();

		rendering_system(const rendering_system& copy) = delete; // copy constructor
		rendering_system(rendering_system&& move) noexcept = delete; // move constructor
		rendering_system& operator=(const rendering_system& copy) = delete; // copy assignment
		rendering_system& operator=(rendering_system&&) = delete; // move assigment

		int init(RENDERING_TYPE type, const compatibility_profile& profile);
		void finish();
		void swapBuffers();
		void clearBuffers();

		std::string info(bool extension);

	private:
		RENDERING_TYPE m_type = RENDERING_TYPE::OPENGL;
		rendering_context* m_context = nullptr;
	};
}