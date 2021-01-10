/*
	Game Engine Rendering System
	This file is part of the K-Engine.

	Copyright (C) 2020 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_RENDERING_SYSTEM_H
#define K_ENGINE_RENDERING_SYSTEM_H

#include <geapiwrapper.h>

// ****************************************************************************
//  Rendering System Context (2D or 3D)
// ****************************************************************************
#define K_CONTEXT_2D 0x01
#define K_CONTEXT_3D_PERSPECTIVE 0x02
#define K_CONTEXT_3D_ORTOGRAPHIC 0x03

// ****************************************************************************
//  Rendering System Class
// ****************************************************************************
class GERenderingSystem
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructors
	// ------------------------------------------------------------------------
	GERenderingSystem(GEAPIWrapper *apiWrapper);

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	int initialize();
	void renderFrame();
	int setVSync(int vsync);
	void setViewport(int x, int y, int width, int height);
	void setProjection();
	void drawGlobaldAxis();

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	void setRenderingContext(int renderingContext);
	int getRenderingContext();

	void setWindow(double left, double right, double bottom, double top);

	void setProjectionZNear(double projectionZNear);
	double getProjectionZNear();
	
	void setProjectionZFar(double projectionZFar);
	double getProjectionZFar();
	
	void setProjectionFOVY(double fovy);
	double getProjectionFOVY();

	void setWindowAspectCorrection(double windowAspectCorrection);
	double getWindowAspectCorrection();

	void setWindowAspectCorrectionState(bool state);
	bool getWindowAspectCorrectionState();

	void setGlobalAxisState(bool state);

private:
	GEAPIWrapper *apiWrapper;

	int renderingContext;
	int viewportWidth;
	int viewportHeight;
	double windowLeft;
	double windowRight;
	double windowTop;
	double windowBottom;
	double projectionZNear;
	double projectionZFar;
	double projectionFOVY;
	double windowAspectCorrection;

	bool windowAspectCorrectionState;
	bool globalAxisState;
};

#endif