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
#include <animation.hpp>

#include <string>
#include <fstream>

#include <nlohmann/json.hpp>

/*
	glTF https://www.khronos.org/gltf/#gltf-spec

	Tested models in https://github.com/KhronosGroup/glTF-Sample-Models/tree/main/2.0:
		- Triangle Without Indices [ok]
		- Triangle [ok]
		- Animated Triangle [ok]
		- Simple Skin [ok]
		- Box [ok]
*/
#define GLTF_MIN_VERSION 20 // 2.0

namespace kengine
{
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

		explicit gltf_asset(const nlohmann::json& data) {
			version = data.value("version", ""); // required
			copyright = data.value("copyright", "");
			generator = data.value("generator", "");
			minVersion = data.value("minVersion", "");
		}

		bool checkVersion(int version) const {
			// check first minVersion property
			// if no minVersion, check version property
			return true;
		}

		std::string dump() const {
			return "copyright: " + copyright + "\ngenerator: " + generator + "\nversion: " + version + "\nminVersion: " + minVersion + "\n";
		}
	};

	/*
	*/
	class gltf_buffer
	{
	public:
		std::string m_name;
		std::string m_uri;
		size_t m_byteLength = 0; // required
		unsigned char* m_data = nullptr;
		// extensions
		// extras

		gltf_buffer(const nlohmann::json& data, const std::string& path) {
			m_byteLength = data.value("byteLength", 0); // required
			m_uri = data.value("uri", "");
			m_name = data.value("name", "");

			if (!m_uri.empty()) {
				m_uri = path + m_uri;
				std::ifstream filestream(m_uri, std::ios::in | std::ios::binary);

				if (filestream) {
					filestream.seekg(0, std::ios::end);
					std::streamoff fileSize = filestream.tellg();

					// warn if file size is different of bytelength
					if (static_cast<size_t>(fileSize) >= m_byteLength) {
						m_byteLength = static_cast<size_t>(fileSize);
						filestream.seekg(0, std::ios::beg);
						m_data = new unsigned char[fileSize];
						filestream.read(reinterpret_cast<char*>(m_data), static_cast<std::streamsize>(fileSize));
					}
				}
			}

			// stabilish invariant
			if (m_data == nullptr) {
				clear();
			}
		}

		~gltf_buffer() {
			clear();
		}

		// copy constructor
		gltf_buffer(const gltf_buffer& copy) {
			m_byteLength = copy.m_byteLength;

			if (m_byteLength > 0) {
				m_data = new unsigned char[m_byteLength];
				
				for (size_t i = 0; i < m_byteLength; i++) {
					m_data[i] = copy.m_data[i];
				}
			}

			m_uri = copy.m_uri;
			m_name = copy.m_name;

			if (m_data == nullptr) {
				clear();
			}
		}

		// move constructor
		gltf_buffer(gltf_buffer&& _move) noexcept {
			m_byteLength = _move.m_byteLength;
			_move.m_byteLength = 0;

			m_data = _move.m_data;
			_move.m_data = nullptr;

			m_uri = std::move(_move.m_uri);
			m_name = std::move(_move.m_name);

			if (m_data == nullptr) {
				clear();
			}
		}

		// copy assignment
		gltf_buffer& operator=(const gltf_buffer& right) {
			if (right.m_byteLength == 0) {
				assert(false && "Failed to copy gltf_buffer.");
				clear();
				return *this;
			}

			m_byteLength = right.m_byteLength;

			delete[] m_data;
			m_data = new unsigned char[m_byteLength];

			for (size_t i = 0; i < m_byteLength; i++) {
				m_data[i] = right.m_data[i];
			}

			m_uri = right.m_uri;
			m_name = right.m_name;

			return *this;
		}

		// move assigment
		gltf_buffer& operator=(gltf_buffer&& right) noexcept { // move assigment
			if (right.m_byteLength == 0) {
				assert(false && "Failed to move gltf_buffer.");
				clear();
				return *this;
			}

			m_byteLength = right.m_byteLength;
			right.m_byteLength = 0;

			delete[] m_data;
			m_data = right.m_data;
			right.m_data = nullptr;

			m_uri = std::move(right.m_uri);
			m_name = std::move(right.m_name);

			return *this;
		}

		void clear() {
			delete[] m_data;
			m_data = nullptr;
			m_byteLength = 0;
			m_name.clear();
			m_uri.clear();
		}

		bool empty() const { return m_data == nullptr; }
	};

	/*
	*/
	class gltf_scene {
	public:
		std::vector<size_t> m_nodes;
		std::string m_name;
		// extensions
		// extras

		explicit gltf_scene(const nlohmann::json& data) {
			m_name = data.value("name", "");

			if (data.contains("nodes")) {
				for (auto& n : data["nodes"]) {
					m_nodes.push_back(n.get<size_t>());
				}
			}
		}
	};

	/*
	*/
	class gltf_node {
	public:
		int m_camera = -1;
		int m_skin = -1;
		int m_mesh = -1;
		float m_matrix[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		float m_rotation[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		float m_scale[3] = { 1.0f, 1.0f, 1.0f };
		float m_translation[3] = { 0.0f, 0.0f, 0.0f };
		bool m_isTRS = false;
		kengine::matrix<GLfloat> globalTransform{ 1.0f };
		std::vector<size_t> m_children;
		std::vector<float> m_weights;
		std::string m_name;
		// extensions
		// extras

		explicit gltf_node(const nlohmann::json& data) {
			m_camera = data.value("camera", -1);
			m_skin = data.value("skin", -1);
			m_mesh = data.value("mesh", -1);
			m_name = data.value("name", "");

			if (data.contains("matrix")) {
				for (int i = 0; i < 16; i++) {
					m_matrix[i] = data["matrix"][i].get<float>();
				}
			}

			if (data.contains("rotation")) {
				m_isTRS = true;

				//if necessary, normalize the quaternion
				float x = data["rotation"][0];
				float y = data["rotation"][1];
				float z = data["rotation"][2];
				float w = data["rotation"][3];

				float len = x * x + y * y + z * z + w * w;

				if (std::abs(len - 1.0f) > 1e-6f) {
					float inv = 1.0f / std::sqrt(len);
					x *= inv;
					y *= inv;
					z *= inv;
					w *= inv;
				}

				m_rotation[0] = x;
				m_rotation[1] = y;
				m_rotation[2] = z;
				m_rotation[3] = w;
			}

			if (data.contains("scale")) {
				m_isTRS = true;

				for (int i = 0; i < 3; i++) {
					m_scale[i] = data["scale"][i].get<float>();
				}

				// check is scale is 0, 0, 0
				if (m_scale[0] == 0 && m_scale[1] == 0 && m_scale[2] == 0) {
					assert(false && "gltf_node constructor -> scale in TRS is (0, 0, 0)");
				}
			}

			if (data.contains("translation")) {
				m_isTRS = true;
				for (int i = 0; i < 3; i++) {
					m_translation[i] = data["translation"][i].get<float>();
				}
			}

			if (data.contains("children")) {
				for (auto& c : data["children"]) {
					m_children.push_back(c.get<size_t>());
				}
			}

			if (data.contains("weights")) {
				for (auto& w : data["weights"]) {
					m_weights.push_back(w.get<float>());
				}
			}
		}
	};

	/*
	*/
	class gltf_target
	{
	public:
		int node = -1;
		std::string path; // required
		// extensions
		// extras

		explicit gltf_target(const nlohmann::json& data) {
			node = data.value("node", -1);
			path = data.value("path", "");
		}
	};

	/*
	*/
	class gltf_channel
	{
	public:
		int sampler = -1; // required
		gltf_target target; // required
		// extensions
		// extras

		explicit gltf_channel(const nlohmann::json& data)
			: target(data["target"])
		{
			sampler = data.value("sampler", -1);
		}
	};

	/*
	*/
	class gltf_sampler
	{
	public:
		int input = -1; // required
		int output = -1; // required
		std::string interpolation;
		// extensions
		// extras

		explicit gltf_sampler(const nlohmann::json& data) {
			input = data.value("input", -1);
			output = data.value("output", -1);
			interpolation = data.value("interpolation", "LINEAR");
		}
	};

	/*
	*/
	class gltf_animation
	{
	public:
		std::vector<gltf_channel> channels; // required
		std::vector<gltf_sampler> samplers; // required
		std::string name;
		// extensions
		// extras

		explicit gltf_animation(const nlohmann::json& data) {
			for (auto& s : data["samplers"]) {
				gltf_sampler sampler(s);
				samplers.push_back(sampler);
			}

			for (auto& c : data["channels"]) {
				gltf_channel channel(c);
				channels.push_back(channel);
			}

			name = data.value("name", "");
		}
	};

	/*
	*/
	class gltf_skin
	{
	public:
		int m_inverseBindMatrices = -1;
		int m_skeleton = -1;
		std::vector<size_t> m_joints; // required
		std::string m_name;
		// extensions
		// extras

		explicit gltf_skin(const nlohmann::json& data) {
			m_inverseBindMatrices = data.value("inverseBindMatrices", -1);
			m_skeleton = data.value("skeleton", -1);
			m_name = data.value("name", "");

			for (auto& j : data["joints"]) {
				m_joints.push_back(j);
			}

		}
	};

	/*
	*/
	class gltf_bufferView
	{
	public:
		int m_buffer = -1; // required
		size_t m_byteOffset = 0;
		size_t m_byteLength = 0; // required
		size_t m_byteStride = 0;
		size_t m_target = 0;
		std::string m_name;
		// extensions
		// extras

		explicit gltf_bufferView(const nlohmann::json& data) {
			m_buffer = data.value("buffer", -1);
			m_byteOffset = data.value("byteOffset", 0);
			m_byteLength = data.value("byteLength", 0);
			m_byteStride = data.value("byteStride", 0);
			m_target = data.value("target", 0);
			m_name = data.value("name", "");
		}
	};

	/*
	*/
	class gltf_accessor {
	public:
		int m_bufferView = -1;
		size_t m_byteOffset = 0;
		size_t m_componentType = 0; // required
		size_t m_count = 0; // required
		bool m_normalized = false;
		std::vector<float> m_max;
		std::vector<float> m_min;
		std::string m_type; // required
		std::string m_name;
		// sparse
		// extensions
		// extras

		explicit gltf_accessor(const nlohmann::json& data) {
			m_bufferView = data.value("bufferView", -1);
			m_byteOffset = data.value("byteOffset", 0);
			m_componentType = data.value("componentType", 0);
			m_normalized = data.value("normalized", false);
			m_count = data.value("count", 0);
			m_type = data.value("type", "");
			m_name = data.value("name", "");

			if (data.contains("max")) {
				for (size_t i = 0; i < data["max"].size(); i++) {
					m_max.push_back(data["max"][i].get<float>());
				}
			}

			if (data.contains("min")) {
				for (size_t i = 0; i < data["min"].size(); i++) {
					m_min.push_back(data["min"][i].get<float>());
				}
			}
		}
	};

	/*
	*/
	class gltf_render
	{
	public:
		gltf_render() {}
		~gltf_render() { clear(); }

		gltf_render(const gltf_render& copy) = delete; // copy constructor
		gltf_render(gltf_render&& move) = delete; // move constructor
		gltf_render& operator=(const gltf_render& copy) = delete; // copy assignment
		gltf_render& operator=(gltf_render&& move) = delete; // move assigment

		void updateAnimation(float deltaTime);
		int getRootNode(int scene, int node);
		void traverseScene(size_t node, matrix<GLfloat>& parentTransform);
		void calculateGlobalTransforms(size_t node, matrix<GLfloat>& parentTransform);

		void setDefaultScene(int currentScene) { m_defaultScene = currentScene; }
		void setCurrentAnimation(int animation) { m_currentAnimation = animation; }

		void pushScene(const gltf_scene& scene) { m_scenes.push_back(scene); }
		void pushNode(const gltf_node& node) { m_nodes.push_back(node); }
		void pushAnimation(const gltf_animation& anim) { m_animations.push_back(anim); }
		void pushAnimationData(const animation& animData) { m_animationsData.push_back(animData); }
		void pushSkin(const gltf_skin& skin) { m_skins.push_back(skin); }

		void movetGpuBuffers(std::unique_ptr<gpu_buffer>&& buffer) { m_gpuBuffers = std::move(buffer); }
		void moveMesh(std::unique_ptr<mesh_render>&& mesh) { m_meshes.push_back(std::move(mesh)); }

		gltf_node getNode(size_t index) const { return m_nodes[index]; }

		// ------------------------------------------------------------------------------------------------------------------

		void setShader(GLSLprogram* shader) { m_shader = shader; }

	private:
		int m_defaultScene = -1;
		int m_defaultRootNode = 0;
		int m_currentAnimation = -1;
		std::unique_ptr<gpu_buffer> m_gpuBuffers;
		std::vector<gltf_scene> m_scenes;
		std::vector<gltf_node> m_nodes;
		std::vector<gltf_animation> m_animations;
		std::vector<animation> m_animationsData;
		std::vector<gltf_skin> m_skins;
		std::vector<std::unique_ptr<mesh_render>> m_meshes;

		// ------------------------------------------------------------------------------------------------------------------

		kengine::GLSLprogram* m_shader = nullptr;

		// nodes
		//std::vector<matrix<GLfloat>> m_globalTransforms;
		void clear();
	};


	/*
		This function loads an GLTF file (https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html)
	*/
	std::unique_ptr<kengine::gltf_render> loadGLTF(const std::string& path, const std::string& filename, std::unordered_map<std::string, GLuint>& shaderLocations, GLSLprogram* shader);

	/*
		Auxiliar functions
	*/
}

#endif