/*
	K-Engine Auxiliary Header
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

#ifndef K_ENGINE_AUX_H
#define K_ENGINE_AUX_H


// ----------------------------------------------------------------------------
//  This macro provide std output for debug only
// ----------------------------------------------------------------------------
#ifdef K_DEBUG

	#define K_DEBUG_ERROR "(x) ERROR"
	#define K_DEBUG_WARNING "(!) WARNING"

	#include <iostream>
	#define K_DEBUG_OUTPUT(type, message) std::cout << type << " - " << message << std::endl;

#else
	#define K_DEBUG_OUTPUT(type, message)
#endif


// ----------------------------------------------------------------------------
//  This macro is used for avoid non-used parameter warning
// ----------------------------------------------------------------------------
#define K_UNREFERENCED_PARAMETER(p) reinterpret_cast<void*>(&p)

#endif