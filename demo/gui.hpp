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

#ifndef KENGINE_GUI_HPP
#define KENGINE_GUI_HPP

#include <profile.hpp>
#include <camera.hpp>
#include <k_math.hpp>

#include <vector>

namespace kgui {
	void customWndProcHandler(void* param1, void* param2, void* param3, void* param4);

	/*
		kgui::widget class
	*/
	class widget {
	public:
		virtual ~widget() {}
		virtual void draw() = 0;
	};

	/*
		kgui::profile_widget class
	*/
	class profile_widget : public widget {
	public:
		~profile_widget() {}

		profile_widget() {}

		void draw();
		void setProfile(kengine::profile* p) { p_profile = p; }

	private:
		kengine::profile* p_profile = nullptr;
	};

	/*
		kgui::projection_widget class
	*/
	class projection_widget : public widget {
	public:
		~projection_widget() {}

		void draw() {}

		bool mustBeUpdate() { return m_update; }
		void setUpdate(bool b) { m_update = b; }

		bool mustCameraBeUpdate() { return m_cameraUpdate; }
		void setCameraUpdate(bool b) { m_cameraUpdate = b; }

		bool m_update = false;
		bool m_cameraUpdate = false;
		kengine::PROJECTION_TYPE m_type = kengine::PROJECTION_TYPE::_3D_FRUSTUM;
		kengine::camera* m_cam = nullptr;
	};

	/*
	*/
	class lighting_widget : public widget {
	public:
		~lighting_widget() {}

		void draw() {
			//ImGui::SeparatorText("Lighting:");

			//static int e = 0;
			//ImGui::RadioButton("On", &e, 1); ImGui::SameLine();
			//ImGui::RadioButton("Off", &e, 0);

			//if (e != enableLighting) {
			//	enableLighting = e;
			//	update = true;
			//}

			//ImGui::SeparatorText("Light position:");

			//static float lx = 0.0f;

			//ImGui::InputFloat("x", &lightPosition.x, 0.0f, 1.0f, "%.6f");

			//if (lx != lightPosition.x) {
			//	lx = lightPosition.x;
			//	update = true;
			//}

			//static float ly = 0.0f;

			//ImGui::InputFloat("y", &lightPosition.y, 0.0f, 1.0f, "%.6f");

			//if (ly != lightPosition.y) {
			//	ly = lightPosition.y;
			//	update = true;
			//}

			//static float lz = 0.0f;

			//ImGui::InputFloat("z", &lightPosition.z, 0.0f, 1.0f, "%.6f");

			//if (lz != lightPosition.z) {
			//	lz = lightPosition.z;
			//	update = true;
			//}
		}

		bool mustBeUpdate() { return update; }
		void setUpdate(bool b) { update = b; }

		bool update = false;
		int enableLighting = 1;
		kengine::vec4<float> lightPosition;
	};

	class GraphicalUserInterface {
	public:
		GraphicalUserInterface() {}
		~GraphicalUserInterface() {}

		GraphicalUserInterface(const GraphicalUserInterface& copy) = delete; // copy constructor
		GraphicalUserInterface& operator=(const GraphicalUserInterface& copy) = delete; // copy assignment
		GraphicalUserInterface(GraphicalUserInterface&& move) noexcept = delete;  // move constructor
		GraphicalUserInterface& operator=(GraphicalUserInterface&&) = delete; // move assigment

		void init(void* windowHandle);
		void destroy();

		void setEnable(bool b) { m_enable = b; }
		void setProfile(kengine::profile* p) { m_profileWidget.setProfile(p); }
		void draw();
		void drawView();

		void setCamera(kengine::camera* c) { m_projectionWidget.m_cam = c; }
		projection_widget* getProjectionWidget() { return &m_projectionWidget; }
	
		lighting_widget* getLightingWidget() { return &m_lightingWidget; }

	private:
		bool m_enable = true;
		profile_widget m_profileWidget;
		projection_widget m_projectionWidget;
		lighting_widget m_lightingWidget;
	};
}

#endif
