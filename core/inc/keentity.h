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

#ifndef K_ENGINE_ENTITY_H
#define K_ENGINE_ENTITY_H

#include <ketimer.h>

namespace kengine
{
	// ----------------------------------------------------------------------------
	//  kengine::entity class
	// ----------------------------------------------------------------------------
	class entity
	{
	public:
		entity() {}
		virtual ~entity() {}

		entity(const entity& copy) = delete; // copy constructor
		entity(entity&& move) noexcept = delete; // move constructor
		entity& operator=(const entity& copy) = delete; // copy assignment

		virtual void update(double frameTime) = 0;
	};


	enum class ANIMATION_TYPE { CONTINUOUS, FINITE };

	// ------------------------------------------------------------------------
	//  kengine::animation2D
	// ------------------------------------------------------------------------
	class animation2D
	{
	public:
		animation2D(ANIMATION_TYPE typeParam, int frame, int totalFramesParam, long long frameTime);
		~animation2D();

		animation2D(const animation2D& copy) = delete; // copy constructor
		animation2D(animation2D&& move) noexcept = delete; // move constructor
		animation2D& operator=(const animation2D& copy) = delete; // copy assignment

		void start();
		void update();
		int getCurrentFrame() const;

	private:
		ANIMATION_TYPE	type;
		kengine::timer	timer;
		int				startFrame;
		int				currentFrame;
		int				totalFrames;
	};
}

#endif