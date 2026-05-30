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

#ifndef KENGINE_GUI_HPP
#define KENGINE_GUI_HPP

#include <profile.hpp>
#include <camera.hpp>
#include <lighting.hpp>

#include <vector>

namespace kengine {
	int customWndProcHandler(void* param1, void* param2, void* param3, void* param4);

	/*
	*/
	class widget {
	public:
		virtual ~widget() {}
		virtual void draw() = 0;
	};

	/*
	*/
	class profile_widget : public widget {
	public:
		~profile_widget() {}

		void draw();
		void drawMenu();
		void setProfile(kengine::profile* p) { m_profile = p; }

	private:
		bool m_isSelected = true;
		kengine::profile* m_profile = nullptr;
	};

	/*
	*/
	class camera_widget : public widget {
	public:
		~camera_widget() {}

		void draw();
		void drawMenu();

		void setCamera(kengine::camera* cam) { m_camera = cam; }
		void setCameraControl(kengine::camera_control* camControl) { m_cameraControl = camControl; }

		void setUpdate(bool update) { m_update = update; }
		bool mustBeUpdate() const { return m_update; }

	private:
		bool m_isCameraSelected = false;
		bool m_isCameraControlSelected = false;
		bool m_update = false;

		kengine::camera* m_camera = nullptr;
		kengine::camera_control* m_cameraControl = nullptr;
	};

	/*
	*/
	class PhongLightingWidget : public widget {
	public:
		~PhongLightingWidget() {}

		void draw();
		void drawMenu();

		void set(kengine::phong_light* l, kengine::phong_material* m) { m_light = l; m_material = m; }

		void setUpdate(bool update) { m_update = update; }
		bool mustBeUpdate() const { return m_update; }

	private:
		bool m_isSelected = false;
		bool m_update = false;

		kengine::phong_light* m_light = nullptr;
		kengine::phong_material* m_material = nullptr;
	};

	/*
	*/
	class inspector_widget : public widget {
	public:
		~inspector_widget() {}

		void drawMenu();
		void draw();

	private:
		bool m_isSelected = false;
	};

	/*
	*/
	class gui {
	public:
		gui() {}
		~gui() {}

		gui(const gui& copy) = delete; // copy constructor
		gui& operator=(const gui& copy) = delete; // copy assignment
		gui(gui&& move) noexcept = delete;  // move constructor
		gui& operator=(gui&&) = delete; // move assigment

		void init(void* windowHandle);
		void destroy();
		void draw();

		void setEnable(bool b) { m_enable = b; }
		bool getEnable() const { return m_enable; }

		void setWindowSize(int width, int height) { m_width = width; m_height = height; }

		void setProfile(kengine::profile* p) { m_profileWidget.setProfile(p); }

		void setCameraWidget(kengine::camera* cam, kengine::camera_control* camControl) { m_cameraWidget.setCamera(cam); m_cameraWidget.setCameraControl(camControl); }
		camera_widget* getCameraWidget() { return &m_cameraWidget; }

		void setPhongLighting(kengine::phong_light* l, kengine::phong_material* m) { m_phongLightingWidget.set(l, m); }
		PhongLightingWidget* getPhongLightingWidget() { return &m_phongLightingWidget; }
		
		// ** test **
		void setDemoExample(bool b) { m_demoExample = b; }
		bool getDemoExample() const { return m_demoExample; }

	private:
		int m_width = 1280;
		int m_height = 720;
		bool m_enable = true;

		profile_widget m_profileWidget;
		camera_widget m_cameraWidget;
		PhongLightingWidget m_phongLightingWidget;
		inspector_widget m_inspectorWidget;

		// ** test **
		bool m_demoExample = false;
	};
}

#endif
