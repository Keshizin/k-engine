/*
	K-Engine Event Handler
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

#include <keeventhandler.h>
#include <kecore.h>
#include <keaux.h>


/*
* 
*  kengine::scene - member class definition
* 
*/
kengine::scene::scene(core* engine, scene_manager* sceneManager)
	: engineHandle{ engine }, sceneManagerHandle{ sceneManager }
{
}


kengine::scene::~scene()
{
}


void kengine::scene::setEngine(core* engine)
{
	engineHandle = engine;
}


void kengine::scene::set()
{
	if(engineHandle != nullptr)
		engineHandle->setEventHandler(this);
}


void kengine::scene::createWindowEvent()
{
}


void kengine::scene::beforeMainLoopEvent()
{
}


void kengine::scene::afterMainLoopEvent()
{
}


void kengine::scene::finishBeforeEvent()
{
	if(engineHandle != nullptr)
		engineHandle->getGameWindow()->destroy();
}


void kengine::scene::finishAfterEvent()
{
	if(engineHandle != nullptr)
		engineHandle->stopMainLoop();
}


void kengine::scene::frameEvent(double frameTime)
{
	K_UNREFERENCED_PARAMETER(frameTime);
}


void kengine::scene::resumeEvent() {}
void kengine::scene::pauseEvent() {}


void kengine::scene::mouseEvent(int button, int state, int x, int y)
{
	K_UNREFERENCED_PARAMETER(button);
	K_UNREFERENCED_PARAMETER(state);
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}


void kengine::scene::mouseMotionEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}


void kengine::scene::keyboardEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}


void kengine::scene::keyboardSpecialEvent(unsigned long long key, int state)
{
	K_UNREFERENCED_PARAMETER(key);
	K_UNREFERENCED_PARAMETER(state);
}


void kengine::scene::resizeWindowEvent(int width, int height)
{
	K_UNREFERENCED_PARAMETER(width);
	K_UNREFERENCED_PARAMETER(height);
}


void kengine::scene::moveWindowEvent(int x, int y)
{
	K_UNREFERENCED_PARAMETER(x);
	K_UNREFERENCED_PARAMETER(y);
}


/*
* 
*  kengine::scene_manager - member class definition
* 
*/
kengine::scene_manager::scene_manager()
	: scenes{}
{
}


kengine::scene_manager::~scene_manager()
{
	for (auto i : scenes)
		delete i;
}


size_t kengine::scene_manager::addScene(scene* scene)
{
	scenes.push_back(scene);
	return scenes.size() - 1;
}


void kengine::scene_manager::removeScene(size_t pos)
{
	if (pos >= scenes.size())
		return;

	kengine::scene* s = scenes[pos];
	//delete s;
	scenes.erase(scenes.begin() + static_cast<int>(pos));
	delete s;

}


void kengine::scene_manager::setScene(size_t pos)
{
	if (pos >= scenes.size())
		return;

	scenes[pos]->set();
}