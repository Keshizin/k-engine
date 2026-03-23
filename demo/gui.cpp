/*
	K-Engine Graphical User Interface (experimental)
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

#include <gui.hpp>
#include <os_api_wrapper.hpp>
#include <k_version.hpp>

#include <imgui-1.92.5/imgui.h>
#include <imgui-1.92.5/backends/imgui_impl_win32.h>
#include <imgui-1.92.5/backends/imgui_impl_opengl3.h>

#if defined(_WIN32)
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(void* hWnd, void* msg, void* wParam, void* lParam);
#endif

int kengine::customWndProcHandler(void* param1, void* param2, void* param3, void* param4)
{
#if defined(_WIN32)
	ImGui_ImplWin32_WndProcHandler(param1, param2, param3, param4);
#endif

	return 1;
}

/*
	kengine::profile_widget member class definition
*/

void kengine::profile_widget::draw()
{
	if (!m_profile)
		return;

	auto fps = m_profile->getFramesPerSecond();
	ImGui::Text("FPS: %d", fps);
}

void kengine::profile_widget::drawMenu()
{
	ImGui::CheckboxFlags("Profile", &m_comboFlags, ImGuiComboFlags_None);
}

/*
	kengine::camera_widget member class definition
*/

void kengine::camera_widget::draw()
{
	if (!m_camera)
		return;

	ImGui::SeparatorText("Projection Type:");

	static int type = static_cast<int>(m_camera->getProjectionType());
	
	if (ImGui::RadioButton("Frustum", &type, static_cast<int>(kengine::PROJECTION_TYPE::_3D_FRUSTUM))) {
		m_update = true;
		m_camera->setProjectionType(static_cast<kengine::PROJECTION_TYPE>(type));
	}

	ImGui::SameLine();
		
	if (ImGui::RadioButton("Perspective", &type, static_cast<int>(kengine::PROJECTION_TYPE::_3D_PERSPECTIVE))) {
		m_update = true;
		m_camera->setProjectionType(static_cast<kengine::PROJECTION_TYPE>(type));
	}
		
	ImGui::SameLine();
	
	if (ImGui::RadioButton("Ortho", &type, static_cast<int>(kengine::PROJECTION_TYPE::_3D_ORTHO))) {
		m_update = true;
		m_camera->setProjectionType(static_cast<kengine::PROJECTION_TYPE>(type));
	}

	ImGui::SeparatorText("Projection Parameters:");

	if (ImGui::InputFloat("left", &m_camera->getProjectionInfo()->left, 0.001f, 0.1f, "%.6f", ImGuiInputTextFlags_None)) {
		m_update = true;
	}

	if (ImGui::InputFloat("right", &m_camera->getProjectionInfo()->right, 0.001f, 0.1f, "%.6f", ImGuiInputTextFlags_None)) {
		m_update = true;
	}

	if (ImGui::InputFloat("bottom", &m_camera->getProjectionInfo()->bottom, 0.001f, 0.1f, "%.6f", ImGuiInputTextFlags_None)) {
		m_update = true;
	}

	if (ImGui::InputFloat("top", &m_camera->getProjectionInfo()->top, 0.001f, 0.1f, "%.6f", ImGuiInputTextFlags_None)) {
		m_update = true;
	}

	if (ImGui::InputFloat("near", &m_camera->getProjectionInfo()->zNear, 0.001f, 0.1f, "%.6f", ImGuiInputTextFlags_None)) {
		m_update = true;
	}

	if (ImGui::InputFloat("far", &m_camera->getProjectionInfo()->zFar, 0.001f, 0.1f, "%.6f", ImGuiInputTextFlags_None)) {
		m_update = true;
	}

	if (m_camera->getProjectionType() == kengine::PROJECTION_TYPE::_3D_PERSPECTIVE) {

		if (ImGui::InputFloat("fovy", &m_camera->getProjectionInfo()->fovy, 0.001f, 0.1f, "%.6f", ImGuiInputTextFlags_None)) {
			m_update = true;
		}
	}

	ImGui::SeparatorText("Camera parameters:");

	float vEye[4] = { 0 };
	m_camera->getEye().copy(vEye);

	if (ImGui::InputFloat3("eye", vEye, "%.6f", ImGuiInputTextFlags_None) && ImGui::IsItemDeactivatedAfterEdit()) {
		m_update = true;
		m_camera->setEye(vEye);
	}

	float vCenter[4] = { 0 };
	m_camera->getCenter().copy(vCenter);

	if (ImGui::InputFloat3("center", vCenter, "%.6f", ImGuiInputTextFlags_None) && ImGui::IsItemDeactivatedAfterEdit()) {
		m_update = true;
		m_camera->setCenter(vCenter);
	}

	float vUp[4] = { 0 };
	m_camera->getUp().copy(vUp);

	if (ImGui::InputFloat3("up", vUp, "%.6f", ImGuiInputTextFlags_None) && ImGui::IsItemDeactivatedAfterEdit()) {
		m_update = true;
		m_camera->setUp(vUp);
	}
}


/*
	kengine::projection_widget member class definition
*/
void kengine::vectors_widget::drawMenu()
{
	ImGui::MenuItem("Vectors", nullptr, &m_isSelected, true);
}

void kengine::vectors_widget::draw()
{
	if (m_isSelected) {

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

		//window_flags |= ImGuiWindowFlags_NoResize;
		//window_flags |= ImGuiWindowFlags_NoMove;
		//window_flags |= ImGuiWindowFlags_NoScrollbar;
		//window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		//window_flags |= ImGuiWindowFlags_NoCollapse;
		//window_flags |= ImGuiWindowFlags_NoBackground;
		//window_flags |= ImGuiWindowFlags_MenuBar;

		if (!ImGui::Begin("Vectors Tool", &m_isSelected)) {
			ImGui::End();
			return;
		}

		ImGui::End();
	}
}


/*
	kengine::projection_widget member class definition
*/

void kengine::projection_widget::drawMenu()
{
	ImGui::MenuItem("Projection", nullptr, &m_isSelected, true);
}

void kengine::projection_widget::draw()
{
	if (m_isSelected) {

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

		//window_flags |= ImGuiWindowFlags_NoResize;
		//window_flags |= ImGuiWindowFlags_NoMove;
		//window_flags |= ImGuiWindowFlags_NoScrollbar;
		//window_flags |= ImGuiWindowFlags_NoScrollWithMouse;
		//window_flags |= ImGuiWindowFlags_NoCollapse;
		//window_flags |= ImGuiWindowFlags_NoBackground;
		//window_flags |= ImGuiWindowFlags_MenuBar;

		if (!ImGui::Begin("Projection Tool", &m_isSelected)) {
			ImGui::End();
			return;
		}

		ImGui::End();
	}
}


/*
*/
void kengine::gui::init(void* windowHandle)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // enable keyboard controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // enable gamepad controls

	ImGui_ImplWin32_InitForOpenGL(windowHandle);
	ImGui_ImplOpenGL3_Init();
}

void kengine::gui::destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void kengine::gui::draw()
{
	if (!m_enable)
		return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// ************************************************************************

	if (m_demoExample) {
		ImGui::ShowDemoWindow();
	} else {
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
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
				ImGui::Text("K-Engine version: %s", kengine::version().c_str());
				m_profileWidget.drawMenu();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Camera")) {
				m_cameraWidget.draw();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools")) {
				m_vectorsWidget.drawMenu();
				m_projectionWidget.drawMenu();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		/*
			rendering in main viewport
		*/ 
		m_profileWidget.draw();
		m_vectorsWidget.draw();
		m_projectionWidget.draw();

		ImGui::End();
	}

	// ************************************************************************

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}