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
	/*
	* 
	*  kengine::camera_navigation class
	* 
	*  (!) not finished yet
	*/
	class camera_navigation
	{
		friend class camera;

	public:
		camera_navigation();
		~camera_navigation();

		camera_navigation(const camera_navigation& copy) = delete; // copy constructor
		camera_navigation(camera_navigation&& move) noexcept = delete; // move constructor
		camera_navigation& operator=(const camera_navigation& copy) = delete; // copy assignment

		void set(int activeButton, int x, int y);
		kengine::matrix update(int x, int y);
		void clear();
		
	private:
		int mouse_x_init;
		int mouse_y_init;
		int mouse_active_button;

		kengine::vec3<float> rotate_init;
		float sensor_rotate;

		//kengine::vec3<float> camera_pos_init;
		//float sensor_camera;
		//float sensor_translate;
	};


	/*
	*
	*  Enum for render context
	*
	*/
	enum class RENDER_CONTEXT
	{
		RENDER_CONTEXT_2D,
		RENDER_CONTEXT_3D_ORTHO,
		RENDER_CONTEXT_3D_FRUSTUM
	};


	/*
	*
	*  kengine::viewing_info class
	* 
	*  This class define the properties for the following viewing models:
	*	- perspective viewing model
	*	- orthographic viewing model
	*/
	class viewing_info
	{
	public:
		viewing_info();
		~viewing_info();

		viewing_info(const viewing_info& copy) = delete; // copy constructor
		viewing_info(viewing_info&& move) noexcept = delete; // move constructor
		viewing_info& operator=(const viewing_info& copy) = delete; // copy assignment

		void set(int width, int height, float left, float right, float bottom, float top, float fovyParam, float nearPlane, float farPlane);

		kengine::rect plane_size;
		int window_width;
		int window_height;
		float near_plane;
		float far_plane;
		float fovy; // field of view in Y axis for perspective use
	};


	/*
	* 
	*  kengine::camera class
	* 
	*/
	class camera
	{
	public:
		camera();
		~camera();

		camera(const camera& copy) = delete; // copy constructor
		camera(camera&& move) noexcept = delete; // move constructor
		camera& operator=(const camera& copy) = delete; // copy assignment

		void lookAt();
		void lookAt(kengine::vec3<float> eyeParam, kengine::vec3<float> centerParam, kengine::vec3<float> upParam);

		void setNavigation(int button, int x, int y);
		void updateNavigation(int x, int y);
		void clearNavigation();
		
		const kengine::matrix& get() const;
		kengine::matrix getProjectionMatrix() const;

		kengine::vec3<float> eye; // camera position
		kengine::vec3<float> center; // camera looking at position
		kengine::viewing_info viewingInfo;
		RENDER_CONTEXT renderContext;

	private:
		kengine::matrix viewMatrix; // camera matrix for linear transformation
		camera_navigation* navigation;
	};
}

#endif