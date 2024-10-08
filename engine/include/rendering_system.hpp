/*
	K-Engine Rendering System
	This file is part of the K-Engine.

	Copyright (C) 2020-2024 Fabio Takeshi Ishikawa

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

	/*
		kengine::rendering_system class

		Support to:
			- OpenGL
			- Vulkan (not implemented yet)
			- DirectX (not implemented yet)

		Ideas:
			- inicialização do contexto
				- escolher entre tipos de renderizador (OpenGL, Vulkan, DirectX, etc)
				- inicialização apropriada do contexto de acordo com o renderizador e a janela de aplicação
			- possibilidade de gerenciar múltiplos contextos? (future)
	*/

	enum class RENDERING_TYPE
	{
		OPENGL,
		OPENGL_ES,
		VULKAN
	};

	class rendering_system
	{
	public:
		rendering_system();
		~rendering_system();

		rendering_system(const rendering_system& copy) = delete; // copy constructor
		rendering_system(rendering_system&& move) noexcept = delete; // move constructor
		rendering_system& operator=(const rendering_system& copy) = delete; // copy assignment
		rendering_system& operator=(rendering_system&&) = delete; // move assigment

		int init(window* win, RENDERING_TYPE type);
		void finish();
		int swapBuffers();
		
	private:
		RENDERING_TYPE type;
		rendering_context* context = nullptr;
	};
}