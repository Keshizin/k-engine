/*
	K-Engine Demonstration
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

#include <game.hpp>

#include <iostream>
#include <cassert>
#include <cstring>

demo::game::game(kengine::core* engine)
	:
	m_engine{ engine },
	m_window{ kengine::windowInstance() },
	m_projectionInfo{}
{
	m_engine->setEventsCallback(this);
}

demo::game::~game() {
	delete m_renderingSystem;
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
	auto c = kengine::cube(1.0f);
	node.load(c, 1);
}

void demo::game::update(const int64_t frameTime)
{
	static float angleY = 0.0f;

	angleY += 1.0f;

	if (angleY > 360.0f)
		angleY = 0.0f;

	kengine::matrix<float> modelMatrix = kengine::scale(angleY / 100.0f, angleY / 100.0f, angleY / 100.0f);
	modelMatrix = kengine::rotate(angleY, 0.0f, 1.0f, 0.0f) * modelMatrix;

	kengine::matrix<float> eyeMatrix = kengine::lookAt(
		kengine::vec4<float>(0.0f, 3.0f, 10.0f),
		kengine::vec4<float>(0.0f, 0.0f, 0.0f),
		kengine::vec4<float>(0.0f, 0.1f, 0.0f));
	
	kengine::matrix<float> projectionMatrix = kengine::frustum(m_projectionInfo.left, m_projectionInfo.right, m_projectionInfo.bottom, m_projectionInfo.top, m_projectionInfo.zNear, m_projectionInfo.zFar);

	m_shader.setUniform("model", false, modelMatrix.value());
	m_shader.setUniform("eye", false, eyeMatrix.value());
	m_shader.setUniform("projection", false, projectionMatrix.value());

	// ----------------------------------------------------------------------------
	//	rendering here
	// ----------------------------------------------------------------------------

	m_renderingSystem->clearBuffers();	

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	node.drawArrays();

// (!) Unificar a chamada abaixo de SwapBuffer para facilitar o usuário
#ifdef __ANDROID__
	m_renderingSystem.swapBuffers();
#else
	m_window->swapBuffers();
#endif
}

void demo::game::onFinishEvent()
{
	m_engine->stopMainLoop();
}

void demo::game::onWindowReady(kengine::window* window)
{
	/*
		É recomendável que o sistema de renderização seja criado no evento "onWindowReady".

		Nas plataformas Android, a janela é criada implicitamente pelo Android Surface. Portanto,
		este evento foi criado especificamente para notificafar o usuário quando a janela estiver disponível.
	*/

	kengine::compatibility_profile profile;
	profile.contextFlag = kengine::CONTEXT_FLAG::CONTEXT_DEBUG_BIT_ARB;
	profile.profileMask = kengine::CONTEXT_FLAG::CONTEXT_CORE_PROFILE_BIT_ABR;

	/*
		Rendering system
			- escolher o tipo de render(e.g. OpenGL, Vulkan, DirectX, etc)
			- passar o handle de os_window para criar o contexto de renderização. É possível passar múltiplos handles de janela?
	*/
	m_renderingSystem = new kengine::rendering_system(window);

	if (!m_renderingSystem->init(kengine::RENDERING_TYPE::OPENGL, profile))
	{
		K_LOG_OUTPUT_RAW("(!) Failed to init the rendering system!");
		assert("(!) SHOWSTOPPER ERROR (!)");
	}

	K_LOG_OUTPUT_RAW(m_renderingSystem->info(false));

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(kengine::debugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	glCullFace(GL_BACK);

	kengine::ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, KENGINE_SHADER_PATH_STR + "/shaders/vs_example.vert"},
		{GL_FRAGMENT_SHADER, KENGINE_SHADER_PATH_STR + "/shaders/fs_example.frag"},
		{GL_NONE, ""}
	};

	if (!m_shader.loadShaders(shaders)) {
		// (!) showstopper
		assert(false && "(!) SHOWSTOPPER - FAILED TO LOAD SHADERS!");
	}

	m_shader.print();
	m_shader.useProgram();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// setting the projection
	m_projectionInfo.left = -2.0f;
	m_projectionInfo.right = 2.0f;
	m_projectionInfo.bottom = -2.0f;
	m_projectionInfo.top = 2.0f;
	m_projectionInfo.zNear = 2.0f;
	m_projectionInfo.zFar = 100.0f;

	//glPointSize(5.0f);
	//glLineWidth(1.0f);
	//glFrontFace(GL_CCW);
	//glCullFace(GL_FRONT);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

void demo::game::onWindowDestroy()
{
	kengine::quitApplication(0);
}

void demo::game::onPauseEvent()
{
}

void demo::game::onResumeEvent()
{
}

void demo::game::closeButtonEvent()
{
	assert(!(m_engine == nullptr));

	m_renderingSystem->finish();
	m_window->destroy();
	//m_engine->stopMainLoop(); no android a janela é fechada 
}

void demo::game::onResizeWindowEvent(const int width, const int height)
{
}

void demo::game::onMoveWindowEvent(const int x, const int y)
{
}

void demo::game::onMouseMotionEvent(const int x, const int y)
{
}

void demo::game::onMouseButtonEvent(const int button, const kengine::MOUSE_ACTION& state, const int x, const int y)
{
}

void demo::game::onKeyboardEvent(const unsigned long long key, const int state)
{
}

void demo::game::onKeyboardSpecialEvent(const unsigned long long key, const int state)
{
}

void demo::game::debugMessage(const std::string& msg)
{
	std::string message = "> debug message: " + msg;
	K_LOG_OUTPUT_RAW(message);
}

void demo::game::customWindowProcedure(void* param1, void* param2, void* param3, void* param4)
{
}