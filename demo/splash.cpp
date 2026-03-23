/*
	K-Engine Demonstration
	This file provide a splash screen example created with K-Engine.

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

#include <splash.hpp>
#include <logger.hpp>

#include <cassert>

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#pragma warning(pop)

demo::splash::splash(kengine::core* engine)
	: m_engine{ engine }, m_window{ kengine::windowInstance() }
{
	assert(!(m_engine == nullptr) && "splash constructor failed - m_engine is nullptr");
	m_engine->setEventsCallback(this);
}

demo::splash::~splash() {
}

void demo::splash::start()
{
	m_engine->startMainLoop();
}

void demo::splash::createWindow() {
	// se a janela foi criada anteriormente, deve ser destruida antes de chamar ela novamente
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
		este evento foi criado especificamente para notificar o usuário quando a janela estiver disponível.
	*/

	kengine::compatibility_profile profile;
	//profile.contextFlag = kengine::CONTEXT_FLAG::CONTEXT_DEBUG_BIT_ARB;
	profile.profileMask = kengine::CONTEXT_FLAG::CONTEXT_CORE_PROFILE_BIT_ABR;

	if (m_renderingSystem != nullptr)
		m_renderingSystem.reset();

	m_renderingSystem = std::make_unique<kengine::rendering_system>(window);

	if (!m_renderingSystem->init(kengine::RENDERING_TYPE::OPENGL, profile)) {
		assert(false && "splash::onWindowReady can't init rendering system!");
	}

	//glEnable(GL_DEBUG_OUTPUT);
	//glDebugMessageCallback(kengine::debugMessageCallback, nullptr);
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

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
		projection and camera settings
	*/
	m_cam.setProjectionType(kengine::PROJECTION_TYPE::_2D);

	float w = static_cast<float>(m_window->getWidth());
	float h = static_cast<float>(m_window->getHeight());

	kengine::projection_info<float> projectionInfo;
	projectionInfo.left = -w / 2.0f;
	projectionInfo.right = w / 2.0f;
	projectionInfo.bottom = -h / 2.0f;
	projectionInfo.top = h / 2.0f;
	projectionInfo.zNear = 0.1f;
	projectionInfo.zFar = 100.0f;
	projectionInfo.width = w;
	projectionInfo.height = h;
	projectionInfo.fovy = 60.0f;
	m_cam.setProjectionInfo(projectionInfo);

	kengine::vec3<float> eye{ 0.0f, 0.0f, 1.0f };
	kengine::vec3<float> center{ 0.0f, 0.0f, 0.0f };
	kengine::vec3<float> up{ 0.0f, 1.0f, 0.0f };
	m_cam.setCamera(eye, center, up);

	m_shader.setUniform("projectionTransform", false, m_cam.getProjection(false).data());
	m_shader.setUniform("viewTransform", false, m_cam.lookAt().data());

	glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	kengine::setVSync(1);

	float red = ((kengine::BACKGROUND_COLOR_YELLOW_LOGO & 0xFF000000) >> 24) / 256.0f;
	float green = ((kengine::BACKGROUND_COLOR_YELLOW_LOGO & 0x00FF0000) >> 16) / 256.0f;
	float blue = ((kengine::BACKGROUND_COLOR_YELLOW_LOGO & 0x0000FF00) >> 8) / 256.0f;
	float alpha = (kengine::BACKGROUND_COLOR_YELLOW_LOGO & 0x000000FF) / 256.0f;
	glClearColor(red, green, blue, alpha);
}

void demo::splash::beforeMainLoopEvent()
{
	std::unordered_map<std::string, GLuint> shaderLocations = {
		{"POSITION", 0 },
		{"TEXTURE", 1 },
	};

	m_primitive = kengine::quad(324.0f, 324.0f, shaderLocations);

	int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	//std::string filename = std::string(K_PROJECT_PATH) + "/assets/shisabranco1080_fullhd.png";
	std::string filename = std::string(K_PROJECT_PATH) + "/assets/shisacolorido1080_fullhd.png";
	//std::string filename = std::string(K_PROJECT_PATH) + "/assets/shisapreto1080_fullhd.png";
	unsigned char* imageData = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	if (imageData) {
		GLint internalFormat = 0;
		GLenum format = 0;

		switch (channels) {
		case 1:
			internalFormat = GL_R8;
			format = GL_RED;
			break;

		case 3:
			internalFormat = GL_RGB8;
			format = GL_RGB;
			break;

		case 4:
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
			break;
		}

		m_texture.create(kengine::TEXTURE_TYPE::_2D, 0, internalFormat, width, height, format, GL_UNSIGNED_BYTE, imageData);
		m_texture.bindTextureUnit(0);
	} else {
		std::cout << stbi_failure_reason() << std::endl;
	}

	stbi_image_free(imageData);

	m_sceneTimer.setTimerInMs(5000); // 5 seconds
	m_sceneTimer.start();
}

void demo::splash::update(int64_t frameTime)
{
	if (m_sceneTimer.done()) {
		m_renderingSystem->finish();
		m_window->destroy();
		return;
	}

	static float currentTime = 0.0f;
	currentTime += static_cast<float>(frameTime) / static_cast<float>(kengine::getHighResolutionTimerFrequency());

	float circlePoints = static_cast<float>(M_PI) * 2.0f / 16.0f;
	circlePoints = 1.0f;
	
	float speed = 10.0f;
	float x = std::cos(circlePoints * currentTime * speed) * 1.5f;
	float y = std::sin(circlePoints * currentTime * speed) * 1.5f;
	kengine::matrix<float> t = kengine::translate<float>(x, y, 0.0f);
	m_shader.setUniform("modelTransform", false, t.data());

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_primitive->draw();
	m_renderingSystem->swapBuffers();
}

void demo::splash::onFinishEvent()
{
	m_engine->stopMainLoop();
}

void demo::splash::onWindowDestroy()
{
	kengine::quitApplication(0);
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
	m_cam.getProjectionInfo()->width = static_cast<float>(width);
	m_cam.getProjectionInfo()->height = static_cast<float>(height);

	// na primeira vez que a janela criada, rendering system ainda não foi inicializada
	if (!m_renderingSystem)
		return;

	m_shader.useProgram();
	m_shader.setUniform("projectionTransform", false, m_cam.getProjection(false).data());
	glViewport(0, 0, width, height);
}

void demo::splash::onKeyboardEvent(const unsigned long long key, const int state)
{
	if (key == kengine::KEY::ENTER && state)
	{
		if(m_renderingSystem)
			m_renderingSystem->finish();

		m_window->destroy();
	}
}

void demo::splash::debugMessage(const std::string& msg)
{
	std::string message = "> debug message: " + msg;
	K_LOG_OUTPUT_RAW(message);
}
