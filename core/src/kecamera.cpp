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

#include <kecamera.h>

// ----------------------------------------------------------------------------
//  kengine::camera class - member class definition
// ----------------------------------------------------------------------------
kengine::camera::camera()
	: 
		right{ 0.0f, 0.0f, 0.0f},
		up{ 0.0f, 0.0f, 0.0f },
		forward{ 0.0f, 0.0f, 0.0f },
		from{ 0.0f, 0.0f, 0.0f },
		viewingTransformation{ 1 }
{
}


kengine::camera::~camera()
{
}


void kengine::camera::update(double frameTime)
{
}


void kengine::camera::lookAt(kengine::vec3<float> fromParam, kengine::vec3<float> toParam)
{
	from = fromParam;

	// computing forward vector
	forward = toParam - fromParam;
	forward.normalize();

	// computing right vector
	kengine::vec3<float> temp(0.0f, 1.0f, 0.0f);
	temp.normalize();
	right = temp.crossProduct(forward);

	// computing up vector
	up = forward.crossProduct(right);

	viewingTransformation = kengine::lookAt(right, up, forward, from);
}


const kengine::matrix& kengine::camera::get() const
{
	return viewingTransformation;
}