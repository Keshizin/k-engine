/*
	K-Engine Logger
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

#ifndef K_ENGINE_LOGGER_HPP
#define K_ENGINE_LOGGER_HPP

/*
	[ ... under construction ... ]
*/

#ifdef K_ENGINE_DEBUG
#if defined(__ANDROID__)
#include <sstream>
#define K_LOG_OUTPUT_RAW(message) { std::stringstream msg; msg << message; K_LOG_OUTPUT(msg.str().c_str()); }
inline void K_LOG_OUTPUT(const char* message) {
	__android_log_print(ANDROID_LOG_INFO, MODULE_NAME, "%s", message);
}
#else
#include <iostream>
#define K_LOG_OUTPUT_RAW(message) std::cout << message << std::endl;
#endif
#else
#define K_LOG_OUTPUT_RAW(message)
#endif

#endif