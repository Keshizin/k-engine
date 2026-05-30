/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#include <game.hpp>
#include <media.hpp>
#include <gltf_wrapper.hpp>
#include <logger.hpp>

#include <cassert>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#define GAME_WINDOW_WIDTH 1280
#define GAME_WINDOW_HEIGHT 720

demo::game::game(kengine::core* engine)
	: m_engine{ engine }, m_window{ kengine::windowInstance() }
{
	assert(!(m_engine == nullptr) && "game constructor failed - m_engine is nullptr");
	m_engine->setEventsCallback(this);
	m_modelTransforms = std::make_unique<float[]>(16 * TOTAL_ENTITY);
}

demo::game::~game() {
}

void demo::game::start()
{
	m_engine->startMainLoop();
}

void demo::game::createWindow() {
	// se a janela foi criada anteriormente, deve ser destruida antes de chamar ela novamente
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
	profile.profileMask = kengine::CONTEXT_FLAG::CONTEXT_CORE_PROFILE_BIT_ARB;

	if (m_renderingSystem != nullptr)
		m_renderingSystem.reset();

	m_renderingSystem = std::make_unique<kengine::rendering_system>(window);

	if (!m_renderingSystem->init(kengine::RENDERING_TYPE::OPENGL, profile)) {
		assert(false && "game::onWindowReady failed to initialize rendering system!");
	}

	K_LOG_OUTPUT_RAW(kengine::getGLInfo(false));
	K_LOG_OUTPUT_RAW("GLM version: " << GLM_VERSION_MAJOR << "." << GLM_VERSION_MINOR << "." << GLM_VERSION_PATCH);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(kengine::debugMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	/*
		Creating shader programs
	*/

	kengine::ShaderInfo particleShaders[] = {
		{GL_VERTEX_SHADER, std::string(K_PROJECT_PATH) + "/shaders/particle.vert"},
		{GL_FRAGMENT_SHADER, std::string(K_PROJECT_PATH) + "/shaders/minimal.frag"},
		{GL_NONE, ""}
	};

	if (!m_particleShader.loadShaders(particleShaders)) {
		assert(false && "Failed to load particle shaders!");
	}

	kengine::ShaderInfo lightingShaders[] = {
		{GL_VERTEX_SHADER, std::string(K_PROJECT_PATH) + "/shaders/phong_instanced.vert"},
		{GL_FRAGMENT_SHADER, std::string(K_PROJECT_PATH) + "/shaders/phong.frag"},
		{GL_NONE, ""}
	};

	if(!m_lightingShader.loadShaders(lightingShaders)) {
		assert(false && "Failed to load phong shaders");
	}

	kengine::ShaderInfo simpleShaderInfo[] = {
		{GL_VERTEX_SHADER, std::string(K_PROJECT_PATH) + "/shaders/minimal.vert"},
		{GL_FRAGMENT_SHADER, std::string(K_PROJECT_PATH) + "/shaders/minimal.frag"},
		{GL_NONE, ""}
	};

	if (!m_simpleShader.loadShaders(simpleShaderInfo)) {
		assert(false && "Failed to load trigonometry shaders");
	}

	kengine::ShaderInfo shaderInfo[] = {
		{GL_VERTEX_SHADER, std::string(K_PROJECT_PATH) + "/shaders/phong.vert"},
		{GL_FRAGMENT_SHADER, std::string(K_PROJECT_PATH) + "/shaders/phong.frag"},
		{GL_NONE, ""}
	};

	if (!m_lightingWithTexture.loadShaders(shaderInfo)) {
		assert(false && "Failed to load simple shaders");
	}

	kengine::ShaderInfo skeletalShaderInfo[] = {
		{GL_VERTEX_SHADER, std::string(K_PROJECT_PATH) + "/shaders/skeletal.vert"},
		{GL_FRAGMENT_SHADER, std::string(K_PROJECT_PATH) + "/shaders/skeletal.frag"},
		{GL_NONE, ""}
	};

	if (!m_skeletalShader.loadShaders(skeletalShaderInfo)) {
		assert(false && "Failed to load trigonometry shaders");
	}

	/*
		Camera settings
	*/
	if (!kengine::loadCameraConfig(&m_cam)) {

		m_cam.setProjectionType(kengine::PROJECTION_TYPE::_3D_PERSPECTIVE);

		kengine::projection_info<float> projectionInfo;
		projectionInfo.left = -2.0f;
		projectionInfo.right = 2.0f;
		projectionInfo.bottom = -2.0f;
		projectionInfo.top = 2.0f;
		projectionInfo.zNear = 0.1f;
		projectionInfo.zFar = 1000.0f;
		projectionInfo.width = static_cast<float>(window->getWidth());
		projectionInfo.height = static_cast<float>(window->getHeight());
		projectionInfo.fovy = 60.0f;
		m_cam.setProjectionInfo(projectionInfo);

		kengine::vec3<float> eye{ 0.0f, 5.0f, 50.0f };
		kengine::vec3<float> center{ 0.0f, 0.0f, 0.0f };
		kengine::vec3<float> up{ 0.0f, 1.0f, 0.0f };
		m_cam.setCamera(eye, center, up);

		kengine::saveCameraConfig(&m_cam);
	}

	setCameraShaders();

	/*
		GUI initializing
	*/
	m_gui.init(m_window->getHandle());
	m_gui.setWindowSize(m_window->getWidth(), m_window->getHeight());
	m_gui.setProfile(m_engine->getProfile());
	m_gui.setCameraWidget(&m_cam, &m_cameraControl);
	m_gui.setPhongLighting(&m_light, &m_material);

	glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
	glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	kengine::setVSync(0);

	float red = ((kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0xFF000000) >> 24) / 255.0f;
	float green = ((kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0x00FF0000) >> 16) / 255.0f;
	float blue = ((kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0x0000FF00) >> 8) / 255.0f;
	float alpha = (kengine::BACKGROUND_COLOR_DARK_AUBERGINE_UBUNTU & 0x000000FF) / 255.0f;
	glClearColor(red, green, blue, alpha);
}

void demo::game::beforeMainLoopEvent()
{
	/*
		mesh initialization
	*/

	std::unordered_map<std::string, GLuint> shaderLocations = {
		{"POSITION", 0 },
		{"TEXCOORD_0", 1 },
		{"NORMAL", 2 },
		{"COLOR", 3 },
		{"MODEL_TRANSFORM", 12 }
	};

	m_primitive = kengine::quad(1.0f, 1.0f, shaderLocations);

	std::unordered_map<std::string, GLuint> gltfShaderLocations = {
		{"POSITION", 0 },
		{"TEXCOORD_0", 1 },
		{"TEXCOORD_1", 6 },
		{"TEXCOORD_2", 7 },
		{"NORMAL", 2 },
		{"COLOR_0", 3 },
		{"JOINTS_0", 4 },
		{"WEIGHTS_0", 5 },
		{"MODEL_TRANSFORM", 12 }
	};

	m_grid = kengine::grid3D(shaderLocations);
	m_cube = kengine::cube(8.0f, shaderLocations);
	m_lamp = kengine::sphere(1.0f, 16, 16, shaderLocations);
	m_sphere = kengine::sphere(10.0f, 32, 32, shaderLocations);
	m_torus = kengine::torus(48, 20.0f, 5.0f, shaderLocations);

	/*
		Phong lighting
	*/

	m_light.m_position = kengine::vec3(15.0f, 5.0f, 10.0f);
	m_light.La = kengine::vec3(0.1f, 0.1f, 0.1f);
	m_light.Ld = kengine::vec3(1.0f, 1.0f, 1.0f);
	m_light.Ls = kengine::vec3(0.8f, 0.8f, 0.8f);

	m_material.Ka = kengine::vec3(0.1f, 0.1f, 0.1f);
	m_material.Kd = kengine::vec3(0.5f, 0.0f, 0.0f);
	m_material.Ks = kengine::vec3(0.8f, 0.8f, 0.8f);

	setLightingShaders(&m_lightingShader);
	setLightingShaders(&m_lightingWithTexture);

	/*
		GLTF
	*/
	//auto startTime = kengine::getHighResolutionTimerCounter();
	//m_gltfModel = kengine::loadGLTF(std::string(K_PROJECT_PATH) + "/assets/" , "scene.gltf", gltfShaderLocations, &m_skeletalShader);
	//auto endTime = kengine::getHighResolutionTimerCounter();
	//K_LOG_OUTPUT_RAW("> GLTF file loading time: " << static_cast<double>(endTime - startTime) / static_cast<double>(kengine::getHighResolutionTimerFrequency()));
	//m_gltfModel->setShader(&m_skeletalShader);

	/*
		setting vbo for instancing model transform
	*/
	m_gpuBuffer.addBuffers(1);
	m_gpuBuffer.load(0, GL_ARRAY_BUFFER, 16 * TOTAL_ENTITY * sizeof(float), nullptr);

	m_cube->loadAttributeInfo(shaderLocations["MODEL_TRANSFORM"], m_gpuBuffer[0], GL_ARRAY_BUFFER, 4, GL_FLOAT, false, 16 * sizeof(float), 0);
	glVertexAttribDivisor(shaderLocations["MODEL_TRANSFORM"], 1);
	m_cube->loadAttributeInfo(shaderLocations["MODEL_TRANSFORM"] + 1, m_gpuBuffer[0], GL_ARRAY_BUFFER, 4, GL_FLOAT, false, 16 * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribDivisor(shaderLocations["MODEL_TRANSFORM"] + 1, 1);
	m_cube->loadAttributeInfo(shaderLocations["MODEL_TRANSFORM"] + 2, m_gpuBuffer[0], GL_ARRAY_BUFFER, 4, GL_FLOAT, false, 16 * sizeof(float), (void*)(8 * sizeof(float)));
	glVertexAttribDivisor(shaderLocations["MODEL_TRANSFORM"] + 2, 1);
	m_cube->loadAttributeInfo(shaderLocations["MODEL_TRANSFORM"] + 3, m_gpuBuffer[0], GL_ARRAY_BUFFER, 4, GL_FLOAT, false, 16 * sizeof(float), (void*)(12 * sizeof(float)));
	glVertexAttribDivisor(shaderLocations["MODEL_TRANSFORM"] + 3, 1);
}

void demo::game::update(int64_t frameTime)
{
	float slowTime = 0.5;
	static float currentTime = 0.0f;
	currentTime += m_engine->getFrameTime() * slowTime;

	m_particleShader.useProgram();
	m_particleShader.setUniform("currentTime", currentTime);

	kengine::matrix<float> sunTransform = kengine::translate<float>(0.0f, 0.0f, 0.0f);
	kengine::matrix local = kengine::rotate<float>(currentTime * 35.0f, currentTime * 75.0f, 0.0f) * sunTransform;
	local.copy(m_modelTransforms.get());

	kengine::matrix<float> earthTransform = sunTransform * kengine::rotate<float>(0.0f, currentTime * 50.0f, 0.0f) * kengine::translate<float>(15.0f, 0.0f, 0.0f);
	local = earthTransform * kengine::rotate<float>(currentTime * 15.0f, currentTime * 25.0f, 0.0f) * kengine::scale(0.5f, 0.5f, 0.5f);
	local.copy((m_modelTransforms.get() + 16));

	kengine::matrix<float> moonTransform = earthTransform * kengine::rotate<float>(currentTime * 50.0f, 0.0f, 0.0f) * kengine::translate<float>(0.0f, 5.0f, 0.0f);
	local = moonTransform * kengine::rotate<float>(currentTime * 5.0f, currentTime * 5.0f, 0.0f) * kengine::scale(0.15f, 0.15f, 0.15f);
	local.copy((m_modelTransforms.get() + 32));
	
	m_gpuBuffer.update(0, GL_ARRAY_BUFFER, 16 * TOTAL_ENTITY * sizeof(float), m_modelTransforms.get());

	//m_gltfModel->updateAnimation(m_engine->getFrameTime());

	// ***************************************************************************
	//	rendering the scene
	// ***************************************************************************

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_simpleShader.useProgram();
	m_simpleShader.setUniform("modelTransform", false, kengine::matrix<float>(1.0f).data());
	m_grid->draw();

	m_particleShader.useProgram();
	m_primitive->drawInstanced(TOTAL_TRIANGLES);

	m_simpleShader.useProgram();
	m_simpleShader.setUniform("modelTransform", false, kengine::translate<float>(m_light.m_position.x, m_light.m_position.y, m_light.m_position.z).data());
	m_lamp->draw();

	m_lightingShader.useProgram();
	m_cube->drawInstanced(TOTAL_ENTITY);

	m_lightingWithTexture.useProgram();
	m_lightingWithTexture.setUniform("modelTransform", false, kengine::translate<float>(std::sin(currentTime * 0.75f) * 30.0f, 0.0f, 0.0f).data());
	m_sphere->draw();

	m_lightingWithTexture.useProgram();
	m_lightingWithTexture.setUniform("modelTransform", false, kengine::rotate<float>(currentTime * 15.0f, 0.0f, 0.0f).data());
	m_torus->draw();
	
	//auto rootNode = m_gltfModel->getRootNode(-1, -1);

	//if (rootNode != -1)
	//	m_gltfModel->traverseScene(rootNode, kengine::matrix<GLfloat>(1.0f));

	/*
		drawing GUI 
	*/
	m_gui.draw();

	if (m_gui.getCameraWidget()->mustBeUpdate()) {
		setCameraShaders();
		m_gui.getCameraWidget()->setUpdate(false);
	}

	if (m_gui.getPhongLightingWidget()->mustBeUpdate()) {
		setLightingShaders(&m_lightingShader);
		setLightingShaders(&m_lightingWithTexture);
		m_gui.getPhongLightingWidget()->setUpdate(false);
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
	m_gui.destroy();
	// Todo: destruir os objetos OpenGL antes de destruir o contexto
	m_renderingSystem->finish();
	m_window->destroy();
}

void demo::game::onResizeWindowEvent(const int width, const int height)
{
	m_window->setWindowSize(width, height);
	m_cam.getProjectionInfo()->width = static_cast<float>(width);
	m_cam.getProjectionInfo()->height = static_cast<float>(height);

	setCameraShaders();

	glViewport(0, 0, width, height);
}

void demo::game::onMoveWindowEvent(const int x, const int y)
{
}

void demo::game::onMouseMotionEvent(const int x, const int y)
{
	if (m_isMouseButtonPressed) {

		m_cameraControl.m_xoffset = x - m_cameraControl.m_lastMouseX;
		m_cameraControl.m_yoffset = m_cameraControl.m_lastMouseY - y;
		m_cameraControl.m_yaw += (m_cameraControl.m_xoffset * m_cameraControl.m_sensitivity);

		m_cameraControl.m_yaw = std::fmod(m_cameraControl.m_yaw, 360.0f);
		m_cameraControl.m_pitch += (m_cameraControl.m_yoffset * m_cameraControl.m_sensitivity * 0.1f);

		if (m_cameraControl.m_pitch > 89.0f)
			m_cameraControl.m_pitch = 89.0f;

		if (m_cameraControl.m_pitch < -89.0f)
			m_cameraControl.m_pitch = -89.0f;

		m_cameraControl.m_forward.x = std::cos(m_cameraControl.m_yaw * K_ANGLE_TO_RADIAN) * std::cos(m_cameraControl.m_pitch);
		m_cameraControl.m_forward.y = std::sin(m_cameraControl.m_pitch);
		m_cameraControl.m_forward.z = std::sin(m_cameraControl.m_yaw * K_ANGLE_TO_RADIAN) * std::cos(m_cameraControl.m_pitch);
		m_cameraControl.m_forward.normalize();

		m_cam.setCenter(m_cam.getEye() + m_cameraControl.m_forward);

		m_particleShader.useProgram();
		m_particleShader.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_lightingShader.useProgram();
		m_lightingShader.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_simpleShader.useProgram();
		m_simpleShader.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_lightingWithTexture.useProgram();
		m_lightingWithTexture.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_skeletalShader.useProgram();
		m_skeletalShader.setUniform("viewTransform", false, m_cam.lookAt().data());
	}

	m_cameraControl.m_lastMouseX = x;
	m_cameraControl.m_lastMouseY = y;
}

void demo::game::onMouseButtonEvent(const int button, const kengine::MOUSE_ACTION& state, const int x, const int y)
{
	if (button == kengine::MOUSE_BUTTON::RIGHT) {

		if (state == kengine::MOUSE_ACTION::DOWN)
			m_isMouseButtonPressed = true;
		else
			m_isMouseButtonPressed = false;
	}
}

void demo::game::onKeyboardEvent(const unsigned long long key, const int state)
{
	if (key == kengine::KEY::TAB && state) {
		m_gui.setEnable(!m_gui.getEnable());
	}

	if (key == kengine::KEY::OEM_3 && state) {
		m_gui.setDemoExample(!m_gui.getDemoExample());
	}

	bool isPressed = false;
	float speed = 10.0f;
	kengine::vec3<float> forward = kengine::normalize(m_cam.getCenter() - m_cam.getEye());

	if (key == 'W' && state) {
		kengine::vec3<float> pos = m_cam.getEye() + (forward * speed);
		m_cam.setEye(pos);
		m_cam.setCenter(pos + forward);
		isPressed = true;
	}
	
	if (key == 'A' && state) {
		kengine::vec3<float> right = kengine::normalize(kengine::crossProduct(forward, m_cam.getUp()));

		kengine::vec3<float> pos = m_cam.getEye() - (right * speed);
		m_cam.setEye(pos);
		m_cam.setCenter(pos + forward);
		isPressed = true;
	}

	if (key == 'S' && state) {
		kengine::vec3<float> pos = m_cam.getEye() - (forward * speed);
		m_cam.setEye(pos);
		m_cam.setCenter(pos + forward);
		isPressed = true;
	}

	if (key == 'D' && state) {
		kengine::vec3<float> right = kengine::normalize(kengine::crossProduct(forward, m_cam.getUp()));

		kengine::vec3<float> pos = m_cam.getEye() + (right * speed);
		m_cam.setEye(pos);
		m_cam.setCenter(pos + forward);
		isPressed = true;
	}

	if (key == 'P' && state) {
		kengine::saveCameraConfig(&m_cam);
	}

	if (key == 'Z' && state) {
		static bool b = false;

		b ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL) : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		b = !b;
	}

	if (isPressed) {
		m_particleShader.useProgram();
		m_particleShader.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_lightingShader.useProgram();
		m_lightingShader.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_simpleShader.useProgram();
		m_simpleShader.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_lightingWithTexture.useProgram();
		m_lightingWithTexture.setUniform("viewTransform", false, m_cam.lookAt().data());
		m_skeletalShader.useProgram();
		m_skeletalShader.setUniform("viewTransform", false, m_cam.lookAt().data());
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

int demo::game::customWindowProcedure(void* param1, void* param2, void* param3, void* param4)
{
	return kengine::customWndProcHandler(param1, param2, param3, param4);
}

void demo::game::setCameraShaders()
{
	m_particleShader.useProgram();
	m_particleShader.setUniform("projectionTransform", false, m_cam.getProjection(true).data());
	m_particleShader.setUniform("viewTransform", false, m_cam.lookAt().data());

	m_lightingShader.useProgram();
	m_lightingShader.setUniform("projectionTransform", false, m_cam.getProjection(true).data());
	m_lightingShader.setUniform("viewTransform", false, m_cam.lookAt().data());

	m_simpleShader.useProgram();
	m_simpleShader.setUniform("projectionTransform", false, m_cam.getProjection(true).data());
	m_simpleShader.setUniform("viewTransform", false, m_cam.lookAt().data());

	m_lightingWithTexture.useProgram();
	m_lightingWithTexture.setUniform("projectionTransform", false, m_cam.getProjection(true).data());
	m_lightingWithTexture.setUniform("viewTransform", false, m_cam.lookAt().data());

	m_skeletalShader.useProgram();
	m_skeletalShader.setUniform("projectionTransform", false, m_cam.getProjection(true).data());
	m_skeletalShader.setUniform("viewTransform", false, m_cam.lookAt().data());


}

void demo::game::setLightingShaders(kengine::GLSLprogram* shaderProgram)
{
	shaderProgram->useProgram();
	shaderProgram->setUniform("Light.position", m_light.m_position.x, m_light.m_position.y, m_light.m_position.z);
	shaderProgram->setUniform("Light.La", m_light.La.x, m_light.La.y, m_light.La.x);
	shaderProgram->setUniform("Light.Ld", m_light.Ld.x, m_light.Ld.y, m_light.Ld.x);
	shaderProgram->setUniform("Light.Ls", m_light.Ls.x, m_light.Ls.y, m_light.Ls.x);
	shaderProgram->setUniform("Material.shininess", m_material.shininess);
	shaderProgram->setUniform("Material.Ka", m_material.Ka.x, m_material.Ka.y, m_material.Ka.z);
	shaderProgram->setUniform("Material.Kd", m_material.Kd.x, m_material.Kd.y, m_material.Kd.z);
	shaderProgram->setUniform("Material.Ks", m_material.Ks.x, m_material.Ks.y, m_material.Ks.z);
}
