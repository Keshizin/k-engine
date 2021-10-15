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

typedef struct {
	double x, y, z, w;
} GEOMETRIC_VERTEX;

typedef struct {
	double u, v, w;
} PARAMETER_SPACE_VERTEX;

typedef struct {
	double u, v, w;
} TEXTURE_VERTEX;

typedef struct {
	double i, j, k;
} NORMAL_VERTEX;

typedef struct {
	int total_of_vertices;
	// int vertex_index[4];
	std::vector<int> vertex_index;
	std::vector<int> vertex_texture_index;
	std::vector<int> vertex_normal_index;
} FACE;

typedef struct {
	GEOMETRIC_VERTEX *vertices;
	FACE *faces;
	int total_of_faces;
} OBJ;

typedef struct {
	GEOMETRIC_VERTEX *vertices;
	// COLOR *colors;
	unsigned int *indices;
	
	unsigned int total_indices;
	unsigned int total_vertex;
} MODEL;

void drawOBJ(OBJ *obj);

// ****************************************************************************
//  K-Engine OBJReader Class
// ****************************************************************************
class OBJReader
{
public:
	// ------------------------------------------------------------------------
	//  Public Methods
	// ------------------------------------------------------------------------
	bool loadfile(const char *filename);

private:
};

#endif