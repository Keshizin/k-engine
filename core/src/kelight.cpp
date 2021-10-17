/*
	K-Engine Light Class
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

#include <kelight.h>

// ****************************************************************************
//  KELight Class - Constructors and Destructors
// ****************************************************************************
KELight::KELight() : diffuse(), specular(), ambient(), position()
{
}

// ****************************************************************************
//  KELight Class - Getters and Setters
// ****************************************************************************
void KELight::setAmbient(float red, float green, float blue, float alpha)
{
	ambient[0] = red;
	ambient[1] = green;
	ambient[2] = blue;
	ambient[3] = alpha;
}

void KELight::setDiffuse(float red, float green, float blue, float alpha)
{
	diffuse[0] = red;
	diffuse[1] = green;
	diffuse[2] = blue;
	diffuse[3] = alpha;
}

void KELight::setSpecular(float red, float green, float blue, float alpha)
{
	specular[0] = red;
	specular[1] = green;
	specular[2] = blue;
	specular[3] = alpha;
}

void KELight::setPosition(float x, float y, float z, float w)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
	position[3] = w;
}