/*
	K-Engine Rendering System
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

#ifndef K_ENGINE_RENDERING_SYSTEM_H
#define K_ENGINE_RENDERING_SYSTEM_H

class KEWINAPIWrapper;
class KEModel;

typedef struct {
	double left;
	double right;
	double top;
	double bottom;
} KERECT;

void drawModel(const KEModel &model);

// ****************************************************************************
//  K-Engine Rendering System Class
// ****************************************************************************
class KERenderingSystem
{
public:
	// ------------------------------------------------------------------------
	//  Constructors and Destructors
	// ------------------------------------------------------------------------
	KERenderingSystem(KEWINAPIWrapper* apiWrapperParam);

	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	void setViewport(int x, int y, int width, int height);
	void setProjection();
	int initialize();

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	void setRenderingContext(int renderingContextParam);

	void setRenderingWindow(double left, double right, double bottom, double top);
	
	void setWindowAspectCorrectionState(bool state);
	
	void setProjectionFOVY(double fovyParam);
	double getProjectionFOVY() const;

	void setProjectionZNear(double projectionZNearParam);
	
	void setProjectionZFar(double projectionZFarParam);
	
	void setZoom(double zoomParam);
	double getZoom() const;
	
	void setRenderingWindowOffsetX(double offset);
	double getRenderingWindowOffsetX() const;
	
	void setRenderingWindowOffsetY(double offset);
	double getRenderingWindowOffsetY() const;

private:
	KEWINAPIWrapper* apiWrapper;
	int renderingContext;
	int viewportWidth;
	int viewportHeight;
	KERECT renderingWindow;
	double windowAspectCorrection;
	bool windowAspectCorrectionState;
	double projectionFOVY;
	double projectionZNear;
	double projectionZFar;
	double zoom;
	double renderingWindowOffsetX;
	double renderingWindowOffsetY;
};

#endif

/*
	glBegin(GL_LINES);

	glVertex3f(-25.0f, 25.0f, 25.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);

	glVertex3f(25.0f, 25.0f, 25.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);

	glVertex3f(25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, 25.0f);

	glVertex3f(-25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);

	// ------------------------------

	glVertex3f(-25.0f, 25.0f, 25.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);

	glVertex3f(-25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);
	
	glVertex3f(25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);

	glVertex3f(25.0f, 25.0f, 25.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);

	// ------------------------------
	glVertex3f(-25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);

	glVertex3f(25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, -25.0f, -25.0f);

	glVertex3f(25.0f, -25.0f, -25.0f);
	glVertex3f(-25.0f, -25.0f, -25.0f);

	glVertex3f(-25.0f, -25.0f, -25.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);

	// ------------------------------
	glVertex3f(-25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, -25.0f);

	glVertex3f(-25.0f, -25.0f, -25.0f);
	glVertex3f(25.0f, -25.0f, -25.0f);

	glVertex3f(25.0f, -25.0f, -25.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);

	glVertex3f(25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, 25.0f);

	// ------------------------------
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);
	glVertex3f(-25.0f, 25.0f, 25.0f);

	glVertex3f(-25.0f, 25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, 25.0f);

	glVertex3f(-25.0f, -25.0f, 25.0f);
	glVertex3f(-25.0f, -25.0f, -25.0f);

	glVertex3f(-25.0f, -25.0f, -25.0f);
	glVertex3f(-25.0f, 25.0f, -25.0f);

	// ------------------------------
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, 25.0f);

	glVertex3f(25.0f, 25.0f, 25.0f);
	glVertex3f(25.0f, -25.0f, 25.0f);
	
	glVertex3f(25.0f, -25.0f, 25.0f);
	glVertex3f(25.0f, -25.0f, -25.0f);

	glVertex3f(25.0f, -25.0f, -25.0f);
	glVertex3f(25.0f, 25.0f, -25.0f);

	glEnd();
*/