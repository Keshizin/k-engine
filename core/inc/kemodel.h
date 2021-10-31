/*
	K-Engine Geometric Model
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

#ifndef K_ENGINE_MODEL_H
#define K_ENGINE_MODEL_H

#include <vector>
#include <string>

typedef struct {
	double x, y, z, w;
} GEOMETRIC_VERTEX;

typedef struct {
	double u, v, w;
} PARAMETER_SPACE_VERTEX;

typedef struct {
	double i, j, k;
} NORMAL_VERTEX;

typedef struct {
	double u, v, w;
} TEXTURE_VERTEX;

class MATERIAL
{
public:
	MATERIAL() :
		name(), Ns(0.0f), Ka(), Kd(), Ks(), Ke(), Ni(0.0f), d(0.0f), illum(0.0f), map_Kd()
	{
	}

	std::string name;
	float Ns; // focus of specular highlights in the material
	float Ka[4];
	float Kd[4];
	float Ks[4];
	float Ke[4];
	float Ni; // optical density (aka index of refraction)
	float d; // factor for dissolve
	float illum;
	std::string map_Kd;
};

class FACE
{
public:
	std::vector<int> vertex_index;
	std::vector<int> vertex_texture_index;
	std::vector<int> vertex_normal_index;
	std::string material_name;
};

// ****************************************************************************
//  K-Engine OBJReader Class
// ****************************************************************************
class KEModel
{
public:
	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	bool loadfile(std::string path, std::string filename);
	bool loadMTLFile(std::string path, std::string filename);
	int getMaterial(std::string material_name) const;
	void print() const;

	std::vector<GEOMETRIC_VERTEX> geometricVertices;
	std::vector<PARAMETER_SPACE_VERTEX> parameterSpaceVertices;
	std::vector<NORMAL_VERTEX> vertexNormals;
	std::vector<TEXTURE_VERTEX> textureVertices;
	std::vector<FACE> faces;
	std::vector<MATERIAL> materials;
};

#endif