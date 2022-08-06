/*
	K-Engine Event Handler
	This file is part of the K-Engine.

	Copyright (C) 2022 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_EVENT_HANDLER_H
#define K_ENGINE_EVENT_HANDLER_H

#include <vector>

namespace kengine
{
	// ----------------------------------------------------------------------------
	//  kengine::eventhandler class
	// 
	//  This class has methods (callback events) that will be called by the core.
	//  Use this class to create your custom callback events.
	// ----------------------------------------------------------------------------
	class eventhandler
	{
	public:
		virtual ~eventhandler() {}

		virtual void createWindowEvent() = 0;
		virtual void beforeMainLoopEvent() = 0;
		virtual void afterMainLoopEvent() = 0;
		virtual void finishBeforeEvent() = 0;
		virtual void finishAfterEvent() = 0;
		virtual void frameEvent(double frameTime) = 0;
		virtual void resumeEvent() = 0;
		virtual void pauseEvent() = 0;
		virtual void mouseEvent(int button, int state, int x, int y) = 0;
		virtual void mouseMotionEvent(int x, int y) = 0;
		virtual void keyboardEvent(unsigned long long key, int state) = 0;
		virtual void keyboardSpecialEvent(unsigned long long key, int state) = 0;
		virtual void resizeWindowEvent(int width, int height) = 0;
		virtual void moveWindowEvent(int x, int y) = 0;
	};

	class core; // forward declaration
	class scene_manager; // forward declaration

	// ----------------------------------------------------------------------------
	//  kengine::scene class
	// 
	//  This class defines a "scene" in the k-engine context. A scene is used to
	//  encapsulate callback events and make switching scenes easier.
	// ----------------------------------------------------------------------------
	class scene : public eventhandler
	{
	public:
		scene(core* engine, scene_manager* sceneManager);
		~scene();

		scene(const scene& copy) = delete; // copy constructor
		scene(scene&& move) noexcept = delete; // move constructor
		scene& operator=(const scene& copy) = delete; // copy assignment

		void setEngine(core* engine);
		virtual void set();

		virtual void createWindowEvent();
		virtual void beforeMainLoopEvent();
		virtual void afterMainLoopEvent();
		virtual void finishBeforeEvent();
		virtual void finishAfterEvent();
		virtual void frameEvent(double frameTime);
		virtual void resumeEvent();
		virtual void pauseEvent();
		virtual void mouseEvent(int button, int state, int x, int y);
		virtual void mouseMotionEvent(int x, int y);
		virtual void keyboardEvent(unsigned long long key, int state);
		virtual void keyboardSpecialEvent(unsigned long long key, int state);
		virtual void resizeWindowEvent(int width, int height);
		virtual void moveWindowEvent(int x, int y);

	protected:
		core* engineHandle;
		scene_manager* sceneManagerHandle;
	};


	// ----------------------------------------------------------------------------
	//  kengine::scene_manager class
	// 
	//  This class manages scene switching.
	// ----------------------------------------------------------------------------
	class scene_manager
	{
	public:
		scene_manager();
		~scene_manager();

		scene_manager(const scene_manager& copy) = delete; // copy constructor
		scene_manager(scene_manager&& move) noexcept = delete; // move constructor
		scene_manager& operator=(const scene_manager& copy) = delete; // copy assignment

		size_t addScene(scene* scene);
		void removeScene(size_t pos);
		void setScene(size_t pos);

	private:
		std::vector<kengine::scene*> scenes;
	};
}

#endif