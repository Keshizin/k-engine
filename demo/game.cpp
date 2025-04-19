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

#define GAME_WINDOW_WIDTH 1280
#define GAME_WINDOW_HEIGHT 720

demo::game::game(kengine::core* engine)
	:
	m_engine{ engine },
	m_window{ kengine::windowInstance() }
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

void demo::game::createWindow() {
	m_window->create(
		kengine::getDisplayCenterPosX(GAME_WINDOW_WIDTH),
		kengine::getDisplayCenterPosY(GAME_WINDOW_HEIGHT),
		GAME_WINDOW_WIDTH,
		GAME_WINDOW_HEIGHT,
		"K-Engine! " + kengine::version(),
		kengine::window_style::DEFAULT);
}

void demo::game::showWindow() {
	m_window->show();
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

	kengine::setVSync(0);

	K_LOG_OUTPUT_RAW(m_renderingSystem->info(false));

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(kengine::debugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	kengine::ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, std::string(K_PROJECT_PATH) + "/shaders/main.vert"},
		{GL_FRAGMENT_SHADER, std::string(K_PROJECT_PATH) + "/shaders/main.frag"},
		{GL_NONE, ""}
	};

	if (!m_shader.loadShaders(shaders)) {
		assert(false && "(!) SHOWSTOPPER - FAILED TO LOAD SHADERS!");
	}

	m_shader.print();
	m_shader.useProgram();

	m_engineGUI.init(m_window->getHandle());
	m_engineGUI.setProfile(m_engine->getProfile());

	m_cam.eye.x = 0.0f;
	m_cam.eye.y = 0.0f;
	m_cam.eye.z = 3.0f;
	m_cam.center.x = 0.0f;
	m_cam.center.y = 0.0f;
	m_cam.center.z = 0.0f;
	m_cam.up.x = 0.0f;
	m_cam.up.y = 1.0f;
	m_cam.up.z = 0.0f;

	m_shader.setUniform("view", false, m_cam.lookAt().value());

	m_cam.m_projectionInfo.left = -5.0f;
	m_cam.m_projectionInfo.right = 5.0f;
	m_cam.m_projectionInfo.bottom = -5.0f;
	m_cam.m_projectionInfo.top = 5.0f;
	m_cam.m_projectionInfo.zNear = 2.0f;
	m_cam.m_projectionInfo.zFar = 100.0f;

	m_engineGUI.setCamera(&m_cam);

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

std::vector<kengine::vec4<float>> positions;

void demo::game::beforeMainLoopEvent()
{
	auto c = kengine::cube(1.0f, 0.5f, 0.0f, 0.0f, 1.0f);
	node.load(c, TOTAL_ENTITY);

	srand(static_cast<unsigned int>(time(0)));

	for (int i = 0; i < TOTAL_ENTITY; i++) {
		float x = static_cast<float>(rand() % 10) - 5.0f;
		float y = static_cast<float>(rand() % 10) - 5.0f;
		float z = -static_cast<float>((rand() % 50));

		kengine::vec4<float> v(x, y, z);
		positions.push_back(v);
	}

	for (size_t i = 0; i < TOTAL_ENTITY; i++)
	{
		modelMatrices[i] = kengine::translate(positions[i].x, positions[i].y, positions[i].z);
	}

	node.updateModelMatrices(modelMatrices, 0, TOTAL_ENTITY);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void demo::game::update(int64_t frameTime)
{
	static float angleY = 0.0f;

	angleY += 0.005f;

	if (angleY > 360.0f)
		angleY = 0.0f;

	for (size_t i = 0; i < TOTAL_ENTITY; i++)
	{
		modelMatrices[i] = kengine::translate(positions[i].x, positions[i].y, positions[i].z) * kengine::rotate(angleY, 1.0f, 0.0f, 0.0f) * kengine::rotate(angleY, 0.0f, 1.0f, 0.0f);
	}

	m_renderingSystem->clearBuffers();
	node.updateModelMatrices(modelMatrices, 0, TOTAL_ENTITY);
	node.drawInstanced(TOTAL_ENTITY);

	// ---------------------------------------------------------------------------

	m_engineGUI.draw();

	if (m_engineGUI.getProjectionWidget()->mustBeUpdate()) {
		auto type = m_engineGUI.getProjectionWidget()->m_type;
		m_shader.setUniform("projection", false, m_engineGUI.getProjectionWidget()->m_cam->getProjection(type, true).value());
		m_engineGUI.getProjectionWidget()->setUpdate(false);
	}

	if (m_engineGUI.getProjectionWidget()->mustCameraBeUpdate()) {
		m_shader.setUniform("view", false, m_engineGUI.getProjectionWidget()->m_cam->lookAt().value());
		m_engineGUI.getProjectionWidget()->setCameraUpdate(false);
	}

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

	m_engineGUI.destroy();
	m_renderingSystem->finish();
	m_window->destroy();
	//m_engine->stopMainLoop(); no android a janela é fechada 
}

void demo::game::onResizeWindowEvent(const int width, const int height)
{
	m_window->setWindowSize(width, height);
	m_cam.m_projectionInfo.width = static_cast<float>(m_window->getWidth());
	m_cam.m_projectionInfo.height = static_cast<float>(m_window->getHeight());
	m_engineGUI.setCamera(&m_cam);
	
	if (m_renderingSystem == nullptr)
		return;

	m_shader.useProgram();
	m_shader.setUniform("projection", false, m_engineGUI.getProjectionWidget()->m_cam->getProjection(m_engineGUI.getProjectionWidget()->m_type, true).value());
	glViewport(0, 0, width, height);
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
	if (key == '0' && state)
	{
		m_engine->getProfile()->save();
	}
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
	kgui::customWndProcHandler(param1, param2, param3, param4);
}