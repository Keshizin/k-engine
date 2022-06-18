/*
	K-Engine Camera Class
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


#ifndef K_ENGINE_CAMERA_H
#define K_ENGINE_CAMERA_H

#include <keentity.h>
#include <kemath.h>

namespace kengine
{
	// ------------------------------------------------------------------------
	//  (!) kengine::camera class
	// ------------------------------------------------------------------------
	class camera : public kengine::entity
	{
	public:
		camera();
		~camera();

		camera(const camera& copy) = delete; // copy constructor
		camera(camera&& move) noexcept = delete; // move constructor
		camera& operator=(const camera& copy) = delete; // copy assignment

		void update(double frameTime);
		void lookAt(kengine::vec3<float> fromParam, kengine::vec3<float> toParam);
		const kengine::matrix& get() const;

	private:
		kengine::vec3<float> right;		// x-xis
		kengine::vec3<float> up;		// y-axis
		kengine::vec3<float> forward;	// z-axis
		kengine::vec3<float> from;      // camera position
		kengine::matrix viewingTransformation;
	};
}

#endif