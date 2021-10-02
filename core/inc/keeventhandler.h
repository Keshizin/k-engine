/*
	K-Engine Event Handler
	This file is part of the K-Engine.

	Copyright (C) 2021 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_EVENT_HANDLER_CLASS_H
#define K_ENGINE_EVENT_HANDLER_CLASS_H

// ****************************************************************************
//  K-Engine Events Class
// ****************************************************************************
class KEEventHandler
{
public:
	virtual ~KEEventHandler() {}

	virtual void frameEvent(double frameTime) = 0;
	virtual void mouseEvent(int button, int state, int x, int y) = 0;
	virtual void mouseMotionEvent(int x, int y) = 0;
	virtual void keyboardEvent(unsigned long long key, int state) = 0;
	virtual void keyboardSpecialEvent(unsigned long long key, int state) = 0;
	virtual void resizeWindowEvent(int width, int height) = 0;
	virtual void moveWindowEvent(int x, int y) = 0;
	virtual void finishAfterEvent() = 0;
	virtual void finishBeforeEvent() = 0;
	virtual void resumeEvent() = 0;
	virtual void pauseEvent() = 0;
	virtual void beforeMainLoopEvent() = 0;
	virtual void afterMainLoopEvent() = 0;
	virtual void createWindowEvent() = 0;
};

#endif