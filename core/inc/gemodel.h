/*
	Game Engine Model
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

#ifndef GAME_ENGINE_MODEL_H
#define GAME_ENGINE_MODEL_H

typedef struct {
	double x, y, z;
} VERTEX;

typedef struct {
	int total;
	int vertex_index[3];
} FACE;

typedef struct {
	VERTEX *vertices;
	FACE *faces;
	int total;
} MODEL;

typedef struct {
	double left;
	double right;
	double top;
	double bottom;
} GERECT;

void drawGEModel(MODEL *model);

// ----------------------------------------------------------------------------
//  BASE MODEL FOR 2D OBJECTS - SPRITES
// ----------------------------------------------------------------------------
// #ifndef SPRITE_2D_SIZE
// #define SPRITE_2D_SIZE 32
// #endif

// VERTEX sprite2D_vertices[] = {
// 	{-SPRITE_2D_SIZE,  SPRITE_2D_SIZE, 0.0},
// 	{ SPRITE_2D_SIZE,  SPRITE_2D_SIZE, 0.0},
// 	{-SPRITE_2D_SIZE, -SPRITE_2D_SIZE, 0.0},
// 	{ SPRITE_2D_SIZE, -SPRITE_2D_SIZE, 0.0}
// };

// FACE sprite2D_faces[] = {
// 	{3, {0, 1, 2}},
// 	{3, {1, 3, 2}}
// };

// MODEL sprite2D_model = {
// 	sprite2D_vertices, sprite2D_faces, 2
// };

#endif