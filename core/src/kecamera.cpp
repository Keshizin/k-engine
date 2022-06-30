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
#include <keaux.h>
#include <keconstants.h>


// ----------------------------------------------------------------------------
//  (!) kengine::cam_navigation class - member class definition
// ----------------------------------------------------------------------------
kengine::camera_navigation::camera_navigation()
	:
		mouse_x_init{ 0 },
		mouse_y_init{ 0 },
		mouse_button{ -1 },
		camera_pos_init{ 0.0f, 0.0f, 0.0f },
		camera_rotate_init{ 0.0f, 0.0f, 0.0f },
		sensor_rotate{ 5.0f },
		sensor_camera{ 15.0f },
		sensor_translate{ 3.0f }
{
}


kengine::camera_navigation::~camera_navigation()
{
}


void kengine::camera_navigation::set(int button, int x, int y, const kengine::vec3<float>& from, const kengine::vec3<float>& rotate)
{
	mouse_x_init = x;
	mouse_y_init = y;
	mouse_button = button;
	camera_pos_init = from;
	camera_rotate_init = rotate;
	
}


void kengine::camera_navigation::clear()
{
	mouse_x_init = 0;
	mouse_y_init = 0;
	mouse_button = -1;
	camera_pos_init = { 0.0f, 0.0f, 0.0f };
	camera_rotate_init = { 0.0f, 0.0f, 0.0f };
}


// ----------------------------------------------------------------------------
//  kengine::camera class - member class definition
// ----------------------------------------------------------------------------
kengine::camera::camera()
	: 
		right{ 0.0f, 0.0f, 0.0f},
		up{ 0.0f, 0.0f, 0.0f },
		forward{ 0.0f, 0.0f, 0.0f },
		from{ 0.0f, 0.0f, 0.0f },
		viewingMatrix{ 1 }
{
}


kengine::camera::~camera()
{
}


void kengine::camera::update(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
}


void kengine::camera::lookAt(kengine::vec3<float> fromParam, kengine::vec3<float> toParam)
{
	from = fromParam;
	to = toParam;

	// computing forward vector
	forward = toParam - fromParam;
	forward.normalize();

	// computing right vector
	kengine::vec3<float> temp(0.0f, 1.0f, 0.0f);
	temp.normalize();
	right = temp.crossProduct(forward);

	// computing up vector
	up = forward.crossProduct(right);

	viewingMatrix = kengine::lookAt(right, up, forward, from);
}


void kengine::camera::setNavigation(int button, int x, int y)
{
	navigation.set(button, x, y, from, rotate);
}


void kengine::camera::clearNavigation()
{
	navigation.clear();
}


void kengine::camera::updateNavigation(int x, int y)
{
	if (navigation.mouse_button == K_MOUSE_LEFT_BUTTON)
	{
		float deltaX = navigation.camera_pos_init.x - static_cast<float>(x);
		float deltaY = navigation.camera_pos_init.y - static_cast<float>(x);

		rotate.y = navigation.camera_rotate_init.y - deltaX / navigation.sensor_rotate;
		rotate.x = navigation.camera_rotate_init.x - deltaY / navigation.sensor_rotate;

		setViewingMatrix();
	}

	if (navigation.mouse_button == K_MOUSE_RIGHT_BUTTON)
	{
		float deltaZ = navigation.camera_pos_init.y - static_cast<float>(y);
		from.z = navigation.camera_pos_init.z + deltaZ / navigation.sensor_camera;

		setViewingMatrix();
	}
}


const kengine::matrix& kengine::camera::get() const
{
	return viewingMatrix;
}


void kengine::camera::setViewingMatrix()
{
	lookAt(from, to);

	kengine::matrix rx = kengine::rotate(
		rotate.x,
		0.0f,
		0.0f);

	kengine::matrix ry = kengine::rotate(
		0.0f,
		rotate.y,
		0.0f);

	viewingMatrix = viewingMatrix * ry * rx;
}