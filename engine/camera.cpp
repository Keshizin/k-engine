/*
	K-Engine Camera
	This file is part of the K-Engine.

	Copyright (C) 2020-2025 Fabio Takeshi Ishikawa

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

#include <camera.hpp>
//#include <keaux.h>
//#include <keconstants.h>

/*
*
*  kengine::camera_navigation class - member class definition
*
*/

//kengine::navigation::navigation()
//	:
//	rotation{ 0.0f } ,
//	rotation_init{ 0.0f },
//	mouse_x_init{ 0 },
//	mouse_y_init{ 0 },
//	mouse_active_button{ -1 },
//	sensor_rotate{ 500.0f }
//	//	camera_pos_init{ 0.0f, 0.0f, 0.0f },
//	//	sensor_camera{ 15.0f },
//	//	sensor_translate{ 3.0f }
//{
//}
//
//
//kengine::navigation::~navigation()
//{
//}
//
//
//void kengine::navigation::set(int activeButton, int x, int y)
//{
//	mouse_x_init = x;
//	mouse_y_init = y;
//	rotation_init = rotation;
//	mouse_active_button = activeButton;
//
//	//	mouse_button = button;
//	//	camera_pos_init = from;
//	//	camera_rotate_init = rotate;
//}
//
//
//kengine::matrix kengine::navigation::update(int x, int y)
//{
//	kengine::matrix r(1);
//
//	if (mouse_active_button != -1)
//	{
//		int deltax = mouse_x_init - x;
//		int deltay = mouse_y_init - y;
//		
//		float _y = rotation_init.y - static_cast<float>(deltax) / sensor_rotate;
//		float _x = rotation_init.x - static_cast<float>(deltay) / sensor_rotate;
//		rotation = rotation_init;
//
//		kengine::matrix ry = kengine::rotate(_y, 0.0f, 1.0f, 0.0f);
//		kengine::matrix rx = kengine::rotate(_x, 1.0f, 0.0f, 0.0f);
//		r = ry * rx;
//	}
//
//	return r;
//}
//
//
//void kengine::navigation::clear()
//{
//	mouse_x_init = 0;
//	mouse_y_init = 0;
//	mouse_active_button = -1;
//	//camera_pos_init = { 0.0f, 0.0f, 0.0f };
//	//camera_rotate_init = { 0.0f, 0.0f, 0.0f };
//}

/*
*
*  kengine::camera class - member class definition
*
*/
//kengine::camera::camera()
//	: projectionInfo{}
	//	eye{ 0.0f, 0.0f, 0.0f },
	//	center{ 0.0f, 0.0f, 0.0f },
	//	viewMatrix{ 1 },
	//	nav{ new kengine::navigation }
//{
//}

//kengine::camera::~camera()
//{
	//	delete nav;
//}

//void kengine::camera::lookAt()
//{
//	viewMatrix = kengine::lookAt(eye, center, { 0.0f, 1.0f, 0.0f });
//}
//
//
//void kengine::camera::lookAt(kengine::vec3<float> eyeParam, kengine::vec3<float> centerParam, kengine::vec3<float> upParam)
//{
//	eye = eyeParam;
//	center = centerParam;
//	
//	viewMatrix = kengine::lookAt(eye, center, upParam);
//}
//
//
//void kengine::camera::setNavigation(int button, int x, int y)
//{
//	if (nav == nullptr)
//		return;
//	
//	nav->set(button, x, y);
//}


//void kengine::camera::updateNavigation(int x, int y)
//{
//	kengine::matrix r = nav->update(x, y);
//	viewMatrix = viewMatrix * r;
//}


//void kengine::camera::clearNavigation()
//{
//	if (nav == nullptr)
//		return;
//
//	nav->clear();
//}



//const kengine::matrix& kengine::camera::get() const
//{
//	return viewMatrix;
//}


//void kengine::camera::setWindow(int width, int height)
//{
//	projectionInfo.window_width = width;
//	projectionInfo.window_height = height;
//}
//
//
//void kengine::camera::setProjectionInfo(const projection_info& projInfo)
//{
//	projectionInfo = projInfo;
//}
//
//
//kengine::matrix kengine::camera::getProjectionMatrix(RENDER_CONTEXT renderContext) const
//{
//	kengine::matrix projection(1.0f);
//
//	float aspectRatio;
//	kengine::rect window = projectionInfo.plane_size;
//
//	if (projectionInfo.window_width <= projectionInfo.window_height)
//	{
//		aspectRatio = static_cast<float>(projectionInfo.window_height) / static_cast<float>(projectionInfo.window_width);
//		window.bottom *= aspectRatio;
//		window.top *= aspectRatio;
//	}
//	else
//	{
//		aspectRatio = static_cast<float>(projectionInfo.window_width) / static_cast<float>(projectionInfo.window_height);
//		window.left *= aspectRatio;
//		window.right *= aspectRatio;
//	}
//
//	if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_2D)
//	{
//		projection = kengine::ortho(
//			window.left,
//			window.right,
//			window.bottom,
//			window.top,
//			-1.0f,
//			1.0f);
//	}
//	else if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_3D_ORTHO)
//	{
//		projection = kengine::ortho(
//			window.left,
//			window.right,
//			window.bottom,
//			window.top,
//			projectionInfo.near_plane,
//			projectionInfo.far_plane
//		);
//	}
//	else if (renderContext == kengine::RENDER_CONTEXT::RENDER_CONTEXT_3D_FRUSTUM)
//	{
//		projection = kengine::perspective(projectionInfo.fovy, aspectRatio, projectionInfo.near_plane, projectionInfo.far_plane);
//
//		//projection = kengine::frustum(
//		//	window.left,
//		//	window.right,
//		//	window.bottom,
//		//	window.top,
//		//	viewingWindow._near,
//		//	viewingWindow._far);
//
//		//projection = glm::perspective(projectionInfo.fovy, aspectRatio, projectionInfo.near_plane, projectionInfo.far_plane);
//	}
//
//	return projection;
//}
//
//
//kengine::projection_info kengine::camera::getProjectionInfo() const
//{
//	return projectionInfo;
//}