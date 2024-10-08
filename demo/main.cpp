/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#include <game.hpp>

int main()
{
	K_LOG_OUTPUT_RAW("> Welcome to K-Engine Game Engine! v" + kengine::version() + "\n");

	/*
		Platform info
	*/
	K_LOG_OUTPUT_RAW(kengine::infoType());

	kengine::core engine;
	demo::game scene(&engine);

	/*
		On Android platforms, the window is created (and destroyed) implicitly by Android Surface Object
	*/

#ifndef __ANDROID__
	scene.createWindow(kengine::getDisplayCenterPosX(640), kengine::getDisplayCenterPosY(480), 640, 480, "K-Engine! v" + kengine::version());
	scene.showWindow();
#endif

	/*
		Nota: É recomendável que o sistema de renderização seja criado no evento "onWindowReady".
		Nas plataformas Android, a janela é criada implicitamente pelo Android Surface. Portanto,
		este evento foi criado especificamente para notificafar o usuário quando a janela estiver disponível.
	*/

	scene.start();

	K_LOG_OUTPUT_RAW("> End of K-Engine Game Engine!\n");
	return 0;
}