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

#include <gui.hpp>
#include <os_api_wrapper.hpp>

#include <imgui-1.91.8/imgui.h>
#include <imgui-1.91.8/backends/imgui_impl_win32.h>
#include <imgui-1.91.8/backends/imgui_impl_opengl3.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(void* hWnd, void* msg, void* wParam, void* lParam);

void kgui::customWndProcHandler(void* param1, void* param2, void* param3, void* param4)
{
	ImGui_ImplWin32_WndProcHandler(param1, param2, param3, param4);
}

/*
	kgui::profile_widget member class definition
*/

void kgui::profile_widget::draw()
{
	if (!p_profile)
		return;

	auto fps = p_profile->getFramesPerSecond();
	ImGui::Text("FPS: %d", fps);
}


/*
	kgui::GraphicalUserInterface member class definition
*/

void kgui::GraphicalUserInterface::init(void* windowHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui_ImplWin32_InitForOpenGL(windowHandle);
	ImGui_ImplOpenGL3_Init();
}

void kgui::GraphicalUserInterface::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void kgui::GraphicalUserInterface::draw()
{
	if (!m_enable)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ImGui::ShowDemoWindow();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(1280, 720));
	ImGui::SetNextWindowBgAlpha(0.35f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoScrollbar;
	window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_MenuBar;

	ImGui::Begin("K-Engine!", nullptr, window_flags);

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Info")) {
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View")) {
			drawView();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Lighting")) {
			//m_lightingWidget.draw();
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools")) {
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	m_profileWidget.draw();

	ImGui::End();

	// ========================================================================

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void kgui::GraphicalUserInterface::drawView()
{
	m_projectionWidget.m_update = false;

	ImGui::SeparatorText("Projection Type:");
	static int e = 0;
	ImGui::RadioButton("Frustum", &e, 0); ImGui::SameLine();
	ImGui::RadioButton("Perspective", &e, 1); ImGui::SameLine();
	ImGui::RadioButton("Ortho", &e, 2);

	switch (e)
	{
	case 0: // frustum
		if (m_projectionWidget.m_type != kengine::PROJECTION_TYPE::_3D_FRUSTUM) {
			m_projectionWidget.m_type = kengine::PROJECTION_TYPE::_3D_FRUSTUM;
			m_projectionWidget.m_update = true;
		}

		break;

	case 1: // perspective
		if (m_projectionWidget.m_type != kengine::PROJECTION_TYPE::_3D_PERSPECTIVE) {
			m_projectionWidget.m_type = kengine::PROJECTION_TYPE::_3D_PERSPECTIVE;
			m_projectionWidget.m_update = true;
		}

		break;

	case 2: // ortho
		if (m_projectionWidget.m_type != kengine::PROJECTION_TYPE::_3D_ORTHO) {
			m_projectionWidget.m_type = kengine::PROJECTION_TYPE::_3D_ORTHO;
			m_projectionWidget.m_update = true;
		}

		break;
	}

	ImGui::SeparatorText("Projection Parameters:");

	static float left = 0.0f;
	ImGui::InputFloat("Left", &m_projectionWidget.m_cam->m_projectionInfo.left, 0.0f, 1.0f, "%.6f");

	if (left != m_projectionWidget.m_cam->m_projectionInfo.left) {
		left = m_projectionWidget.m_cam->m_projectionInfo.left;
		m_projectionWidget.m_update = true;
	}

	static float right = 0.0f;
	ImGui::InputFloat("Right", &m_projectionWidget.m_cam->m_projectionInfo.right, 0.0f, 1.0f, "%.6f");

	if (right != m_projectionWidget.m_cam->m_projectionInfo.right) {
		right = m_projectionWidget.m_cam->m_projectionInfo.right;
		m_projectionWidget.m_update = true;
	}

	static float bottom = 0.0f;
	ImGui::InputFloat("Bottom", &m_projectionWidget.m_cam->m_projectionInfo.bottom, 0.0f, 1.0f, "%.6f");

	if (bottom != m_projectionWidget.m_cam->m_projectionInfo.bottom) {
		bottom = m_projectionWidget.m_cam->m_projectionInfo.bottom;
		m_projectionWidget.m_update = true;
	}

	static float top = 0.0f;
	ImGui::InputFloat("Top", &m_projectionWidget.m_cam->m_projectionInfo.top, 0.0f, 1.0f, "%.6f");

	if (top != m_projectionWidget.m_cam->m_projectionInfo.top) {
		top = m_projectionWidget.m_cam->m_projectionInfo.top;
		m_projectionWidget.m_update = true;
	}

	static float zNear = 0.0f;
	ImGui::InputFloat("Near", &m_projectionWidget.m_cam->m_projectionInfo.zNear, 0.0f, 1.0f, "%.6f");

	if (zNear != m_projectionWidget.m_cam->m_projectionInfo.zNear) {
		zNear = m_projectionWidget.m_cam->m_projectionInfo.zNear;
		m_projectionWidget.m_update = true;
	}

	static float zFar = 0.0f;
	ImGui::InputFloat("Far", &m_projectionWidget.m_cam->m_projectionInfo.zFar, 0.0f, 1.0f, "%.6f");

	if (zFar != m_projectionWidget.m_cam->m_projectionInfo.zFar) {
		zFar = m_projectionWidget.m_cam->m_projectionInfo.zFar;
		m_projectionWidget.m_update = true;
	}

	static float fovy = 0.0f;
	ImGui::InputFloat("Field of view", &m_projectionWidget.m_cam->m_projectionInfo.fovy, 0.0f, 1.0f, "%.6f");

	if (fovy != m_projectionWidget.m_cam->m_projectionInfo.fovy) {
		fovy = m_projectionWidget.m_cam->m_projectionInfo.fovy;
		m_projectionWidget.m_update = true;
	}

	ImGui::SeparatorText("Camera Parameters:");

	static float eyeX = 0.0f;
	ImGui::InputFloat("Eye X", &m_projectionWidget.m_cam->eye.x, 0.0f, 1.0f, "%.6f");

	if (eyeX != m_projectionWidget.m_cam->eye.x) {
		eyeX = m_projectionWidget.m_cam->eye.x;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float eyeY = 0.0f;

	ImGui::InputFloat("Eye Y", &m_projectionWidget.m_cam->eye.y, 0.0f, 1.0f, "%.6f");

	if (eyeY != m_projectionWidget.m_cam->eye.y) {
		eyeY = m_projectionWidget.m_cam->eye.y;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float eyeZ = 0.0f;

	ImGui::InputFloat("Eye Z", &m_projectionWidget.m_cam->eye.z, 0.0f, 1.0f, "%.6f");

	if (eyeZ != m_projectionWidget.m_cam->eye.z) {
		eyeZ = m_projectionWidget.m_cam->eye.z;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float centerX = 0.0f;

	ImGui::InputFloat("center X", &m_projectionWidget.m_cam->center.x, 0.0f, 1.0f, "%.6f");

	if (centerX != m_projectionWidget.m_cam->center.x) {
		centerX = m_projectionWidget.m_cam->center.x;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float centerY = 0.0f;

	ImGui::InputFloat("center Y", &m_projectionWidget.m_cam->center.y, 0.0f, 1.0f, "%.6f");

	if (centerY != m_projectionWidget.m_cam->center.y) {
		centerY = m_projectionWidget.m_cam->center.y;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float centerZ = 0.0f;

	ImGui::InputFloat("center Z", &m_projectionWidget.m_cam->center.z, 0.0f, 1.0f, "%.6f");

	if (centerZ != m_projectionWidget.m_cam->center.z) {
		centerZ = m_projectionWidget.m_cam->center.z;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float upX = 0.0f;

	ImGui::InputFloat("up X", &m_projectionWidget.m_cam->up.x, 0.0f, 1.0f, "%.6f");

	if (upX != m_projectionWidget.m_cam->up.x) {
		upX = m_projectionWidget.m_cam->up.x;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float upY = 0.0f;

	ImGui::InputFloat("up Y", &m_projectionWidget.m_cam->up.y, 0.0f, 1.0f, "%.6f");

	if (upY != m_projectionWidget.m_cam->up.y) {
		upY = m_projectionWidget.m_cam->up.y;
		m_projectionWidget.m_cameraUpdate = true;
	}

	static float upZ = 0.0f;

	ImGui::InputFloat("up Z", &m_projectionWidget.m_cam->up.z, 0.0f, 1.0f, "%.6f");

	if (upZ != m_projectionWidget.m_cam->up.z) {
		upZ = m_projectionWidget.m_cam->up.z;
		m_projectionWidget.m_cameraUpdate = true;
	}
}