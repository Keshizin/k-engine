/*
	K-Engine Constants Header
	This file is part of the K-Engine.

	Copyright (C) 2021 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_CONSTANTS_H
#define K_ENGINE_CONSTANTS_H

// ****************************************************************************
//  K-Engine Window Style
//  Note: This constants were based on Win32 API.
// ****************************************************************************
#define K_WINDOW_SPLASH              0x01
#define K_WINDOW_DEFAULT             0x02
#define K_WINDOW_NO_SYS              0x03
#define K_WINDOW_WINDOWED_FULLSCREEN 0x04
#define K_WINDOW_COMPLETE            0x05

// ****************************************************************************
//  K-Engine Window Show Types
//  Note: This constants were based on Win32 API.
// ****************************************************************************
#define K_WINDOW_SHOW          0x05
#define K_WINDOW_FORCEMINIMIZE 0x0B

// ****************************************************************************
//  K-Engine Runtime States
// ****************************************************************************
#define K_RUNNING 0x01
#define K_STOPPED 0x02
#define K_PAUSED  0x03

// ****************************************************************************
//  Rendering System Context (2D or 3D)
// ****************************************************************************
#define K_CONTEXT_2D 0x01
#define K_CONTEXT_3D_PERSPECTIVE 0x02
#define K_CONTEXT_3D_ORTHOGRAPHIC 0x03

// ****************************************************************************
//  K-Engine Mouse
// ****************************************************************************
#define K_MOUSE_LEFT_BUTTON   0
#define K_MOUSE_MIDDLE_BUTTON 1
#define K_MOUSE_RIGHT_BUTTON  2

// ****************************************************************************
//  Common Colors
// ****************************************************************************
#define K_YELLOW_SHISA          0xF7C217
#define K_DARK_AUBERGINE_UBUNTU 0x2C001E

#endif