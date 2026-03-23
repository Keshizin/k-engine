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

#include <gltf_wrapper.hpp>
#include <logger.hpp>

#include <fstream>

/*
	kengine::gltf_render class - member class definition
*/
kengine::gltf_render::gltf_render()
{
}

kengine::gltf_render::~gltf_render()
{
}

kengine::gltf_render::gltf_render(gltf_render&& move) noexcept // move constructor
{
}

kengine::gpu_buffer gpuBuffer;
kengine::primitive_render* prim = nullptr;

/*
	This function loads an GLTF file (https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html)
*/
void kengine::loadGLTF(const std::string& path, const std::string& filename, std::unordered_map<std::string, GLuint>& shaderLocations)
{
	K_LOG_OUTPUT_RAW("> Loading GLTF file: " + path + filename);
	//gltf_render gltfRender;

	/*
		Loading glTF JSON file
	*/
	std::fstream gltfJsonFile(path + filename);
	nlohmann::json data = nlohmann::json::parse(gltfJsonFile);

	/*
		parsing glTF asset
	*/
	kengine::gltf_asset asset(data["asset"]);
	K_LOG_OUTPUT_RAW(asset.dump());

	if (!asset.checkVersion(GLTF_MIN_VERSION)) {
		// return false;
	}

	/*
		parsing gltf binary data
	*/
	std::vector<kengine::gltf_buffer> buffers;

	for (size_t i = 0; i < data["buffers"].size(); i++) {
		gltf_buffer b(data["buffers"][i], path);

		if (!b.empty()) {
			buffers.push_back(std::move(b));
		}
	}

	/*
		Creating one vbo for each bufferView or
		Creating one vbo for each buffer
	*/
	//gltfRender.m_gpuBuffers.createBuffers(data["bufferViews"].size());
	//gltfRender.m_gpuBuffers.createBuffers(data["buffers"].size());
	//kengine::gpu_buffer gpuBuffer;
	gpuBuffer.addBuffers(data["buffers"].size());

	for (size_t i = 0; i < data["buffers"].size(); i++) {
		//gltfRender.m_gpuBuffers.load(i, GL_ARRAY_BUFFER, buffers[i].byteLength, buffers[i].data());
		gpuBuffer.load(i, GL_ARRAY_BUFFER, buffers[i].byteLength, buffers[i].data());
	}

	/*
		parsing gltf mesh
	*/
	for (size_t m = 0; m < data["meshes"].size(); m++) {
		auto mesh = data["meshes"][m];

		for (size_t p = 0; p < mesh["primitives"].size(); p++) {
			auto primitive = mesh["primitives"][p];

	//		kengine::primitive_render prim;

			// loading VBO data
			//for (size_t i = 0; i < gltfRender.m_gpuBuffers.size(); i++) {
			//	kengine::gltf_bufferView bufferView(data["bufferViews"][i]);

			//	GLenum target = bufferView.m_target != 0
			//		? bufferView.m_target
			//		: GL_ARRAY_BUFFER;

			//	glBindBuffer(target, gltfRender.m_gpuBuffers[i]);

			//	glBufferData(
			//		target,
			//		bufferView.m_byteLength,
			//		buffers[bufferView.m_buffer].data() + bufferView.m_byteOffset,
			//		GL_STATIC_DRAW);
			//}

			/*
				parsing attributes
			*/
			for (auto it = primitive["attributes"].begin(); it != primitive["attributes"].end(); ++it) {
				K_LOG_OUTPUT_RAW("\t- " << it.key() << ": " << it.value());

				kengine::gltf_accessor accessor(data["accessors"][static_cast<size_t>(it.value())]);
				kengine::gltf_bufferView bufferView(data["bufferViews"][accessor.m_bufferView]);

				GLenum target = bufferView.m_target != 0 ? bufferView.m_target : GL_ARRAY_BUFFER;
				GLuint location = shaderLocations[it.key()];

				GLsizei stride = bufferView.m_byteStride != 0 ?
					bufferView.m_byteStride : kengine::getGLDataTypeSize(accessor.m_type) * kengine::getGLComponentTypeSize(accessor.m_componentType);

				prim->loadAttributeInfo(location, gpuBuffer[bufferView.m_buffer], target, kengine::getGLDataTypeSize(accessor.m_type), accessor.m_componentType, GL_FALSE, stride, (void*)accessor.m_byteOffset);
			}

			/*
				parsing indices
			*/
			if (primitive.contains("indices")) {
				kengine::gltf_accessor accessor(data["accessors"][primitive["indices"].get<size_t>()]);
				kengine::gltf_bufferView bufferView(data["bufferViews"][accessor.m_bufferView]);

				GLenum target = bufferView.m_target != 0 ? bufferView.m_target : GL_ELEMENT_ARRAY_BUFFER;
				glBindBuffer(target, gpuBuffer[accessor.m_bufferView]);
			}
		}

		// weights
		// name
		// extensions
		// extras
	}
}


/*
	kengine::gltf_asset class - class members definition
*/

kengine::gltf_asset::gltf_asset(const nlohmann::json& data) {
	version = data["version"].get<std::string>(); // required 

	if (data.contains("copyright"))
		copyright = data["copyright"].get<std::string>();

	if (data.contains("generator"))
		generator = data["generator"].get<std::string>();

	if (data.contains("minVersion"))
		minVersion = data["minVersion"].get<std::string>();
}

std::string kengine::gltf_asset::dump()
{
	return "copyright: " + copyright + "\ngenerator: " + generator + "\nversion: " + version + "\nminVersion: " + minVersion + "\n";
}

/*
	gltf_buffer class - class members definition
*/

kengine::gltf_buffer::gltf_buffer(const nlohmann::json& _data, const std::string& _path) {
	byteLength = _data["byteLength"].get<size_t>(); // required

	if (_data.contains("uri")) {
		uri = _path + _data["uri"].get<std::string>();

		std::ifstream filestream(uri, std::ios::in | std::ios::binary);

		if (filestream) {
			filestream.seekg(0, std::ios::end);
			std::streamoff fileSize = filestream.tellg();

			if (static_cast<size_t>(fileSize) >= byteLength) {
				filestream.seekg(0, std::ios::beg);
				m_data = new unsigned char[fileSize];
				filestream.read(reinterpret_cast<char*>(m_data), static_cast<std::streamsize>(fileSize));
			}
		}
	}

	if (_data.contains("name"))
		name = _data["name"].get<std::string>();

	if (m_data == nullptr) {
		clear();
	}
}

kengine::gltf_buffer::~gltf_buffer() {
	clear();
}

kengine::gltf_buffer::gltf_buffer(const gltf_buffer& copy) {
	byteLength = copy.byteLength;

	if (byteLength > 0) {
		m_data = new unsigned char[byteLength];
		for (size_t i = 0; i < byteLength; i++) {
			m_data[i] = copy.m_data[i];
		}
	}

	uri = copy.uri;
	name = copy.name;

	if (m_data == nullptr) {
		clear();
	}
}

kengine::gltf_buffer::gltf_buffer(gltf_buffer&& _move) noexcept {
	byteLength = _move.byteLength;
	_move.byteLength = 0;

	m_data = _move.m_data;
	_move.m_data = nullptr;

	uri = std::move(_move.uri);
	name = std::move(_move.name);

	if (m_data == nullptr) {
		clear();
	}
}

kengine::gltf_buffer& kengine::gltf_buffer::operator=(const gltf_buffer& right) { // copy assignment
	if (right.byteLength == 0)
		return *this;

	byteLength = right.byteLength;

	delete[] m_data;
	m_data = new unsigned char[byteLength];

	for (size_t i = 0; i < byteLength; i++) {
		m_data[i] = right.m_data[i];
	}

	uri = right.uri;
	name = right.name;

	return *this;
}

kengine::gltf_buffer& kengine::gltf_buffer::operator=(gltf_buffer&& right) noexcept { // move assigment
	if (right.byteLength == 0)
		return *this;

	byteLength = right.byteLength;
	right.byteLength = 0;

	delete[] m_data;
	m_data = right.m_data;
	right.m_data = nullptr;

	uri = std::move(right.uri);
	name = std::move(right.name);

	return *this;
}

void kengine::gltf_buffer::clear()
{
	delete[] m_data;
	m_data = nullptr;
	byteLength = 0;
	name.clear();
	uri.clear();
}

/*
	kengine::gltf_bufferView class - member class definition
*/

kengine::gltf_bufferView::gltf_bufferView(const nlohmann::json& data) {
	m_buffer = data["buffer"];
	m_byteOffset = data.contains("byteOffset") ? data["byteOffset"] : 0;
	m_byteLength = data["byteLength"];

	if (data.contains("byteStride")) {
		m_byteStride = data["byteStride"];
	}

	if (data.contains("target")) {
		m_target = data["target"];
	}

	if (data.contains("name")) {
		m_name = data["name"];
	}

	// extensions
	// extras
}

/*
	kengine::gltf_accessor class - member class definition
*/

kengine::gltf_accessor::gltf_accessor(const nlohmann::json& data) {
	if (data.contains("bufferView")) {
		m_bufferView = data["bufferView"].get<size_t>();
	}

	m_byteOffset = (data.contains("byteOffset") ? data["byteOffset"].get<size_t>() : 0);
	m_componentType = data["componentType"].get<size_t>();
	m_normalized = (data.contains("normalized") ? data["normalized"].get<bool>() : false);
	m_count = data["count"].get<size_t>();
	m_type = data["type"].get<std::string>();

	//auto max = (accessor.contains("max") ? accessor["max"].get<size_t>() : 0);
	//auto min = (accessor.contains("min") ? accessor["min"].get<size_t>() : 0);

	//if (data.contains("sparse")) {
	//}

	if (data.contains("name")) {
		m_name = data["name"];
	}

	// extensions
	// extras
}