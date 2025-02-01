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
#include <k_math.hpp>

#include <iostream>
#include <cassert>
#include <cstring>

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
	kengine::mesh q = kengine::quad(2.0f);
	node.load(q);
}

void demo::game::update(const int64_t frameTime)
{
	m_renderingSystem.clearBuffers();	
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
	if (!m_renderingSystem.init(window, kengine::RENDERING_TYPE::OPENGL, profile))
	{
		K_LOG_OUTPUT_RAW("(!) Failed to init the rendering system!");
		assert("(!) SHOWSTOPPER ERROR (!)");
	}

	K_LOG_OUTPUT_RAW(m_renderingSystem.info(false));

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(kengine::debugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

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

	GLuint blockIndex = glGetUniformBlockIndex(m_shader.getProgramID(), "BlobSettings");

	GLint blockSize;
	glGetActiveUniformBlockiv(m_shader.getProgramID(), blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
	
	GLubyte* blockBuffer = new GLubyte[static_cast<size_t>(blockSize)];
	
	const GLchar* names[] = { "InnerColor", "OuterColor", "RadiusInner", "RadiusOuter" };
	GLuint indices[4];

	glGetUniformIndices(m_shader.getProgramID(), 4, names, indices);
	
	GLint offset[4];
	glGetActiveUniformsiv(m_shader.getProgramID(), 4, indices, GL_UNIFORM_OFFSET, offset);

	// Store data within the buffer at the appropriate offsets
	GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };
	GLfloat innerRadius = 0.3f;
	GLfloat outerRadius = 0.45f;
	
	memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
	memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
	memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

	GLuint uboHandle;
	glGenBuffers(1, &uboHandle);
	glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
	glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboHandle);

	delete[] blockBuffer;

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
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
	m_renderingSystem.finish();
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