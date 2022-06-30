/*
	K-Engine Entity
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

#include <keentity.h>


kengine::animation2D::animation2D(ANIMATION_TYPE typeParam, int frame, int totalFramesParam, long long frameTime)
	:
		type{ typeParam },
		timer{ frameTime },
		startFrame{ frame },
		currentFrame{ frame },
		totalFrames{ totalFramesParam }
{
}


kengine::animation2D::~animation2D()
{
}


void kengine::animation2D::start()
{
	timer.start();
	currentFrame = startFrame;
}


void kengine::animation2D::update()
{
	if (currentFrame == -1)
		return;

	if (timer.isDoneAndRestart())
	{
		currentFrame++;

		if (currentFrame > totalFrames - 1)
		{
			if (type == ANIMATION_TYPE::CONTINUOUS)
				currentFrame = startFrame;

			if (type == ANIMATION_TYPE::FINITE)
				currentFrame = -1;
		}
	}
}


int kengine::animation2D::getCurrentFrame() const
{
	if (currentFrame == -1)
		return totalFrames - 1;

	return currentFrame;
}