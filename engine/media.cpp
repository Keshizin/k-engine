/*
	K-Engine Media Library
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

#include <media.hpp>
#include <gl_wrapper.hpp>
#include <logger.hpp>

#include <string>
#include <fstream>



#include <nlohmann/json.hpp>

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#pragma warning(pop)

/*
*/
bool kengine::loadCameraConfig(camera* cam) {
	nlohmann::json configJson;
	
	std::string filepath = std::string(K_BIN_PATH) + "/kengine_config.json";
	
	if (std::filesystem::exists(filepath)) {
		std::ifstream configFile(filepath);
		
		try {
			configFile >> configJson;

			if (configJson.contains("projection_type")) {
				cam->setProjectionType(configJson["projection_type"].get<PROJECTION_TYPE>());
			}

			if (configJson.contains("projection_info")) {
				kengine::projection_info<float> projectionInfo;
				projectionInfo.left = configJson["projection_info"]["left"].get<float>();
				projectionInfo.right = configJson["projection_info"]["right"].get<float>();
				projectionInfo.bottom = configJson["projection_info"]["bottom"].get<float>();
				projectionInfo.top = configJson["projection_info"]["top"].get<float>();
				projectionInfo.zNear = configJson["projection_info"]["zNear"].get<float>();
				projectionInfo.zFar = configJson["projection_info"]["zFar"].get<float>();
				projectionInfo.fovy = configJson["projection_info"]["fovy"].get<float>();
				projectionInfo.width = configJson["projection_info"]["width"].get<float>();
				projectionInfo.height = configJson["projection_info"]["height"].get<float>();

				cam->setProjectionInfo(projectionInfo);
			}

			if (configJson.contains("camera_info")) {
				kengine::vec3<float> eye;
				eye.x = configJson["camera_info"]["eye_x"].get<float>();
				eye.y = configJson["camera_info"]["eye_y"].get<float>();
				eye.z = configJson["camera_info"]["eye_z"].get<float>();

				kengine::vec3<float> center;
				center.x = configJson["camera_info"]["center_x"].get<float>();
				center.y = configJson["camera_info"]["center_y"].get<float>();
				center.z = configJson["camera_info"]["center_z"].get<float>();

				kengine::vec3<float> up;
				up.x = configJson["camera_info"]["up_x"].get<float>();
				up.y = configJson["camera_info"]["up_y"].get<float>();
				up.z = configJson["camera_info"]["up_z"].get<float>();

				cam->setCamera(eye, center, up);
			}

			return true;
		}

		catch (const nlohmann::json::parse_error& e) {
			K_LOG_OUTPUT_RAW(e.what());
			return false;
		}
	}

	return false;
}

/*
*/
bool kengine::saveCameraConfig(const camera* cam) {
	nlohmann::json configJson;

	auto projectionInfo = cam->getProjectionInfo();
	auto eye = cam->getEye();
	auto center = cam->getCenter();
	auto up = cam->getUp();

	configJson = {
		{"projection_type", cam->getProjectionType() },

		{"projection_info", {
			{"left", projectionInfo.left},
			{"right", projectionInfo.right},
			{"bottom", projectionInfo.bottom},
			{"top", projectionInfo.top},
			{"zNear", projectionInfo.zNear},
			{"zFar", projectionInfo.zFar},
			{"fovy", projectionInfo.fovy},
			{"width", projectionInfo.width},
			{"height", projectionInfo.height}
		}},

		{"camera_info", {
			{"eye_x", eye.x},
			{"eye_y", eye.y},
			{"eye_z", eye.z},
			{"center_x", center.x},
			{"center_y", center.y},
			{"center_z", center.z},
			{"up_x", up.x},
			{"up_y", up.y},
			{"up_z", up.z}
		}}
	};

	std::ofstream newFile(std::string(K_BIN_PATH) + "/kengine_config.json");

	if (newFile.is_open()) {
		newFile << configJson.dump(4);
		return true;
	}

	return false;
}

/*
*/
kengine::img::img(const std::string& filename)
{
	load(filename);
}

kengine::img::~img()
{
	clear();

	if(m_imageData)
		stbi_image_free(m_imageData);
}

void kengine::img::load(const std::string& filename)
{
	clear();

	stbi_set_flip_vertically_on_load(true);
	m_imageData = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 0);

	if (m_imageData) {
		switch (m_channels) {
		case 1:
			m_internalFormat = GL_R8;
			m_format = GL_RED;
			break;

		case 3:
			m_internalFormat = GL_RGB8;
			m_format = GL_RGB;
			break;

		case 4:
			m_internalFormat = GL_RGBA8;
			m_format = GL_RGBA;
			break;
		}
	}
	else {
		K_LOG_OUTPUT_RAW(stbi_failure_reason());
	}
}

void kengine::img::clear()
{
	m_width = 0;
	m_height = 0;
	m_channels = 0;
	m_internalFormat = 0;
	m_format = 0;

	if (m_imageData) {
		stbi_image_free(m_imageData);
		m_imageData = nullptr;
	}
}