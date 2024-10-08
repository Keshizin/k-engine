/*
	K-Engine Version
	This file is part of the K-Engine.

	Copyright (C) 2020-2024 Fabio Takeshi Ishikawa

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

/*
	This project uses Semantic Versioning (https://semver.org/) and automatic increment version

		- MAJOR.MINOR.PATCH
		- commit hash
		- build-type.build-increment.build date
		- platform

			M.N.P-hash-[release or debug or undefined]-[platform]

	Notes:
		- The build types are [Release] or [Debug]
		- Platforms are [windows], [linux], and [android]
*/

#define K_ENGINE_VERSION_MAJOR 0 // version for incompatible API changes
#define K_ENGINE_VERSION_MINOR 0 // version for backward compatible changes
#define K_ENGINE_VERSION_PATCH 0 // version for backward compatible bug fixes

// (!) the lines below are automatically updated by external building tool
#define K_ENGINE_GIT_HASHCODE 0
#define K_ENGINE_BUILD_TYPE 1
#define K_ENGINE_VERSION_BUILD_INCREMENT 0
#define K_ENGINE_BUILD_DATE 0
#define K_ENGINE_BUILD_PLATFORM 2

/*
	OpenGL version
*/
#define KENGINE_OPENGL_MAJOR_VERSION 4
#define KENGINE_OPENGL_MINOR_VERSION 5

#include <string>

namespace kengine
{
	enum BUILD_TYPE {
		K_ENGINE_BUILD_UNKNOW_TYPE, // this type is used when the version header was not udpated from external build tool
		K_ENGINE_BUILD_RELEASE,
		K_ENGINE_BUILD_DEBUG,
	};

	enum BUILD_PLATFORM {
		K_ENGINE_PLATFORM_UNKNOW_TYPE, // this type is used when the version header was not udpated from external build tool
		K_ENGINE_PLATFORM_WINDOWS,
		K_ENGINE_PLATFORM_LINUX,
		K_ENGINE_PLATFORM_ANDROID
	};

	inline std::string getBuildType(int type) {
		switch (type)
		{
		case BUILD_TYPE::K_ENGINE_BUILD_RELEASE:
			return "release";
		case BUILD_TYPE::K_ENGINE_BUILD_DEBUG:
			return "debug";
		default:
			return "not-specified";
		}
	}

	inline std::string getBuildPlatform(int platform) {
		switch (platform)
		{
		case BUILD_PLATFORM::K_ENGINE_PLATFORM_WINDOWS:
			return "ms-windows";
		case BUILD_PLATFORM::K_ENGINE_PLATFORM_LINUX:
			return "linux";
		case BUILD_PLATFORM::K_ENGINE_PLATFORM_ANDROID:
			return "android";
		default:
			return "not-specified";
		}
	}

	inline std::string version() {
		return
			std::to_string(K_ENGINE_VERSION_MAJOR) + "." + std::to_string(K_ENGINE_VERSION_MINOR) + "." + std::to_string(K_ENGINE_VERSION_PATCH) + "-" +
			std::to_string(K_ENGINE_GIT_HASHCODE) + "-" +
			getBuildType(K_ENGINE_BUILD_TYPE) + "." + std::to_string(K_ENGINE_VERSION_BUILD_INCREMENT) + "." + std::to_string(K_ENGINE_BUILD_DATE) + "-" +
			getBuildPlatform(K_ENGINE_BUILD_PLATFORM);
	}
}