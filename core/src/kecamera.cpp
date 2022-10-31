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


/*
*
*  kengine::camera_navigation class - member class definition
*
*/

kengine::camera_navigation::camera_navigation()
	:
	mouse_x_init{ 0 },
	mouse_y_init{ 0 },
	mouse_active_button{ -1 },
	rotate_init{ 1 },
	sensor_rotate{ 5.0f }
	//	camera_pos_init{ 0.0f, 0.0f, 0.0f },
	//	sensor_camera{ 15.0f },
	//	sensor_translate{ 3.0f }
{
}


kengine::camera_navigation::~camera_navigation()
{
}


void kengine::camera_navigation::set(int activeButton, int x, int y)
{
	mouse_x_init = x;
	mouse_y_init = y;
	mouse_active_button = activeButton;

	//	mouse_button = button;
	//	camera_pos_init = from;
	//	camera_rotate_init = rotate;
}


kengine::matrix kengine::camera_navigation::update(int x, int y)
{
	kengine::matrix r(1);

	if (mouse_active_button != -1)
	{
		int deltax = mouse_x_init - x;
		int deltay = mouse_y_init - y;
		
		float _y = rotate_init.y - static_cast<float>(deltax) / sensor_rotate;
		float _x = rotate_init.x - static_cast<float>(deltay) / sensor_rotate;

		kengine::matrix ry = kengine::rotate(0.0f, _y, 0.0f);
		kengine::matrix rx = kengine::rotate(_x, 0.0f, 0.0f);
		r = ry * rx;
	}

	return r;
}


void kengine::camera_navigation::clear()
{
	mouse_x_init = 0;
	mouse_y_init = 0;
	mouse_active_button = -1;
	//camera_pos_init = { 0.0f, 0.0f, 0.0f };
	//camera_rotate_init = { 0.0f, 0.0f, 0.0f };
}


/*
*
*  kengine::viewing_info class - member class definition
*
*/
kengine::viewing_info::viewing_info()
	:
	plane_size{ -1.0f, 1.0f, -1.0f, 1.0f },
	window_width{ 640 },
	window_height{ 480 },
	near_plane{ 0.1f },
	far_plane{ 1000.0f },
	fovy{ 39.6f }
{
}


kengine::viewing_info::~viewing_info()
{
}


void kengine::viewing_info::set(int width, int height, float left, float right, float bottom, float top, float fovyParam, float nearPlane, float farPlane)
{
	window_width = width;
	window_height = height;
	plane_size.left = left;
	plane_size.right = right;
	plane_size.bottom = bottom;
	plane_size.top = top;
	near_plane = nearPlane;
	far_plane = farPlane;
	fovy = fovyParam;
}


/*
*
*  kengine::camera class - member class definition
*
*/
kengine::camera::camera()
	:
	eye{ 0.0f, 0.0f, 0.0f },
	center{ 0.0f, 0.0f, 0.0f },
	viewingInfo{},
	renderContext{ RENDER_CONTEXT::RENDER_CONTEXT_2D },
	viewMatrix{ 1 },
	navigation{ nullptr }
{
	navigation = new kengine::camera_navigation;
}


kengine::camera::~camera()
{
	delete navigation;
}


void kengine::camera::lookAt()
{
	viewMatrix = kengine::lookAt(eye, center, { 0.0f, 1.0f, 0.0f });
}


void kengine::camera::lookAt(kengine::vec3<float> eyeParam, kengine::vec3<float> centerParam, kengine::vec3<float> upParam)
{
	eye = eyeParam;
	center = centerParam;
	
	viewMatrix = kengine::lookAt(eye, center, upParam);
}


void kengine::camera::setNavigation(int button, int x, int y)
{
	navigation->set(button, x, y);
}


void kengine::camera::updateNavigation(int x, int y)
{
	kengine::matrix r = navigation->update(x, y);
	viewMatrix = viewMatrix * r;
}


void kengine::camera::clearNavigation()
{
	navigation->clear();
}


const kengine::matrix& kengine::camera::get() const
{
	return viewMatrix;
}


kengine::matrix kengine::camera::getProjectionMatrix() const
{
	kengine::matrix projection(1);

	float aspectRatio;
	kengine::rect window = viewingInfo.plane_size;

	if (viewingInfo.window_width <= viewingInfo.window_height)
	{
		aspectRatio = static_cast<float>(viewingInfo.window_height) / static_cast<float>(viewingInfo.window_width);
		window.bottom *= aspectRatio;
		window.top *= aspectRatio;
	}
	else
	{
		aspectRatio = static_cast<float>(viewingInfo.window_width) / static_cast<float>(viewingInfo.window_height);
		window.left *= aspectRatio;
		window.right *= aspectRatio;
	}

	if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_2D)
	{
		projection = kengine::ortho(
			window.left,
			window.right,
			window.bottom,
			window.top,
			-1.0f,
			1.0f);
	}
	else if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_3D_ORTHO)
	{
		projection = kengine::ortho(
			window.left,
			window.right,
			window.bottom,
			window.top,
			viewingInfo.near_plane,
			viewingInfo.far_plane);
	}
	else if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_3D_FRUSTUM)
	{
		projection = kengine::perspective(viewingInfo.fovy, aspectRatio, viewingInfo.near_plane, viewingInfo.far_plane);

		//projection = kengine::frustum(
		//	window.left,
		//	window.right,
		//	window.bottom,
		//	window.top,
		//	viewingWindow._near,
		//	viewingWindow._far);
	}

	return projection;
}