/*
   K-Engine Scene Graph
   This file is part of the K-Engine.

   Copyright (C) 2020-2026 Fabio Takeshi Ishikawa

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

#ifndef K_ENGINE_SCENE_GRAPH_HPP
#define K_ENGINE_SCENE_GRAPH_HPP

#include <vector>

namespace kengine {
	
	/*
	*/
	class node
	{
	public:
		//node() {}
		//explicit node(int index) : m_index{ index } {}
		//void setIndex(int index) { m_index = index; }

	private:
		//int m_index = -1;
		//node* m_parent = nullptr;
		std::vector<node*> m_children;
	};
}

#endif