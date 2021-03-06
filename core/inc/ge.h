/*
	Game Engine Core
	This file is part of the BPM Game Engine.

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

#ifndef K_ENGINE_H
#define K_ENGINE_H

//#define K_DEBUG 1

#include <gewindow.h>
#include <getimehandler.h>
//#include <geprofile.h>
//#include <gerenderingsystem.h>
//#include <geversion.h>

// ****************************************************************************
//  K-Engine Runtime States
// ****************************************************************************
#define K_RUNNING 0x01
#define K_STOPPED 0x02
#define K_PAUSED  0x03

// ****************************************************************************
//  K-Engine Class
// ****************************************************************************
class KEngine
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructors
	// ------------------------------------------------------------------------
	explicit KEngine(GEEventHandler *eventHandler);
	~KEngine();

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	void startMainLoop();
	void stopMainLoop();
	void pauseGameLoop();
	void resumeGameLoop();
	void setFrameRate(int framePerSecond);

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	GEWINAPIWrapper* getAPIWrapper() const;
	GEWindow* getGameWindow() const;
	GETimeHandler* getTimeHandler();
	//GEProfile* getProfile();
	//GERenderingSystem* getRenderingSystem();
	void setEventHandler(GEEventHandler *eventHandler);

private:
	GEWINAPIWrapper* apiWrapper;
	GEEventHandler* eventHandler;
	GEWindow* gameWindow;
	GETimeHandler* timeHandler;

	//GERenderingSystem* renderingSystem;
	//GEProfile* profile;
	int runningStatus;
};

#endif