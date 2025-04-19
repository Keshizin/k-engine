/*
	K-Engine Demonstration
	This file provide a splash screen example created with K-Engine.

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

#include <splash.hpp>
#include <logger.hpp>
#include <media.hpp>

#include <cassert>

demo::splash::splash(kengine::core* engine)
	:
	m_engine{ engine },
	m_window{ kengine::windowInstance() }
{
	m_engine->setEventsCallback(this);
}

demo::splash::~splash() {
	delete m_renderingSystem;
	delete m_window;
}

void demo::splash::start()
{
	m_engine->startMainLoop();
}

void demo::splash::createWindow() {
	m_window->create(
		kengine::getDisplayCenterPosX(640),
		kengine::getDisplayCenterPosY(480),
		640,
		480,
		"K-Engine! " + kengine::version(),
		kengine::window_style::SPLASH);
}

void demo::splash::showWindow() {
	m_window->show();
}

void demo::splash::onWindowReady(kengine::window* window)
{
	/*
		É recomendável que o sistema de renderização seja criado no evento "onWindowReady".

		Nas plataformas Android, a janela é criada implicitamente pelo Android Surface. Portanto,
		este evento foi criado especificamente para notificafar o usuário quando a janela estiver disponível.
	*/

	kengine::compatibility_profile profile;
	profile.contextFlag = kengine::CONTEXT_FLAG::CONTEXT_DEBUG_BIT_ARB;
	profile.profileMask = kengine::CONTEXT_FLAG::CONTEXT_CORE_PROFILE_BIT_ABR;

	if (m_renderingSystem != nullptr)
		delete m_renderingSystem;

	m_renderingSystem = new kengine::rendering_system(window);

	if (!m_renderingSystem->init(kengine::RENDERING_TYPE::OPENGL, profile))
	{
		K_LOG_OUTPUT_RAW("(!) Failed to init the rendering system!");
		assert("(!) SHOWSTOPPER ERROR (!)");
	}

	kengine::setVSync(1);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(kengine::debugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	kengine::ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, std::string(K_PROJECT_PATH) + "/shaders/splash.vert"},
		{GL_FRAGMENT_SHADER,  std::string(K_PROJECT_PATH) + "/shaders/splash.frag"},
		{GL_NONE, ""}
	};

	if (!m_shader.loadShaders(shaders)) {
		assert(false && "(!) SHOWSTOPPER - FAILED TO LOAD SHADERS!");
	}

	m_shader.useProgram();

	/*
		Projection and Camera
	*/

	m_cam.eye.x = 0.0f;
	m_cam.eye.y = 0.0f;
	m_cam.eye.z = 1.0f;
	m_cam.center.x = 0.0f;
	m_cam.center.y = 0.0f;
	m_cam.center.z = 0.0f;
	m_cam.up.x = 0.0f;
	m_cam.up.y = 1.0f;
	m_cam.up.z = 0.0f;

	m_shader.setUniform("view", false, m_cam.lookAt().value());

	float w = static_cast<float>(m_window->getWidth());
	float h = static_cast<float>(m_window->getHeight());

	m_cam.m_projectionInfo.left = -w / 2.0f;
	m_cam.m_projectionInfo.right = w / 2.0f;
	m_cam.m_projectionInfo.bottom = -h / 2.0f;
	m_cam.m_projectionInfo.top = h / 2.0f;
	m_cam.m_projectionInfo.zNear = 0.1f;
	m_cam.m_projectionInfo.zFar = 100.0f;
	m_cam.m_projectionInfo.width = w;
	m_cam.m_projectionInfo.height = h;

	m_shader.setUniform("projection", false, m_cam.getProjection(kengine::PROJECTION_TYPE::_2D, false).value());

	glViewport(0, 0, m_window->getWidth(), m_window->getHeight());

	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float red = ((kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0xFF000000) >> 24) / 256.0f;
	float green = ((kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0x00FF0000) >> 16) / 256.0f;
	float blue = ((kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0x0000FF00) >> 8) / 256.0f;
	float alpha = (kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0x000000FF) / 256.0f;

	glClearColor(red, green, blue, alpha);
}

void demo::splash::beforeMainLoopEvent()
{
	auto c = kengine::quad(270.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	logo.load(c, 1);

	kengine::k_img img;
	img.loadfile(std::string(K_PROJECT_PATH) + "/assets/shisabranco1080_fullhd.png");
	logoTexture.create(kengine::TEXTURE_TYPE::_2D, 0, GL_RGBA8, img.getWidth(), img.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, img.getPixels());
	logoTexture.bindTextureUnit(0);

	m_timer.setTimerInMs(5000);
	m_timer.start();

	m_circlePoints.generate(16);
}

void demo::splash::update(int64_t frameTime)
{
	if (m_timer.done()) {
		m_renderingSystem->finish();
		m_window->destroy();
		return;
	}
	
	animationTimer += static_cast<float>(frameTime) / static_cast<float>(kengine::getHighResolutionTimerFrequency());

	static size_t pointIndex = 0;

	if (animationTimer >= 0.02f) {
		animationTimer = 0.0f;

		kengine::vec4<float> p = m_circlePoints.getPoint(pointIndex++);
		logo.updateModelMatrix(kengine::translate(p.x * 7.5f, p.y * 7.5f, 0.0f));

		if (pointIndex > 15)
			pointIndex = 0;
	}

	m_renderingSystem->clearBuffers();
	logo.drawElements();

	// (!) Unificar a chamada abaixo de SwapBuffer para facilitar o usuário
#ifdef __ANDROID__
	m_renderingSystem.swapBuffers();
#else
	m_window->swapBuffers();
#endif
}

void demo::splash::onFinishEvent()
{
	m_engine->stopMainLoop();
}

void demo::splash::onWindowDestroy()
{
	kengine::quitApplication(0);
}

void demo::splash::onPauseEvent()
{
}

void demo::splash::onResumeEvent()
{
}

void demo::splash::closeButtonEvent()
{
	assert(!(m_engine == nullptr));
	m_renderingSystem->finish();
	m_window->destroy();
	//m_engine->stopMainLoop(); no android a janela é fechada 
}

void demo::splash::onResizeWindowEvent(const int width, const int height)
{
	m_window->setWindowSize(width, height);

	float w = static_cast<float>(m_window->getWidth());
	float h = static_cast<float>(m_window->getHeight());

	m_cam.m_projectionInfo.left = -w / 2.0f;
	m_cam.m_projectionInfo.right = w / 2.0f;
	m_cam.m_projectionInfo.bottom = -h / 2.0f;
	m_cam.m_projectionInfo.top = h / 2.0f;
	m_cam.m_projectionInfo.zNear = 0.1f;
	m_cam.m_projectionInfo.zFar = 100.0f;
	m_cam.m_projectionInfo.width = w;
	m_cam.m_projectionInfo.height = h;

	if (m_renderingSystem == nullptr)
		return;

	m_shader.useProgram();
	m_shader.setUniform("projection", false, m_cam.getProjection(kengine::PROJECTION_TYPE::_2D, false).value());
	glViewport(0, 0, width, height);
}

void demo::splash::onMoveWindowEvent(const int x, const int y)
{
}

void demo::splash::onMouseMotionEvent(const int x, const int y)
{
}

void demo::splash::onMouseButtonEvent(const int button, const kengine::MOUSE_ACTION& state, const int x, const int y)
{
}

void demo::splash::onKeyboardEvent(const unsigned long long key, const int state)
{
	if (key == kengine::KEY::ENTER && state)
	{
		m_renderingSystem->finish();
		m_window->destroy();
	}
}

void demo::splash::onKeyboardSpecialEvent(const unsigned long long key, const int state)
{
}

void demo::splash::debugMessage(const std::string& msg)
{
	std::string message = "> debug message: " + msg;
	K_LOG_OUTPUT_RAW(message);
}

void demo::splash::customWindowProcedure(void* param1, void* param2, void* param3, void* param4)
{
}