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

	if (m_isSelected) {
		ImGuiViewport* vp = ImGui::GetMainViewport();
		ImVec2 windowSize = vp->WorkPos;
		windowSize.x += 5.0f;
		windowSize.y += 5.0f;
		ImGui::SetNextWindowPos(windowSize);
	
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
		window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoSavedSettings;
		window_flags |= ImGuiWindowFlags_NoNavInputs;
		window_flags |= ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoScrollWithMouse;

		ImGui::SetNextWindowBgAlpha(0.5f);

		if (ImGui::Begin("Profile", &m_isSelected, window_flags)) {
			auto fps = m_profile->getFramesPerSecond();
			ImGui::Text("FPS: %d", fps);
		}

		ImGui::End();
	}
}

void kengine::profile_widget::drawMenu()
{
	ImGui::MenuItem("Profile", NULL, &m_isSelected);
}

/*
	kengine::camera_widget member class definition
*/

void kengine::camera_widget::draw()
{
	if (!m_camera)
		return;

	if (m_isCameraSelected) {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowBgAlpha(0.5f);

		if (ImGui::Begin("Camera Settings", &m_isCameraSelected, window_flags)) {
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

			float vEye[3] = { 0 };
			m_camera->getEye().copy(vEye);

			if (ImGui::InputFloat3("eye", vEye, "%.6f", ImGuiInputTextFlags_None) && ImGui::IsItemDeactivatedAfterEdit()) {
				m_update = true;
				m_camera->setEye(vEye);
			}

			float vCenter[3] = { 0 };
			m_camera->getCenter().copy(vCenter);

			if (ImGui::InputFloat3("center", vCenter, "%.6f", ImGuiInputTextFlags_None) && ImGui::IsItemDeactivatedAfterEdit()) {
				m_update = true;
				m_camera->setCenter(vCenter);
			}

			float vUp[3] = { 0 };
			m_camera->getUp().copy(vUp);

			if (ImGui::InputFloat3("up", vUp, "%.6f", ImGuiInputTextFlags_None) && ImGui::IsItemDeactivatedAfterEdit()) {
				m_update = true;
				m_camera->setUp(vUp);
			}
		}

		ImGui::End();
	}

	// ********************************************************************

	if (!m_cameraControl)
		return;

	if (m_isCameraControlSelected) {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowBgAlpha(0.5f);

		if (ImGui::Begin("Camera Control", &m_isCameraControlSelected, window_flags)) {

			ImGui::Text("mouse offset: (%.2f,%.2f)", m_cameraControl->m_xoffset, m_cameraControl->m_yoffset);

			ImGui::SeparatorText("Camera direction:");

			if (ImGui::DragFloat("yaw", &m_cameraControl->m_yaw, 0.1f, -FLT_MAX, FLT_MAX, "%.6f", ImGuiSliderFlags_None)) {
				m_update = true;
			}

			if (ImGui::DragFloat("pitch", &m_cameraControl->m_pitch, 0.01f, -FLT_MAX, FLT_MAX, "%.6f", ImGuiSliderFlags_None)) {
				m_update = true;
			}

			if (ImGui::DragFloat("roll", &m_cameraControl->m_roll, 0.1f, -FLT_MAX, FLT_MAX, "%.6f", ImGuiSliderFlags_None)) {
				m_update = true;
			}

			m_cameraControl->m_forward.x = std::cos(m_cameraControl->m_yaw * K_ANGLE_TO_RADIAN) * std::cos(m_cameraControl->m_pitch);
			m_cameraControl->m_forward.y = std::sin(m_cameraControl->m_pitch);
			m_cameraControl->m_forward.z = std::sin(m_cameraControl->m_yaw * K_ANGLE_TO_RADIAN) * std::cos(m_cameraControl->m_pitch);
			m_cameraControl->m_forward.normalize();

			ImGui::Separator();

			float v3[3];
			m_cameraControl->m_forward.copy(v3);

			if (ImGui::InputFloat3("forward", v3, "%.6f", ImGuiInputTextFlags_None)) {
				m_update = true;
			}

			m_cameraControl->m_forward.x = v3[0];
			m_cameraControl->m_forward.y = v3[1];
			m_cameraControl->m_forward.z = v3[2];

			if (m_update) {
				m_camera->setCenter(m_camera->getEye() + m_cameraControl->m_forward);
			}

			float eye3v[3] = { 0.0f };
			m_camera->getEye().copy(eye3v);
			ImGui::InputFloat3("eye", eye3v, "%.6f", ImGuiInputTextFlags_None);

			float center3v[3] = { 0.0f };
			m_camera->getCenter().copy(center3v);
			ImGui::InputFloat3("center", center3v, "%.6f", ImGuiInputTextFlags_None);

			float up3v[3] = { 0.0f };
			m_camera->getUp().copy(up3v);
			ImGui::InputFloat3("up", up3v, "%.6f", ImGuiInputTextFlags_None);
		}

		ImGui::End();
	}
}

void kengine::camera_widget::drawMenu()
{
	ImGui::MenuItem("Camera Settings", nullptr, &m_isCameraSelected, true);
	ImGui::MenuItem("Camera Control", nullptr, &m_isCameraControlSelected, true);
}


/*
	kengine::lighting_widget class - member definition
*/
void kengine::PhongLightingWidget::draw()
{
	if (m_isSelected) {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		window_flags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowBgAlpha(0.5f);

		if (ImGui::Begin("Phong Light", &m_isSelected, window_flags)) {
			ImGui::SeparatorText("Phong Light settings:");

			float pos[3] = { 0.0f };
			m_light->m_position.copy(pos);

			if (ImGui::DragFloat3("position", pos, 0.1f, -FLT_MAX, FLT_MAX, "%.6f", ImGuiSliderFlags_None)) {
				m_light->m_position.x = pos[0];
				m_light->m_position.y = pos[1];
				m_light->m_position.z = pos[2];
				m_update = true;
			}

			float La[3] = { 0.0f };
			m_light->La.copy(La);

			if (ImGui::DragFloat3("ambient light intensity (La)", La, 0.005f, 0.0f, 1.0f, "%.6f", ImGuiSliderFlags_None)) {
				m_light->La.x = La[0];
				m_light->La.y = La[1];
				m_light->La.z = La[2];
				m_update = true;
			}

			float Ld[3] = { 0.0f };
			m_light->Ld.copy(Ld);

			if (ImGui::DragFloat3("diffuse light intensity (Ld)", Ld, 0.005f, 0.0f, 1.0f, "%.6f", ImGuiSliderFlags_None)) {
				m_light->Ld.x = Ld[0];
				m_light->Ld.y = Ld[1];
				m_light->Ld.z = Ld[2];
				m_update = true;
			}

			float Ls[3] = { 0.0f };
			m_light->Ls.copy(Ls);

			if (ImGui::DragFloat3("specular light intensity (Ls)", Ls, 0.005f, 0.0f, 1.0f, "%.6f", ImGuiSliderFlags_None)) {
				m_light->Ls.x = Ls[0];
				m_light->Ls.y = Ls[1];
				m_light->Ls.z = Ls[2];
				m_update = true;
			}

			ImGui::SeparatorText("Phong Material settings:");

			if (ImGui::DragFloat("shininess", &m_material->shininess, 0.1f, 0.0f, FLT_MAX, "%.6f", ImGuiSliderFlags_None)) {
				m_update = true;
			}

			float Ka[3] = { 0.0f };
			m_material->Ka.copy(Ka);

			if (ImGui::DragFloat3("ambient reflectivity (Ka)", Ka, 0.01f, 0.0f, 1.0f, "%.6f", ImGuiSliderFlags_None)) {
				m_material->Ka.x = Ka[0];
				m_material->Ka.y = Ka[1];
				m_material->Ka.z = Ka[2];
				m_update = true;
			}

			float Kd[3] = { 0.0f };
			m_material->Kd.copy(Kd);

			if (ImGui::DragFloat3("diffuse reflectivity (Kd)", Kd, 0.01f, 0.0f, 1.0f, "%.6f", ImGuiSliderFlags_None)) {
				m_material->Kd.x = Kd[0];
				m_material->Kd.y = Kd[1];
				m_material->Kd.z = Kd[2];
				m_update = true;
			}

			float Ks[3] = { 0.0f };
			m_material->Ks.copy(Ks);

			if (ImGui::DragFloat3("specular reflectivity (Ks)", Ks, 0.01f, 0.0f, 1.0f, "%.6f", ImGuiSliderFlags_None)) {
				m_material->Ks.x = Ks[0];
				m_material->Ks.y = Ks[1];
				m_material->Ks.z = Ks[2];
				m_update = true;
			}
		}
		
		ImGui::End();
	}
}

void kengine::PhongLightingWidget::drawMenu()
{
	ImGui::MenuItem("phong light 0", nullptr, &m_isSelected, true);
}


/*
	kengine::inspector_widget class - member definition
*/
void kengine::inspector_widget::drawMenu()
{
}

void kengine::inspector_widget::draw()
{
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
		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("Info")) {
				ImGui::Text("K-Engine version: %s", kengine::version().c_str());
				ImGui::Separator();
				m_profileWidget.drawMenu();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Camera")) {
				m_cameraWidget.drawMenu();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Lighting")) {
				m_phongLightingWidget.drawMenu();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Inspector")) {
				m_inspectorWidget.drawMenu();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Tools")) {	
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		/*
			rendering in main display
		*/ 
		m_profileWidget.draw();
		m_cameraWidget.draw();
		m_phongLightingWidget.draw();
	}

	// ************************************************************************

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}