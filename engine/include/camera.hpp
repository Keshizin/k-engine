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

#ifndef K_ENGINE_CAMERA_HPP
#define K_ENGINE_CAMERA_HPP

//#include <ke_math.hpp>

namespace kengine
{
	/*
	*
	*  kengine::navigation class
	*
	*  (!) not finished yet
	*/
	//	class navigation
	//	{
	//		friend class camera;
	//
	//	public:
	//		navigation();
	//		~navigation();
	//
	//		navigation(const navigation& copy) = delete; // copy constructor
	//		navigation(navigation&& move) noexcept = delete; // move constructor
	//		navigation& operator=(const navigation& copy) = delete; // copy assignment
	//
	//		void set(int activeButton, int x, int y);
	//		kengine::matrix update(int x, int y);
	//		void clear();
	//		
	//	//private:
	//		kengine::vec3<float> rotation;
	//		kengine::vec3<float> rotation_init;
	//
	//		int mouse_x_init;
	//		int mouse_y_init;
	//		int mouse_active_button;
	//
	//		float sensor_rotate;
	//
	//		//kengine::vec3<float> camera_pos_init;
	//		//float sensor_camera;
	//		//float sensor_translate;
	//	};

		/*
			Enum for render context
		*/
	//enum class RENDER_CONTEXT
	//{
	//	RENDER_CONTEXT_2D,
	//	RENDER_CONTEXT_3D_ORTHO,
	//	RENDER_CONTEXT_3D_FRUSTUM
	//};

	/*
		kengine::camera class
	*/
	class camera
	{
	public:
	private:
	//		camera();
	//		~camera();
	//
	//		camera(const camera& copy) = delete; // copy constructor
	//		camera(camera&& move) noexcept = delete; // move constructor
	//		camera& operator=(const camera& copy) = delete; // copy assignment
	//		camera& operator=(camera&&) = delete; // move assigment
	//
	//		//		void lookAt();
	//		//		void lookAt(kengine::vec3<float> eyeParam, kengine::vec3<float> centerParam, kengine::vec3<float> upParam);
	//		//		void setNavigation(int button, int x, int y);
	//		//		void updateNavigation(int x, int y);
	//		//		void clearNavigation();
	//
	//		//		const kengine::matrix& get() const;
	//
	//		void setWindow(int width, int height);
	//		void setProjectionInfo(const projection_info& projInfo);
	//
	//		kengine::matrix getProjectionMatrix(RENDER_CONTEXT renderContext) const;
	//		projection_info getProjectionInfo() const;
	//
	//		//glm::mat4 getProjectionMatrix() const;
	//
	////		kengine::vec3<float> eye; // camera position
	////		kengine::vec3<float> center; // camera looking at position
	////		navigation* nav;
	//
		/*private:*/
	//		projection_info projectionInfo;
	//		//kengine::matrix viewMatrix; // camera matrix for linear transformation
	};
}

#endif