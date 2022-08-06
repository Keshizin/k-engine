/*
	K-Engine Demonstration
	This file provide a template for a game created with K-Engine.

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

#include <splash.h>
#include <game.h>
#include <keaux.h>

// ----------------------------------------------------------------------------
//  Point Entry Execution
// ----------------------------------------------------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	K_UNREFERENCED_PARAMETER(hInstance);
	K_UNREFERENCED_PARAMETER(hPrevInstance);
	K_UNREFERENCED_PARAMETER(lpCmdLine);
	K_UNREFERENCED_PARAMETER(nCmdShow);

	kengine::core* engine = new kengine::core();
	kengine::scene_manager* sceneManager = new kengine::scene_manager;

	size_t splashScene = sceneManager->addScene(new game::SplashScene(engine, sceneManager));
	sceneManager->setScene(splashScene);
	sceneManager->removeScene(splashScene);

	size_t gameScene = sceneManager->addScene(new game::GameScene(engine, sceneManager));
	sceneManager->setScene(gameScene);

	delete sceneManager;
	delete engine;

	return 1;
}