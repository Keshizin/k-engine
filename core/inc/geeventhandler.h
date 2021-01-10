/*
	Game Engine Event Handler
	This file is part of the K-Engine.

	Copyright (C) 2020 Fabio Takeshi Ishikawa

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
//  Game Engine Events Class
// ****************************************************************************
class GEEventHandler
{
public:
	virtual ~GEEventHandler() {}
	virtual void frameEvent() {}
	virtual void mouseEvent(int button, int state, int x, int y) {}
	virtual void mouseMotionEvent(int x, int y) {}
	virtual void keyboardEvent(unsigned char key, int state) {}
	virtual void keyboardSpecialEvent(unsigned char key, int state) {}
	virtual void resizeWindowEvent(int width, int height) {}
	virtual void finishAfterEvent() {}
	virtual void finishBeforeEvent() {}
	virtual void resumeEvent() {}
	virtual void pauseEvent() {}
	virtual void beforeMainLoopEvent() {}
	virtual void createWindowEvent() {}
};

#endif