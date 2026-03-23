/*
	K-Engine GLTF Wrapper
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

#ifndef K_ENGINE_GLTF_WRAPPER_HPP
#define K_ENGINE_GLTF_WRAPPER_HPP

#include <gl_wrapper.hpp>

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

/*
	glTF https://www.khronos.org/gltf/#gltf-spec
*/
#define GLTF_MIN_VERSION 20 // 2.0

namespace kengine
{
	/*
	*/
	class gltf_render
	{
	public:
		gltf_render();
		~gltf_render();

		gltf_render(const gltf_render& copy) = delete; // copy constructor
		gltf_render(gltf_render&& move) noexcept; // move constructor
		gltf_render& operator=(const gltf_render& copy) = delete; // copy assignment
		gltf_render& operator=(gltf_render&& move) = delete; // move assigment

		kengine::gpu_buffer m_gpuBuffers;
	};

	/*
		This function loads an GLTF file (https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html)
	*/
	void loadGLTF(const std::string& path, const std::string& filename, std::unordered_map<std::string, GLuint>& shaderLocations);

	/*
	*/
	class gltf_asset
	{
	public:
		std::string copyright;
		std::string generator;
		std::string version; // required
		std::string minVersion;
		// extensions
		// extras

		explicit gltf_asset(const nlohmann::json& data);

		bool checkVersion(int version) {
			return true;
		}

		std::string dump();
	};

	/*
	*/
	class gltf_buffer
	{
	public:
		std::string name;
		std::string uri;
		size_t byteLength = 0;
		unsigned char* m_data = nullptr;

		gltf_buffer(const nlohmann::json& _data, const std::string& _path); // constructor with arguments
		~gltf_buffer();

		gltf_buffer(const gltf_buffer& copy); // copy constructor
		gltf_buffer(gltf_buffer&& _move) noexcept; // move constructor
		gltf_buffer& operator=(const gltf_buffer& right); // copy assignment
		gltf_buffer& operator=(gltf_buffer&& right) noexcept; // move assigment

		void clear();

		unsigned char* data() {
			return m_data;
		}

		bool empty() const { return m_data == nullptr; }
	};

	/*
	*/
	class gltf_bufferView
	{
	public:
		size_t m_buffer = 0; // required
		size_t m_byteOffset = 0;
		size_t m_byteLength = 0; // required
		size_t m_byteStride = 0;
		size_t m_target = 0;
		std::string m_name;
		// extensions
		// extras

		gltf_bufferView(const nlohmann::json& data);
	};

	/*
	*/
	class gltf_accessor {
	public:
		size_t m_bufferView = 0;
		size_t m_byteOffset = 0;
		size_t m_componentType = 0; // required
		size_t m_count = 0; // required
		//double m_max[16] = { 0 };
		//double m_min[16] = { 0 };
		bool m_normalized = false;
		std::string m_type; // required
		std::string m_name;
		// sparse
		// extensions
		// extras

		gltf_accessor(const nlohmann::json& data);
	};
}

#endif