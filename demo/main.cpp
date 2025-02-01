/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#include <game.hpp>

int main()
{
	K_LOG_OUTPUT_RAW("> Welcome to K-Engine Game Engine! " + kengine::version() + "\n");
	K_LOG_OUTPUT_RAW(kengine::getDataTypeInfo());

	kengine::core engine;
	demo::game scene(&engine);

	/*
		On Android platforms, the window is created and destroyed implicitly by Android Surface Object
	*/
#ifndef __ANDROID__
	scene.createWindow(kengine::getDisplayCenterPosX(640), kengine::getDisplayCenterPosY(480), 640, 480, "K-Engine! " + kengine::version());
	scene.showWindow();
#endif

	scene.start();

	K_LOG_OUTPUT_RAW("> End of K-Engine Game Engine!\n");
	return 0;
}