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

#include <kemodel.h>

// remover estes header após validar o tempo de execuçao com HighTimer
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

// ****************************************************************************
//  OBJReader - Public Methods Definition
// ****************************************************************************
bool KEModel::loadfile(const char *filename)
{
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	std::ifstream objfile(filename, std::ios::in);

	if(!objfile)
	{
		std::cout << "(x) This file " << filename << " cannot be opened." << std::endl;
		// throw std::exception("This file cannot be opened.\n");
		return false;
	}

	std::string data;
	std::string element;

	while(objfile)
	{
		std::getline (objfile, data);
		std::istringstream dataStream(data);
		dataStream >> element;

		// Geometric Vertex
		if(element == "v")
		{
			GEOMETRIC_VERTEX vertex;
			vertex.w = 1.0;

			dataStream >> vertex.x >> vertex.y >> vertex.z >> vertex.w;
			geometricVertices.push_back(vertex);
		}

		// Parameter Space Vertices
		if(element == "vp")
		{
			PARAMETER_SPACE_VERTEX parameterSpaceVertex;
			parameterSpaceVertex.w = 1.0;

			dataStream >> parameterSpaceVertex.u >> parameterSpaceVertex.v >> parameterSpaceVertex.w;
			parameterSpaceVertices.push_back(parameterSpaceVertex);
		}

		if(element == "vn")
		{
			NORMAL_VERTEX normal;

			dataStream >> normal.i >> normal.j >> normal.k;
			vertexNormals.push_back(normal);
		}

		if(element == "vt")
		{
			TEXTURE_VERTEX textureVertex;
			textureVertex.v = 0.0;
			textureVertex.w = 0.0;

			// v is an optional argument
			// v is the value for the vertical direction of the texture. The
			// default is 0.
			// w is an optional argument.
			// w is a value for the depth of the texture. The default is 0.
			dataStream >> textureVertex.u >> textureVertex.v >> textureVertex.w;
			textureVertices.push_back(textureVertex);
		}

		if(element == "f")
		{
			std::string vertex_index_string;
			std::string temp;
			int vertex_index;
			int vertex_texture_index;
			int vertex_normal_index;
			
			FACE face;

			while(dataStream)
			{
				dataStream >> vertex_index_string;

				vertex_index = 0;
				vertex_texture_index = 0;
				vertex_normal_index = 0;
				
				if(!vertex_index_string.empty())
				{
					// std::replace(vertex_index_string.begin(), vertex_index_string.end(), '/', ' ');
					vertex_index_string = std::regex_replace(vertex_index_string, std::regex("/"), " / ");
					std::istringstream vertexIndexStream(vertex_index_string);

					vertexIndexStream >> vertex_index;
					face.vertex_index.push_back(vertex_index);

					if(vertex_index_string.find('/') != std::string::npos)
					{
						vertexIndexStream >> temp >> temp;

						if(temp != "/")
						{
							vertex_texture_index = std::stoi(temp);
							face.vertex_texture_index.push_back(vertex_texture_index);
						}

						temp = "";
						vertexIndexStream >> temp >> temp;

						if(!temp.empty() && temp != "/")
						{
							vertex_normal_index = std::stoi(temp);
							face.vertex_normal_index.push_back(vertex_normal_index);
						}

						temp = "";
					}
				}

				vertex_index_string = "";
			}

			faces.push_back(face);
		}

		element = "";
	}

	LARGE_INTEGER endTime;
	QueryPerformanceCounter(&endTime);
	std::cout << "MODEL | loadfile | total time: " << (endTime.QuadPart - time.QuadPart) << std::endl;

	return true;
}

void KEModel::print() const
{
	std::cout << "GEOMETRIC VERTEX: " << geometricVertices.size() << std::endl;

	for(int i = 0; i < geometricVertices.size(); i++)
	{
		std::cout << "v: "
			<< geometricVertices[i].x << " "
			<< geometricVertices[i].y << " "
			<< geometricVertices[i].z << " "
			<< geometricVertices[i].w << " " << std::endl;
	}

	std::cout << "TEXTURE_VERTEX: " << textureVertices.size() << std::endl;

	for(int i = 0; i < textureVertices.size(); i++)
	{
		std::cout << "vt: "
			<< textureVertices[i].u << " "
			<< textureVertices[i].v << " "
			<< textureVertices[i].w << " " << std::endl;
	}

	std::cout << "NORMAL_VERTEX: " << vertexNormals.size() << std::endl;

	for(int i = 0; i < vertexNormals.size(); i++)
	{
		std::cout << "vn: "
			<< vertexNormals[i].i << " "
			<< vertexNormals[i].j << " "
			<< vertexNormals[i].k << " " << std::endl;
	}

	std::cout << "PARAMETER_SPACE_VERTEX: " << parameterSpaceVertices.size() << std::endl;

	for(int i = 0; i < parameterSpaceVertices.size(); i++)
	{
		std::cout << "vp: "
			<< parameterSpaceVertices[i].u << " "
			<< parameterSpaceVertices[i].v << " "
			<< parameterSpaceVertices[i].w << " " << std::endl;
	}

	std::cout << "FACE: " << faces.size() << std::endl;

	for(int i = 0; i < faces.size(); i++)
	{
		std::cout << "f: ";

		for(int j = 0; j < faces[i].vertex_index.size(); j++)
		{
			std::cout << faces[i].vertex_index[j] << "/";

			if(faces[i].vertex_texture_index.size())
				std::cout << faces[i].vertex_texture_index[j];

			if(faces[i].vertex_normal_index.size())
				std::cout << "/" << faces[i].vertex_normal_index[j];

			std::cout << " ";
		}

		std::cout << std::endl;
	}
}