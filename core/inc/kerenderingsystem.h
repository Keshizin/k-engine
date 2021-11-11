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
class DIB;
class KELight;

typedef struct {
	double left;
	double right;
	double top;
	double bottom;
} KERECT;

void drawModel(const KEModel &model, int mode);

void setVertexArray(KEModel &model);
void drawModel2(const KEModel &model, int mode);

void drawImage(int posX, int posY, const DIB &image);
void setLight(const KELight &light, int isLightEnable, int lightParam);

void set(const KEModel &model);
void draw();

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
	void setBackgroundColor(float red, float green, float blue, float alpha);
	int initialize();
	void setLightModel(int isLightEnable);
	int setVSync(int vsync);
	void getGLVersion() const;

	// ------------------------------------------------------------------------
	//  Getters and Setters
	// ------------------------------------------------------------------------
	int getRenderingContext() const;
	void setRenderingContext(int renderingContextParam);

	void setRenderingWindow(double left, double right, double bottom, double top);
	KERECT getRenderingWindow() const;
	
	void setWindowAspectCorrectionState(bool state);
	
	void setProjectionFOVY(double fovyParam);
	double getProjectionFOVY() const;

	void setProjectionZNear(double projectionZNearParam);
	double getProjectionZNear() const;
	
	void setProjectionZFar(double projectionZFarParam);
	double getProjectionZFar() const;
	
	void setZoom(double zoomParam);
	double getZoom() const;
	
	void setRenderingWindowOffsetX(double offset);
	double getRenderingWindowOffsetX() const;
	
	void setRenderingWindowOffsetY(double offset);
	double getRenderingWindowOffsetY() const;

	void setLightModelAmbient(float red, float green, float blue, float alpha);

private:
	KEWINAPIWrapper* apiWrapper;
	int renderingContext;
	int viewportWidth;
	int viewportHeight;
	KERECT renderingWindow;
	bool windowAspectCorrectionState;
	double projectionFOVY;
	double projectionZNear;
	double projectionZFar;
	double zoom;
	double renderingWindowOffsetX;
	double renderingWindowOffsetY;
	float lightModelAmbient[4];
};

#endif