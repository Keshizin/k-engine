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

/*
	kengine::camera class - member class definition
*/

kengine::camera::camera()
	:
	eye{ 0.0f, 0.0f, 0.0f },
	center{ 0.0f, 0.0f, 0.0f },
	up{ 0.0f, 1.0f, 0.0f }
//		viewMatrix{ 1 },
//		nav{ new kengine::navigation }
{
}

//kengine::camera::~camera()
//{
	//	delete nav;
//}

kengine::matrix<float> kengine::camera::lookAt(const vec4<float>& eyeParam, const vec4<float>& centerParam, const vec4<float>& upParam)
{
	eye = eyeParam;
	center = centerParam;
	up = upParam;
	return kengine::lookAt(eye, center, up);
}

kengine::matrix<float> kengine::camera::getProjection(PROJECTION_TYPE type, bool aspectCorrection) const
{
	kengine::matrix<float> projection(1.0f);
	projection_info<float> info = m_projectionInfo;
	float aspectRatio = 1.0f;

	if (aspectCorrection) {

		if (info.width <= info.height)
		{
			aspectRatio = info.height / info.width;
			info.bottom *= aspectRatio;
			info.top *= aspectRatio;
		}
		else
		{
			aspectRatio = info.width / info.height;
			info.left *= aspectRatio;
			info.right *= aspectRatio;
		}
	}

	if (type == PROJECTION_TYPE::_2D)
	{
		projection = kengine::ortho(
			info.left,
			info.right,
			info.bottom,
			info.top,
			0.1f,
			100.0f);
	}
	else if (type == kengine::PROJECTION_TYPE::_3D_ORTHO)
	{
		projection = kengine::ortho(
			info.left,
			info.right,
			info.bottom,
			info.top,
			m_projectionInfo.zNear,
			m_projectionInfo.zFar
		);
	}
	else if (type == PROJECTION_TYPE::_3D_PERSPECTIVE)
	{
		projection = kengine::perspective(m_projectionInfo.fovy, aspectRatio, m_projectionInfo.zNear, m_projectionInfo.zFar);
	}
	else if(type == PROJECTION_TYPE::_3D_FRUSTUM)
	{
		projection = kengine::frustum(
			info.left,
			info.right,
			info.bottom,
			info.top,
			m_projectionInfo.zNear,
			m_projectionInfo.zFar);
	}

	return projection;
}

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

//kengine::navigation::~navigation()
//{
//}

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

//void kengine::navigation::clear()
//{
//	mouse_x_init = 0;
//	mouse_y_init = 0;
//	mouse_active_button = -1;
//	//camera_pos_init = { 0.0f, 0.0f, 0.0f };
//	//camera_rotate_init = { 0.0f, 0.0f, 0.0f };
//}

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

//kengine::projection_info kengine::camera::getProjectionInfo() const
//{
//	return projectionInfo;
//}