/*
	K-Engine Time Handler
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

#ifndef K_ENGINE_TIME_HANDLER_H
#define K_ENGINE_TIME_HANDLER_H

// ****************************************************************************
//  K-Engine Time Handler Class
// ****************************************************************************
class KETimeHandler
{
public:
	KETimeHandler();

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	double getFrameTimeInSeconds() const;
	void updateInternalTimer(long long timer);

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	void setFrameTime(long long frameTime);
	long long getFrameTime() const;

	void setInternalTimer(long long internalTimer);
	long long getInternalTimer() const;

	void setFrameTimeLimit(long long frameTimeLimit);
	long long getFrameTimeLimit() const;

	void setPerfomanceFrequency(long long perfomanceFrequency);
	long long getPerfomanceFrequency() const;

private:
	long long frameTime;
	long long internalTimer;
	long long frameTimeLimit;
	long long perfomanceFrequency;
};

#endif