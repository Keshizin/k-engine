/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

	Copyright (C) 2020-2024 Fabio Takeshi Ishikawa

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

#include <demo.hpp>
#include <iostream>
#include <cassert>

game::demo::demo(kengine::core* engine)
	:
	m_engine{ engine },
	m_profile{ 5000 }
{
	m_engine->setEventsCallback(this);
}

void game::demo::start()
{
	m_engine->startMainLoop();
}

void game::demo::beforeMainLoopEvent()
{
	m_profile.init();
}

void game::demo::update(int64_t frameTime)
{
	//static int counter = 0;

	//if (counter >= m_profile.size()) {
	//	m_engine->getWindow()->destroy();
	//	m_profile.print();
	//}

	//m_profile.update(frameTime);
	//counter++;
}

void game::demo::beforeFinishEvent()
{
	assert(!(m_engine == nullptr));
	m_engine->getWindow()->destroy();
}

void game::demo::afterFinishEvent()
{
	assert(!(m_engine == nullptr));
	m_engine->stopMainLoop();
}

void game::demo::onResumeEvent()
{
}

void game::demo::onPauseEvent()
{
}

void game::demo::onResizeWindowEvent(int width, int height)
{
}

void game::demo::onMoveWindowEvent(int x, int y)
{
}

void game::demo::onMouseMotionEvent(int x, int y)
{
}

void game::demo::onMouseButtonEvent(int button, kengine::MOUSE_ACTION state, int x, int y)
{
}

void game::demo::onKeyboardEvent(unsigned long long key, int state)
{
}

void game::demo::onKeyboardSpecialEvent(unsigned long long key, int state)
{
}

void game::demo::debugMessage(const std::string& msg)
{
	std::cout << "> debug message: " << msg << std::endl;
}