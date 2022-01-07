/*
	K-Engine Shader Class
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

#include <keshader.h>

#include <iostream>
#include <fstream>

KEGLSLShader::KEGLSLShader(std::string path, std::string filename) :
	rows(0), source(0)
{
	loadShader(path, filename);
}

KEGLSLShader::~KEGLSLShader()
{
	delete source;
}

int KEGLSLShader::loadShader(std::string path, std::string filename)
{
	std::ifstream shaderfile(path + filename, std::ios::in | std::ios::binary);

	if(!shaderfile)
	{
		std::cout << "(x) This file " << path + filename << " cannot be opened." << std::endl;
		// throw std::exception("This file cannot be opened.\n");
		return false;	
	}

	shaderfile.seekg(0, std::ios::end);
	// int fileSize = shaderfile.tellg();
	std::streamoff fileSize = shaderfile.tellg();
	shaderfile.seekg(0, std::ios::beg);
	source = new char[ static_cast<size_t>(fileSize)];
	shaderfile.read(reinterpret_cast<char*>(source), fileSize);
	std::string s = source;
	rows = std::count(s.begin(), s.end(), '\n');
	return true;
}

long long int KEGLSLShader::getRows() const
{
	return rows;
}

const char * KEGLSLShader::getSource() const
{
	return source;
}
