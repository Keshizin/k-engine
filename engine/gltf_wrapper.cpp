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

#include <cassert>

/*
	kengine::gltf_render class - member class definition
*/

int kengine::gltf_render::getRootNode(int scene, int node)
{
	if (m_scenes.empty())
		return -1;

	if (scene == -1 && m_defaultScene == -1)
		return -1;

	int currentScene = scene != -1 ? scene : m_defaultScene;
	int currentRootNode = node != -1 ? node : m_defaultRootNode;
	return m_scenes[currentScene].m_nodes[currentRootNode];
}

void kengine::gltf_render::traverseScene(size_t node, matrix<GLfloat>& parentTransform)
{
	if (m_scenes.empty())
		return;

	gltf_node& n = m_nodes[node];
	//K_LOG_OUTPUT_RAW("> traverseScene | node[" << node << "]: " << n.m_name);

	matrix<GLfloat> localTransform;

	if (n.m_isTRS) {
		matrix<GLfloat> t = kengine::translate(n.m_translation[0], n.m_translation[1], n.m_translation[2]);
		matrix<GLfloat> r = kengine::quaternion(n.m_rotation[0], n.m_rotation[1], n.m_rotation[2], n.m_rotation[3]);
		matrix<GLfloat> s = kengine::scale(n.m_scale[0], n.m_scale[1], n.m_scale[2]);
		localTransform = t * r * s;
	} else {
		for (int i = 0; i < 16; i++) {
			localTransform[i] = n.m_matrix[i];
		}
	}

	matrix<GLfloat> globalTransform = parentTransform * localTransform;
	// m_globalTransforms.push_back(globalTransform);

	if (n.m_mesh != -1) {
		m_shader->useProgram();
		m_shader->setUniform("modelTransform", false, globalTransform.data());
		m_meshes[n.m_mesh]->draw();
	}

	for (size_t i = 0; i < n.m_children.size(); i++) {
		traverseScene(n.m_children[i], globalTransform);
	}
}

void kengine::gltf_render::calculateGlobalTransforms(size_t node, kengine::matrix<GLfloat>& parentTransform)
{
	gltf_node& n = m_nodes[node];

	matrix<GLfloat> localTransform;

	if (n.m_isTRS) {
		matrix<GLfloat> t = kengine::translate(n.m_translation[0], n.m_translation[1], n.m_translation[2]);
		matrix<GLfloat> r = kengine::quaternion(n.m_rotation[0], n.m_rotation[1], n.m_rotation[2], n.m_rotation[3]);
		matrix<GLfloat> s = kengine::scale(n.m_scale[0], n.m_scale[1], n.m_scale[2]);
		localTransform = t * r * s;
	}
	else {
		for (int i = 0; i < 16; i++) {
			localTransform[i] = n.m_matrix[i];
		}
	}

	matrix<GLfloat> globalTransform = parentTransform * localTransform;
	n.globalTransform = globalTransform;

	for (size_t i = 0; i < n.m_children.size(); i++) {
		calculateGlobalTransforms(n.m_children[i], globalTransform);
	}
}

#include <set>
#include <algorithm>

void kengine::gltf_render::updateAnimation(float deltaTime)
{
	if (m_currentAnimation == -1)
		return;

	auto& animationData = m_animationsData[m_currentAnimation];
	
	// iterate for all channels
	for (auto& channel : m_animations[m_currentAnimation].channels) {
		auto& keyframe = animationData.m_keyframes[channel.sampler];

		keyframe.m_animationTime += deltaTime;
		
		if (keyframe.m_animationTime > keyframe.m_durationTime) {
			keyframe.m_animationTime = std::fmod(keyframe.m_animationTime, keyframe.m_durationTime);
		}

		// previousTime = The largest element from the times accessor that is smaller than the currentTime
		size_t t0Index = 0;

		for (size_t i = 0; i < keyframe.m_times.size(); i++) {
			if (keyframe.m_animationTime > keyframe.m_times[i])
				t0Index = i;
		}

		// nextTime = The smallest element from the times accessor that is larger than the currentTime
		size_t t1Index = keyframe.m_times.size() - 1;

		for (size_t i = keyframe.m_times.size() - 1; i > 0; i--) {
			if (keyframe.m_animationTime < keyframe.m_times[i])
				t1Index = i;
		}

		float interpolationValue = (keyframe.m_animationTime - keyframe.m_times[t0Index]) / (keyframe.m_times[t1Index] - keyframe.m_times[t0Index]);

		kengine::vec3<float> t0Transform = keyframe.m_outputs[t0Index];
		kengine::vec3<float> t1Transform = keyframe.m_outputs[t1Index];

		auto& node = m_nodes[channel.target.node];

		if (channel.target.path == "translation") {
			vec3<float> translation = kengine::lerp<vec3<float>, float>(t0Transform, t1Transform, interpolationValue);

			node.m_translation[0] = translation.x;
			node.m_translation[1] = translation.y;
			node.m_translation[2] = translation.z;
		}
		else if (channel.target.path == "rotation") {
			vec3<float> rotation = kengine::slerp(t0Transform, t1Transform, interpolationValue);
			node.m_rotation[0] = rotation.x;
			node.m_rotation[1] = rotation.y;
			node.m_rotation[2] = rotation.z;
			node.m_rotation[3] = rotation.w;
		}
		else if (channel.target.path == "scale") {
			vec3<float> scale = kengine::lerp<vec3<float>, float>(t0Transform, t1Transform, interpolationValue);

			node.m_scale[0] = scale.x;
			node.m_scale[1] = scale.y;
			node.m_scale[2] = scale.z;
		}
	}

	// update globalTransform of all nodes
	for (auto& scene : m_scenes) {
		for (auto& node : scene.m_nodes) {
			calculateGlobalTransforms(node, kengine::matrix<GLfloat>(1.0f));
		}
	}

	for (auto& skin : m_skins) {
		GLfloat* globalJointTransforms = new GLfloat[skin.m_joints.size() * 16];
		for (size_t i = 0; i < skin.m_joints.size(); i++) {
			auto& node = m_nodes[skin.m_joints[i]];
			node.globalTransform.copy(globalJointTransforms + (i * 16));
		}
		m_shader->useProgram();
		m_shader->setUniform("globalJointMatrices[0]", skin.m_joints.size(), false, globalJointTransforms);
		delete[] globalJointTransforms;
	}
}

void kengine::gltf_render::clear()
{
	m_defaultScene = -1;
	m_defaultRootNode = 0;
	m_currentAnimation = -1;
	m_shader = nullptr;
	m_gpuBuffers.reset();
	m_scenes.clear();
	m_nodes.clear();
	m_animations.clear();
	m_animationsData.clear();
	m_skins.clear();
	m_meshes.clear();
}

/*
	This function loads an GLTF file (https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html)
*/
std::unique_ptr<kengine::gltf_render> kengine::loadGLTF(const std::string& path, const std::string& filename, std::unordered_map<std::string, GLuint>& shaderLocations, GLSLprogram* shader)
{
	K_LOG_OUTPUT_RAW("> Loading GLTF file: " + path + filename);

	/*
		loading glTF JSON file
	*/
	std::fstream gltfJsonFile(path + filename);
	nlohmann::json data = nlohmann::json::parse(gltfJsonFile);

	/*
		parsing glTF asset (mandatory)
	*/
	kengine::gltf_asset asset(data["asset"]);

	if (!asset.checkVersion(GLTF_MIN_VERSION)) {
		assert(false && "Failed to load GLTF file: version file is not supported");
		return nullptr;
	}

	/*
	*/
	std::unique_ptr<kengine::gltf_render> gltfRender = std::make_unique<kengine::gltf_render>();

	/*
		parsing glTF binary data
	*/
	std::vector<std::unique_ptr<gltf_buffer>> buffers;
	std::unique_ptr<kengine::gpu_buffer> gpuBuffers;
	
	if (data.contains("buffers")) {
		K_LOG_OUTPUT_RAW("> Reading " << data["buffers"].size() << " buffer(s)");
		gpuBuffers = std::make_unique<kengine::gpu_buffer>(data["buffers"].size());
		
		/*
			Nota: tem alguns buffers que não precisam ser enviados para a GPU.
			Necessário uma lógica para verificar qual buffer deve ser enviado para a GPU.
			Talvez gpuBuffer não deve ser criado aqui.
		*/
		for (size_t i = 0; i < data["buffers"].size(); i++) {
			std::unique_ptr<kengine::gltf_buffer> buffer = std::make_unique<kengine::gltf_buffer>(data["buffers"][i], path);
			
			if (!buffer->empty()) {
				K_LOG_OUTPUT_RAW("\t- buffer[" << i << "]: " << buffer->m_uri << " (" << buffer->m_byteLength << " bytes).");
				gpuBuffers->load(i, GL_ARRAY_BUFFER, buffer->m_byteLength, buffer->m_data);
				buffers.push_back(std::move(buffer));
			}
		}
	}

	/*
		parsing glTF nodes
	*/
	if (data.contains("nodes")) {
		K_LOG_OUTPUT_RAW("> reading " << data["nodes"].size() << " node(s)");

		for (auto& node : data["nodes"]) {
			kengine::gltf_node n(node);
			K_LOG_OUTPUT_RAW("\t- node: " << n.m_name);
			gltfRender->pushNode(n);
		}
	}

	/*
		parsing the default scene
	*/
	if (data.contains("scene")) {
		K_LOG_OUTPUT_RAW("> Reading default scene: " << data.value("scene", -1));
		gltfRender->setDefaultScene(data.value("scene", -1));
	}

	/*
		parsing glTF scenes
	*/
	if (data.contains("scenes")) {
		K_LOG_OUTPUT_RAW("> Reading " << data["scenes"].size() << " scene(s)");

		for (auto& scene : data["scenes"]) {
			gltf_scene s(scene);
			gltfRender->pushScene(s);

			for (auto& node : s.m_nodes) {
				gltfRender->calculateGlobalTransforms(node, kengine::matrix<GLfloat>(1.0f));
			}
		}
	}

	/*
		parsing glTF animations
	*/
	if (data.contains("animations")) {
		K_LOG_OUTPUT_RAW("> reading " << data["animations"].size() << " animation(s)");
		
		gltfRender->setCurrentAnimation(0);
		
		for (auto& animation : data["animations"]) {
			gltf_animation anim(animation);
			K_LOG_OUTPUT_RAW("\t- animation: " << anim.name);

			kengine::animation animationData;

			// reading sampler data
			K_LOG_OUTPUT_RAW("> reading " << anim.samplers.size() << " sampler(s) for animation " << anim.name);
			for (auto& sampler : anim.samplers) {
				kengine::keyframe keyframeData;

				// input
				kengine::gltf_accessor inputAccessor(data["accessors"][sampler.input]);
				kengine::gltf_bufferView inputBufferView(data["bufferViews"][inputAccessor.m_bufferView]);

				for (size_t i = 0; i < inputAccessor.m_count; i++) {
					for (size_t j = 0; j < kengine::getGLTypeComponents(inputAccessor.m_type); j++) {
						float data;
						size_t byteOffset = inputAccessor.m_byteOffset + inputBufferView.m_byteOffset;
						size_t stride = inputBufferView.m_byteStride != 0 ? inputBufferView.m_byteStride : kengine::getGLTypeComponents(inputAccessor.m_type) * sizeof(float);
						std::memcpy(&data, buffers[inputBufferView.m_buffer]->m_data + byteOffset + j * sizeof(float) + (i * stride), sizeof(float));
						keyframeData.m_times.push_back(data);
						//std::cout << "\t- times[" << i << "]: " << data << std::endl;
					}
				}

				if (inputAccessor.m_max.size()) {
					keyframeData.m_durationTime = inputAccessor.m_max[0];
				} else {
					float max = 0.0f;
					
					for (size_t i = 0; i < keyframeData.m_times.size(); i++) {	
						if (keyframeData.m_times[i] > max)
							max = keyframeData.m_times[i];
					}

					keyframeData.m_durationTime = max;
				}

				// ouput
				kengine::gltf_accessor outputAccessor(data["accessors"][sampler.output]);
				kengine::gltf_bufferView outputBufferView(data["bufferViews"][outputAccessor.m_bufferView]);

				if (outputAccessor.m_componentType != 0x1406)
					assert(false && "(!) component type in gltf_accessor for gltf_sampler.output is diferent of GLfloat");

				for (size_t i = 0; i < outputAccessor.m_count; i++) {
					kengine::vec3<float> output;

					for (size_t j = 0; j < kengine::getGLTypeComponents(outputAccessor.m_type); j++) {
						float data;
						size_t byteOffset = outputAccessor.m_byteOffset + outputBufferView.m_byteOffset;
						size_t stride = outputBufferView.m_byteStride != 0 ? outputBufferView.m_byteStride : kengine::getGLTypeComponents(outputAccessor.m_type) * sizeof(float);
						std::memcpy(&data, buffers[outputBufferView.m_buffer]->m_data + byteOffset + j * sizeof(float) + (i * stride), sizeof(float));
						output.set(j, data);
					}
					
					//std::cout << "\t- output[" << i << "]: " << output.x << ", " << output.y << ", " << output.z << ", " << output.w << std::endl;
					keyframeData.m_outputs.push_back(output);
				}

				animationData.m_keyframes.push_back(keyframeData);
			}

			gltfRender->pushAnimation(anim);
			gltfRender->pushAnimationData(animationData);
		}
	}

	/*
		parsing skins
	*/
	if (data.contains("skins")) {
		K_LOG_OUTPUT_RAW("> reading " << data["skins"].size() << " skin(s).");

		if (data["skins"].size() > 0) {
			shader->useProgram();
			shader->setUniform("hasSkinAnimation", 1);
		}

		for (auto& skin : data["skins"]) {
			kengine::gltf_skin s(skin);

			if (s.m_inverseBindMatrices != -1) {
				kengine::gltf_accessor ibmAccessor(data["accessors"][s.m_inverseBindMatrices]);
				kengine::gltf_bufferView ibmBufferView(data["bufferViews"][ibmAccessor.m_bufferView]);

				size_t byteOffset = ibmAccessor.m_byteOffset + ibmBufferView.m_byteOffset;
				size_t stride = ibmBufferView.m_byteStride != 0 ? ibmBufferView.m_byteStride : kengine::getGLTypeComponents(ibmAccessor.m_type) * sizeof(float);

				GLfloat* globalJointTransforms = new GLfloat[ibmAccessor.m_count * 16];

				for (size_t i = 0; i < ibmAccessor.m_count; i++) {

					kengine::matrix<GLfloat> jointMatrix;

					for (size_t j = 0; j < 16; j++) {
						GLfloat value = 0.0f;
						std::memcpy(&value, buffers[ibmBufferView.m_buffer]->m_data + byteOffset + j * sizeof(float) + (i * stride), sizeof(float));
						jointMatrix[j] = value;
					}

					std::cout << "> inverse bind matrix:\n" << jointMatrix.dump() << std::endl;

					auto& node = gltfRender->getNode(s.m_joints[i]);
					jointMatrix = node.globalTransform * jointMatrix;
					std::cout << "> global * inverse bind matrix:\n" << jointMatrix.dump() << std::endl;
					node.globalTransform.copy(globalJointTransforms + (i * 16));

				}

				shader->useProgram();
				shader->setUniform("jointMatrices[0]", ibmAccessor.m_count, false, buffers[ibmBufferView.m_buffer]->m_data + byteOffset);
				shader->setUniform("globalJointMatrices[0]", ibmAccessor.m_count, false, globalJointTransforms);

				delete[] globalJointTransforms;
			}

			gltfRender->pushSkin(s);
		}
	}

	/*
		parsing meshes
	*/
	if (data.contains("meshes")) {
		K_LOG_OUTPUT_RAW("> reading " << data["meshes"].size() << " mesh(es).");

		for(auto& mesh : data["meshes"]) {
			K_LOG_OUTPUT_RAW("> reading " << mesh["primitives"].size() << " primitive(s).");
			
			std::unique_ptr<kengine::mesh_render> meshRender = std::make_unique<kengine::mesh_render>();
			meshRender->setName(mesh.value("name", ""));

			for (auto& primitive : mesh["primitives"]) {
				K_LOG_OUTPUT_RAW("> reading primitive for mesh " << meshRender->getName());

				std::unique_ptr<kengine::primitive_render> prim = std::make_unique<kengine::primitive_render>();

				/*
					parsing primitive attributes
				*/
				for (auto it = primitive["attributes"].begin(); it != primitive["attributes"].end(); ++it) {
					K_LOG_OUTPUT_RAW("\t- " << it.key() << ": " << it.value());

					kengine::gltf_accessor accessor(data["accessors"][static_cast<size_t>(it.value())]);
					kengine::gltf_bufferView bufferView(data["bufferViews"][accessor.m_bufferView]);

					GLenum target = bufferView.m_target != 0 ? bufferView.m_target : GL_ARRAY_BUFFER;

					GLsizei stride = bufferView.m_byteStride != 0 ?
						bufferView.m_byteStride :
						kengine::getGLTypeComponents(accessor.m_type) * kengine::getGLTypeSize(accessor.m_componentType);

					if (gpuBuffers == nullptr) {
						assert(false && "Failed to load GLTF file - gpu buffer is nullptr!");
					}

					if (shaderLocations.find(it.key()) == shaderLocations.end()) {
						assert(false && "Attribute key not found!");
					}

					prim->loadAttributeInfo(
						shaderLocations[it.key()],
						(*gpuBuffers)[bufferView.m_buffer],
						target,
						kengine::getGLTypeComponents(accessor.m_type),
						accessor.m_componentType,
						accessor.m_normalized,
						stride,
						(void*)(accessor.m_byteOffset + bufferView.m_byteOffset));

					// debug
					//size_t byteOffset = accessor.m_byteOffset + bufferView.m_byteOffset;
					//for (size_t i = 0; i < accessor.m_count; i++) {

					//	std::cout << "\t\t- " << accessor.m_type << ": ";

					//	for (size_t j = 0; j < kengine::getGLTypeComponents(accessor.m_type); j++) {

					//		size_t offset = byteOffset + (i * stride) + (j * kengine::getGLTypeSize(accessor.m_componentType));

					//		switch (accessor.m_componentType) {
					//		case GL_UNSIGNED_BYTE:
					//		{
					//			GLubyte value = 0;
					//			std::memcpy(&value, buffers[bufferView.m_buffer]->m_data + offset, sizeof(GLubyte));
					//			std::cout << value << " ";
					//			break;
					//		}

					//		case GL_UNSIGNED_SHORT:
					//		{
					//			GLushort value = 0;
					//			std::memcpy(&value, buffers[bufferView.m_buffer]->m_data + offset, sizeof(GLushort));
					//			std::cout << value << " ";
					//			break;
					//		}

					//		case GL_FLOAT:
					//		{
					//			GLfloat value = 0;
					//			std::memcpy(&value, buffers[bufferView.m_buffer]->m_data + offset, sizeof(GLfloat));
					//			std::cout << value << " ";
					//			break;
					//		}
					//		}
					//	}

					//	std::cout << std::endl;
					//}

					if (it.key() == "POSITION")
						prim->setCount(accessor.m_count);
				}

				/*
					parsing primitive indices
				*/
				if (primitive.contains("indices")) {

					kengine::gltf_accessor accessor(data["accessors"][primitive["indices"].get<size_t>()]);
					kengine::gltf_bufferView bufferView(data["bufferViews"][accessor.m_bufferView]);

					GLenum target = bufferView.m_target != 0 ? bufferView.m_target : GL_ELEMENT_ARRAY_BUFFER;
					prim->bindBuffer(target, (*gpuBuffers)[bufferView.m_buffer]);
					prim->setElements(accessor.m_count);
					prim->setIndexType(accessor.m_componentType);
					size_t byteOffset = accessor.m_byteOffset + bufferView.m_byteOffset;
					prim->setElementsOffset(byteOffset);

					// for debug
					//GLsizei stride = bufferView.m_byteStride != 0 ?
					//	bufferView.m_byteStride :
					//	kengine::getGLTypeComponents(accessor.m_type) * kengine::getGLTypeSize(accessor.m_componentType);
					//for (size_t i = 0; i < accessor.m_count; i++) {

					//	std::cout << "\t\t- " << accessor.m_type << ": ";

					//	for (size_t j = 0; j < kengine::getGLTypeComponents(accessor.m_type); j++) {

					//		size_t offset = byteOffset + (i * stride) + (j * kengine::getGLTypeSize(accessor.m_componentType));

					//		switch (accessor.m_componentType) {
					//		case GL_UNSIGNED_BYTE:
					//		{
					//			GLubyte value = 0;
					//			std::memcpy(&value, buffers[bufferView.m_buffer]->m_data + offset, sizeof(GLubyte));
					//			std::cout << value << " ";
					//			break;
					//		}

					//		case GL_UNSIGNED_SHORT:
					//		{
					//			GLushort value = 0;
					//			std::memcpy(&value, buffers[bufferView.m_buffer]->m_data + offset, sizeof(GLushort));
					//			std::cout << value << " ";
					//			break;
					//		}

					//		case GL_FLOAT:
					//		{
					//			GLfloat value = 0;
					//			std::memcpy(&value, buffers[bufferView.m_buffer]->m_data + offset, sizeof(GLfloat));
					//			std::cout << value << " ";
					//			break;
					//		}
					//		}
					//	}

					//	std::cout << std::endl;
					//}
				}

				/*
					parsing primitive material
				*/
				if (primitive.contains("material")) {
				}

				/*
					parsing primitive mode
				*/
				if (primitive.contains("mode")) {
					prim->setMode(primitive["mode"]);
				}

				/*
					parsing primitive morph targets
				*/

				// parsing primitive extensions
				// parsing primitive extras

				meshRender->pushPrimitive(std::move(prim));
			}

			// parsing mesh weights
			// parsing mesh extensions
			// parsing mesh extras

			gltfRender->moveMesh(std::move(meshRender));
		}
	}

	if(gpuBuffers)
		gltfRender->movetGpuBuffers(std::move(gpuBuffers));

	return gltfRender;
}