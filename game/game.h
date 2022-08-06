/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#ifndef K_ENGINE_GAME_H
#define K_ENGINE_GAME_H

#include <kecore.h>

#define MONITOR_WIDTH	2560
#define MONITOR_HEIGHT	1080

namespace game
{
	// ------------------------------------------------------------------------
	//  GameScene class
	// ------------------------------------------------------------------------
	class GameScene : public kengine::scene
	{
	public:
		GameScene(kengine::core* engine, kengine::scene_manager* sceneManager);
		~GameScene();

		GameScene(const GameScene& copy) = delete; // copy constructor
		GameScene(GameScene&& move) noexcept = delete; // move constructor
		GameScene& operator=(const GameScene& copy) = delete; // copy assignment

		void set();

		void createWindowEvent();
		void beforeMainLoopEvent();
		void afterMainLoopEvent();
		void finishBeforeEvent();
		void finishAfterEvent();
		void frameEvent(double frameTime);
		void resumeEvent();
		void pauseEvent();
		void mouseEvent(int button, int state, int x, int y);
		void mouseMotionEvent(int x, int y);
		void keyboardEvent(unsigned long long key, int state);
		void keyboardSpecialEvent(unsigned long long key, int state);
		void resizeWindowEvent(int width, int height);
		void moveWindowEvent(int x, int y);

	private:
		int windowWidth;
		int windowHeight;
	};
}

#endif