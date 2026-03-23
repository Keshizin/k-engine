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
		int m_comboFlags = 0;
		kengine::profile* m_profile = nullptr;
	};

	/*
	*/
	class camera_widget : public widget {
	public:
		~camera_widget() {}

		void draw();

		void setCamera(kengine::camera* cam) { m_camera = cam; }
		void setUpdate(bool update) { m_update = update; }
		bool mustBeUpdate() const { return m_update; }

	private:
		bool m_update = false;
		kengine::camera* m_camera = nullptr;
	};

	/*
	*/
	class vectors_widget : public widget {
	public:
		~vectors_widget() {}

		void drawMenu();
		void draw();

	private:
		bool m_isSelected = false;
	};

	/*
	*/
	class projection_widget : public widget {
	public:
		~projection_widget() {}

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

		void setCameraWidget(kengine::camera* cam) { m_cameraWidget.setCamera(cam); }
		camera_widget* getCameraWidget() { return &m_cameraWidget; }

		// ** test **
		void setDemoExample(bool b) { m_demoExample = b; }
		bool getDemoExample() const { return m_demoExample; }

	private:
		int m_width = 1280;
		int m_height = 720;
		bool m_enable = true;

		profile_widget m_profileWidget;
		camera_widget m_cameraWidget;
		vectors_widget m_vectorsWidget;
		projection_widget m_projectionWidget;

		// ** test **
		bool m_demoExample = false;
	};
}

#endif
