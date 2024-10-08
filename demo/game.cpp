/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#include <game.hpp>
#include <iostream>
#include <cassert>

demo::game::game(kengine::core* engine)
	:
	m_engine{ engine },
	m_window{ kengine::windowInstance() },
	m_profile{}
{
	m_engine->setEventsCallback(this);
}

demo::game::~game() {
	delete m_window;
}

void demo::game::start()
{
	m_engine->startMainLoop();
}

void demo::game::createWindow(int x, int y, int width, int height, const std::string& name) {
	m_window->create(x, y, width, height, name);
}

void demo::game::showWindow() {
	m_window->show();
}

void demo::game::beforeMainLoopEvent()
{
}

void demo::game::update(int64_t frameTime)
{
	glClear(GL_COLOR_BUFFER_BIT);

#ifdef __ANDROID__
	m_renderingSystem.swapBuffers();
#else
	m_window->swapBuffers();
#endif
}

void demo::game::closeButtonEvent()
{
	assert(!(m_engine == nullptr));
	m_renderingSystem.finish();
	m_window->destroy();
	//m_engine->stopMainLoop(); no android a janela é fechada 
}

void demo::game::onFinishEvent()
{
	m_engine->stopMainLoop();
}

void demo::game::onResumeEvent()
{
}

void demo::game::onPauseEvent()
{
}

void demo::game::onResizeWindowEvent(int width, int height)
{
}

void demo::game::onMoveWindowEvent(int x, int y)
{
}

void demo::game::onMouseMotionEvent(int x, int y)
{
}

void demo::game::onMouseButtonEvent(int button, kengine::MOUSE_ACTION state, int x, int y)
{
}

void demo::game::onKeyboardEvent(unsigned long long key, int state)
{
}

void demo::game::onKeyboardSpecialEvent(unsigned long long key, int state)
{
}

void demo::game::onWindowReady(kengine::window* window)
{
	/*
		Nota: É recomendável que o sistema de renderização seja criado no evento "onWindowReady".
		Nas plataformas Android, a janela é criada implicitamente pelo Android Surface. Portanto,
		este evento foi criado especificamente para notificafar o usuário quando a janela estiver disponível.
	*/

	/*
		Rendering system
			- escolher o tipo de render(e.g. OpenGL, Vulkan, DirectX, etc)
			- passar o handle de os_window para criar o contexto de renderização. É possível passar múltiplos handles de janela?
	*/
	if (!m_renderingSystem.init(window, kengine::RENDERING_TYPE::OPENGL))
	{
		K_LOG_OUTPUT_RAW("(!) Failed to init the rendering system!");
	}

	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void demo::game::onWindowDestroy()
{
	kengine::quitApplication(0);
}

void demo::game::debugMessage(const std::string& msg)
{
	std::string message = "> debug message: " + msg;
	K_LOG_OUTPUT_RAW(message);
}