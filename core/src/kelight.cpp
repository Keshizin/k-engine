/*
	K-Engine Light Header
	This file is part of the K-Engine.

	Copyright (C) 2022 Fabio Takeshi Ishikawa

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

/*
*
*  kengine::phong_light class
*
*/
kengine::phong_light::phong_light()
	:
	type{ SOURCE_LIGHT_TYPE::K_LIGHT_TYPE_SINGLE_POINT},
	position{ 0.0f, 0.0, 0.0f },
	La{ 0.3f, 0.3f, 0.3f },
	Ld{ 0.8f, 0.8f, 0.8f },
	Ls{ 0.8f, 0.8f, 0.8f },
	spot_direction{ 0.0f, 0.0f, 0.0f },
	spot_cutoff{ 0.0f },
	spot_exponent{ 0.0f }
{
}


kengine::phong_light::phong_light(SOURCE_LIGHT_TYPE t, vec3<float> p, vec3<float> a, vec3<float> d, vec3<float> s, vec3<float> sd, float sc, float se)
	:
	type{ t },
	position{ p },
	La{ a },
	Ld{ d },
	Ls{ s },
	spot_direction{ sd },
	spot_cutoff{ sc },
	spot_exponent{ se }
{
}


kengine::phong_light::~phong_light()
{
}



/*
*
*  kengine::phong_material class
*
*/
kengine::phong_material::phong_material()
	:
	Ka{ 0.3f, 0.3f, 0.3f },
	Kd{ 0.8f, 0.8f, 0.8f },
	Ks{ 0.8f, 0.8f, 0.8f },
	shininess{10.0f}
{
}


kengine::phong_material::phong_material(vec3<float> a, vec3<float> d, vec3<float> s, float shininessParam)
	:
	Ka{ a },
	Kd{ d },
	Ks{ s },
	shininess{ shininessParam }
{
}


kengine::phong_material::~phong_material()
{
}


/*
*
*  kengine::lighting_model class
*
*/
kengine::lighting_model::lighting_model()
	:
	nonLocalViewer{ false },
	twoSidedShading{ false },
	flatShading{ false },
	modelType{ LIGHTING_MODEL_TYPE::K_BLINN_PHONG_REFLECTION_MODEL }
{
}


kengine::lighting_model::~lighting_model()
{
}


/*
*
*  kengine::toon_shading_info class (deprecated)
*
*/
kengine::toon_shading_info::toon_shading_info()
	: levels{0}, scaleFactor{0.0f}
{
}


kengine::toon_shading_info::~toon_shading_info()
{
}


/*
*
*  kengine::fog_info class
*
*/
kengine::fog_info::fog_info()
	:
		max_distance{ 0.0f },
		min_distance{ 0.0f },
		color{ 0.0f, 0.0f, 0.0f },
		enable{ 0 }
{
}


kengine::fog_info::~fog_info()
{
}


/*
*
*  kengine::pbr_light class
*
*/
kengine::pbr_light::pbr_light()
	:
	position{ 0.0f, 0.0f, 0.0f },
	L{ 0.0f, 0.0f, 0.0f }
{
}


kengine::pbr_light::~pbr_light()
{
}


/*
*
*  kengine::pbr_material class
*
*/
kengine::pbr_material::pbr_material()
	:
		rough{ 0.0f },
		metal{ 0 },
		color{ 0.0f, 0.0f, 0.0f }
{
}


kengine::pbr_material::~pbr_material()
{
}