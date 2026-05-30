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

#ifndef K_ENGINE_CAMERA_HPP
#define K_ENGINE_CAMERA_HPP

#include <k_math.hpp>

namespace kengine
{
	/*
	*/
	enum class PROJECTION_TYPE
	{
		_2D,
		_3D_ORTHO,
		_3D_FRUSTUM,
		_3D_PERSPECTIVE
	};

	/*
	*/
	class camera
	{
	public:
		camera() {}
		~camera() {}

		camera(const camera& copy) = delete; // copy constructor
		camera(camera&& move) noexcept = delete; // move constructor
		camera& operator=(const camera& copy) = delete; // copy assignment
		camera& operator=(camera&&) = delete; // move assigment

		matrix<float> getProjection(bool aspectCorrection) const;
		matrix<float> lookAt() { return kengine::lookAt(m_eye, m_center, m_up); }
	
		void setProjectionType(const PROJECTION_TYPE& type) { m_projectionType = type; }
		PROJECTION_TYPE getProjectionType() const { return m_projectionType; }

		void setProjectionInfo(const projection_info<float>& projInfo) { m_projectionInfo = projInfo; }
		projection_info<float> getProjectionInfo() const { return m_projectionInfo; }
		projection_info<float>* getProjectionInfo() { return &m_projectionInfo; }

		void setEye(float v[3]) {
			m_eye.x = v[0];
			m_eye.y = v[1];
			m_eye.z = v[2];
		}

		void setCenter(float v[3]) {
			m_center.x = v[0];
			m_center.y = v[1];
			m_center.z = v[2];
		}

		void setUp(float v[3]) {
			m_up.x = v[0];
			m_up.y = v[1];
			m_up.z = v[2];
		}

		void setEye(const vec3<float>& eye) { m_eye = eye; }
		vec3<float> getEye() const { return m_eye; }

		void setCenter(const vec3<float>& center) { m_center = center; }
		vec3<float> getCenter() const { return m_center; }

		void setUp(const vec3<float>& up) { m_up = up; }
		vec3<float> getUp() const { return m_up; }

		void setCamera(const vec3<float>& eye, const vec3<float>& center, const vec3<float>& up)
		{
			m_eye = eye;
			m_center = center;
			m_up = up;
		}

	private:
		PROJECTION_TYPE m_projectionType = PROJECTION_TYPE::_3D_FRUSTUM;
		projection_info<float> m_projectionInfo;

		vec3<float> m_eye;
		vec3<float> m_center;
		vec3<float> m_up;
	};

	/*
	*/
	class camera_control
	{
	public:
		int m_lastMouseX = 0;
		int m_lastMouseY = 0;
		float m_sensitivity = 0.1f;
		float m_yaw = -90.0f;
		float m_pitch = 0.0f;
		float m_roll = 0.0f;
		float m_xoffset = 0.0f;
		float m_yoffset = 0.0f;

		vec3<float> m_forward;
	};
}

#endif