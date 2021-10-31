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
#include <iomanip>

// ****************************************************************************
//  OBJReader - Public Methods Definition
// ****************************************************************************
bool KEModel::loadfile(std::string path, std::string filename)
{
	// LARGE_INTEGER time;
	// QueryPerformanceCounter(&time);

	std::ifstream objfile(path + filename, std::ios::in);

	if(!objfile)
	{
		std::cout << "(x) This file " << filename << " cannot be opened." << std::endl;
		// throw std::exception("This file cannot be opened.\n");
		return false;
	}

	std::string data;
	std::string element;
	std::string current_material;

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
			face.material_name = current_material;

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

		if(element == "mtllib")
		{
			std::string mtlFileName;
			dataStream >> mtlFileName;
			loadMTLFile(path, mtlFileName);
		}

		if(element == "usemtl")
		{
			std::string material_name;
			dataStream >> material_name;
			current_material = material_name;
		}

		element = "";
	}

	// LARGE_INTEGER endTime;
	// QueryPerformanceCounter(&endTime);
	// std::cout << "MODEL | loadfile | total time: " << (endTime.QuadPart - time.QuadPart) << std::endl;

	return true;
}

bool KEModel::loadMTLFile(std::string path, std::string filename)
{
	std::ifstream mtlfile(path + filename, std::ios::in);

	if(!mtlfile)
	{
		std::cout << "(x) This file " << filename << " cannot be opened." << std::endl;
		// throw std::exception("This file cannot be opened.\n");
		return false;
	}

	std::string data;
	std::string element;

	while(mtlfile)
	{
		std::getline (mtlfile, data);
		std::istringstream dataStream(data);
		dataStream >> element;

		if(element == "newmtl")
		{
			MATERIAL material;
			dataStream >> material.name;
			materials.push_back(material);
		}

		if(element == "Ns")
		{
			dataStream >> materials.back().Ns;
		}

		if(element == "Ka")
		{
			dataStream >> materials.back().Ka[0] >> materials.back().Ka[1] >> materials.back().Ka[2];
		}

		if(element == "Kd")
		{
			dataStream >> materials.back().Kd[0] >> materials.back().Kd[1] >> materials.back().Kd[2];
		}

		if(element == "Ks")
		{
			dataStream >> materials.back().Ks[0] >> materials.back().Ks[1] >> materials.back().Ks[2];
		}

		if(element == "Ke")
		{
			dataStream >> materials.back().Ke[0] >> materials.back().Ke[1] >> materials.back().Ke[2];
		}

		if(element == "Ni")
		{
			dataStream >> materials.back().Ni;
		}

		if(element == "d")
		{
			dataStream >> materials.back().d;
		}

		if(element == "illum")
		{
			dataStream >> materials.back().illum;
		}

		if(element == "map_Kd")
		{
			dataStream >> materials.back().map_Kd;
		}

		element = "";
	}

	return true;
}

int KEModel::getMaterial(std::string material_name) const
{
	int index = -1;

	for(int i = 0; i < materials.size(); i++)
	{
		if(materials[i].name == material_name)
		{
			index = i;
		}
	}

	return index;
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

	std::cout << "FACE " << faces.size() << std::endl;

	for(int i = 0; i < faces.size(); i++)
	{
		std::cout << "f " << faces[i].material_name << ": ";

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

	std::cout << "MATERIALS: " << materials.size() << std::endl;

	for(int i  = 0; i < materials.size(); i++)
	{
		std::cout << "materials.name: " << materials[i].name
			<< "\nmaterials.Ns: " << materials[i].Ns
			<< "\nmaterials.Ka: " << materials[i].Ka[0] << " " << materials[i].Ka[1] << " " << materials[i].Ka[2]
			<< "\nmaterials.Kd: " << materials[i].Kd[0] << " " << materials[i].Kd[1] << " " << materials[i].Kd[2]
			<< "\nmaterials.Ks: " << materials[i].Ks[0] << " " << materials[i].Ks[1] << " " << materials[i].Ks[2]
			<< "\nmaterials.Ke: " << materials[i].Ke[0] << " " << materials[i].Ke[1] << " " << materials[i].Ke[2]
			<< "\nmaterials.Ni: " << materials[i].Ni
			<< "\nmaterials.d: " << materials[i].d
			<< "\nmaterials.illum: " << materials[i].illum
			<< "\nmaterials.map_Kd: " << materials[i].map_Kd
			<< "\n" << std::endl;
	}
}