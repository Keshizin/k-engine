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
	// ----------------------------------------------------------------------------
	//  (!) kengine::camera_navigation class
	// ----------------------------------------------------------------------------
	class camera_navigation
	{
		friend class camera;

	public:
		camera_navigation();
		~camera_navigation();

		camera_navigation(const camera_navigation& copy) = delete; // copy constructor
		camera_navigation(camera_navigation&& move) noexcept = delete; // move constructor
		camera_navigation& operator=(const camera_navigation& copy) = delete; // copy assignment

		void set(int button, int x, int y, const kengine::vec3<float>& from, const kengine::vec3<float>& rotate);
		void clear();
		
	private:
		int mouse_x_init;
		int mouse_y_init;
		int mouse_button;

		kengine::vec3<float> camera_pos_init;
		kengine::vec3<float> camera_rotate_init;

		float sensor_rotate;
		float sensor_camera;
		float sensor_translate;
	};


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

		void setNavigation(int button, int x, int y);
		void clearNavigation();
		void updateNavigation(int x, int y);
		
		const kengine::matrix& get() const;
		void setViewingMatrix();

	private:
		kengine::vec3<float> right;		// x-xis
		kengine::vec3<float> up;		// y-axis
		kengine::vec3<float> forward;	// z-axis
		kengine::vec3<float> from;		// camera position
		kengine::vec3<float> to;		// camera looking at position
		kengine::vec3<float> rotate;	// camera rotate

		kengine::matrix viewingMatrix;	// camera matrix for linear transformation

		camera_navigation navigation;
	};
}

#endif