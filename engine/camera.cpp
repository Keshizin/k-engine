/*
	K-Engine Camera
	This file is part of the K-Engine.

	Copyright (C) 2020-2026 Fabio Takeshi Ishikawa

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

kengine::matrix<float> kengine::camera::getProjection(bool aspectCorrection) const
{
	kengine::matrix<float> projection(1.0f);
	projection_info<float> info = m_projectionInfo;
	float aspectRatio = 1.0f;

	if (aspectCorrection) {
		if (info.width <= info.height) {
			aspectRatio = info.height / info.width;
			info.bottom *= aspectRatio;
			info.top *= aspectRatio;
		} else {
			aspectRatio = info.width / info.height;
			info.left *= aspectRatio;
			info.right *= aspectRatio;
		}
	}

	switch (m_projectionType) {
	case PROJECTION_TYPE::_2D:
		projection = kengine::ortho(
			info.left,
			info.right,
			info.bottom,
			info.top,
			0.1f,
			100.0f);

		break;

	case PROJECTION_TYPE::_3D_ORTHO:
		projection = kengine::ortho(
			info.left,
			info.right,
			info.bottom,
			info.top,
			m_projectionInfo.zNear,
			m_projectionInfo.zFar);

		break;

	case PROJECTION_TYPE::_3D_PERSPECTIVE:
		projection = kengine::perspective(m_projectionInfo.fovy, aspectRatio, m_projectionInfo.zNear, m_projectionInfo.zFar);
		break;

	case PROJECTION_TYPE::_3D_FRUSTUM:
		projection = kengine::frustum(
			info.left,
			info.right,
			info.bottom,
			info.top,
			m_projectionInfo.zNear,
			m_projectionInfo.zFar);

		break;
	}

	return projection;
}