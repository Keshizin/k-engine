/*
	K-Engine Lighting Header
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

#ifndef K_ENGINE_LIGHT_H
#define K_ENGINE_LIGHT_H

#include <kemath.h>

namespace kengine
{
	/*
	* 
	*  Enums for source light type
	* 
	*/
	enum class SOURCE_LIGHT_TYPE {
		K_LIGHT_TYPE_SINGLE_POINT,
		K_LIGHT_TYPE_DIRECTIONAL,
		K_LIGHT_TYPE_SPOT
	};


	/*
	*
	*  kengine::phong_light class
	*
	*/
	class phong_light
	{
	public:
		phong_light();
		phong_light(SOURCE_LIGHT_TYPE t, vec3<float> p, vec3<float> a, vec3<float> d, vec3<float> s, vec3<float> sd, float sc, float se);
		~phong_light();

		phong_light(const phong_light& copy) = delete; // copy constructor
		phong_light(phong_light&& move) noexcept = delete; // move constructor
		phong_light& operator=(const phong_light& copy) = delete; // copy assignment

		SOURCE_LIGHT_TYPE type;
		vec3<float> position; // point position for point light source or direction for directional light source
		vec3<float> La;
		vec3<float> Ld;
		vec3<float> Ls;
		vec3<float> spot_direction; // direction of the spotlight in eye space (view * model)
		float spot_cutoff; // angle in radian between 0 and pi/2
		float spot_exponent;
	};


	/*
	* 
	*  kengine::phong_material class
	* 
	*/
	class phong_material
	{
	public:
		phong_material();
		phong_material(vec3<float> a, vec3<float> d, vec3<float> s, float shininessParam);
		~phong_material();

		phong_material(const phong_material& copy) = delete; // copy constructor
		phong_material(phong_material&& move) noexcept = delete; // move constructor
		phong_material& operator=(const phong_material& copy) = delete; // copy assignment

		vec3<float> Ka;
		vec3<float> Kd;
		vec3<float> Ks;
		float shininess;
	};


	/*
	* 
	*  Enums for Lighting Model Type
	* 
	*/
	enum class LIGHTING_MODEL_TYPE {
		K_DIFFUSE_REFLECTION_MODEL,
		K_PHONG_REFLECTION_MODEL,
		K_BLINN_PHONG_REFLECTION_MODEL
	};


	/*
	*
	*  kengine::lighting_model class
	*
	*/
	class lighting_model
	{
	public:
		lighting_model();
		~lighting_model();

		lighting_model(const lighting_model& copy) = delete; // copy constructor
		lighting_model(lighting_model&& move) noexcept = delete; // move constructor
		lighting_model& operator=(const lighting_model& copy) = delete; // copy assignment

		bool nonLocalViewer;
		bool twoSidedShading;
		bool flatShading;
		LIGHTING_MODEL_TYPE modelType;
	};


	/*
	*
	*  kengine::toon_shading_info class (deprecated)
	*
	*/
	class toon_shading_info
	{
	public:
		toon_shading_info();
		~toon_shading_info();

		toon_shading_info(const toon_shading_info& copy) = delete; // copy constructor
		toon_shading_info(toon_shading_info&& move) noexcept = delete; // move constructor
		toon_shading_info& operator=(const toon_shading_info& copy) = delete; // copy assignment

		int levels;
		float scaleFactor;
	};


	/*
	*
	*  kengine::fog_info class (deprecated)
	*
	*/
	class fog_info
	{
	public:
		fog_info();
		~fog_info();

		fog_info(const fog_info& copy) = delete; // copy constructor
		fog_info(fog_info&& move) noexcept = delete; // move constructor
		fog_info& operator=(const fog_info& copy) = delete; // copy assignment

		float max_distance;
		float min_distance;
		vec3<float> color;
		int enable;
	};


	/*
	*
	*  kengine::pbr_light class
	*
	*/
	class pbr_light
	{
	public:
		pbr_light();
		~pbr_light();

		pbr_light(const pbr_light& copy) = delete; // copy constructor
		pbr_light(pbr_light&& move) noexcept = delete; // move constructor
		pbr_light& operator=(const pbr_light& copy) = delete; // copy assignment

		vec3<float> position; // light position
		vec3<float> L; // light intensity
	};


	/*
	*
	*  kengine::pbr_material class
	*
	*/
	class pbr_material
	{
	public:
		pbr_material();
		~pbr_material();

		pbr_material(const pbr_material& copy) = delete; // copy constructor
		pbr_material(pbr_material&& move) noexcept = delete; // move constructor
		pbr_material& operator=(const pbr_material& copy) = delete; // copy assignment

		float rough; // roughness
		int metal; // mettalic (true) or dieletric (false)
		vec3<float> color; // diffuse color for dielectrics, f0 for mettalic
	};
}

#endif