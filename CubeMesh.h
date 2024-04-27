#pragma once
#pragma once
#include "AsciGraphics/Math/mat.h"

//This is the order I render the voxels
namespace Cube
{
	const std::vector<unsigned int> block_type =
	{
		//air 
		0,0,0,0,0,0,

		//grass 1
		1,1,1,1,3,2,

		//cobblestone 2
		4,4,4,4,4,4,

		//oak plank 3
		5,5,5,5,5,5,

		//log 4
		6,6,6,6,7,7,

		//crafting table 5
		9,8,9,8,10,10,

		//furnace 6
		11,12,12,12, 13,13

	};

	const vec3 vertice_data[]
	{
		//Front
		vec3(-0.5, -0.5, -0.5),	//Bottom Left
		vec3(0.5, -0.5, -0.5),	//Bottom Right
		vec3(0.5, 0.5, -0.5),	//Top Right
		vec3(-0.5, 0.5, -0.5),	//Top Left

		//Right
		vec3(0.5,-0.5,-0.5),
		vec3(0.5,-0.5,0.5),
		vec3(0.5,0.5,0.5),
		vec3(0.5,0.5,-0.5),

		//Back
		vec3(0.5,-0.5,0.5),
		vec3(-0.5,-0.5,0.5),
		vec3(-0.5,0.5,0.5),
		vec3(0.5,0.5,0.5),

		//Left
		vec3(-0.5, -0.5, 0.5),
		vec3(-0.5, -0.5, -0.5),
		vec3(-0.5, 0.5, -0.5),
		vec3(-0.5,0.5,0.5),

		//Bottom
		vec3(-0.5, -0.5, 0.5),
		vec3(0.5, -0.5, 0.5),
		vec3(0.5, -0.5, -0.5),
		vec3(-0.5, -0.5, -0.5),

		//Top
		vec3(-0.5, 0.5, -0.5),
		vec3(0.5, 0.5, -0.5),
		vec3(0.5, 0.5, 0.5),
		vec3(-0.5, 0.5, 0.5)
	};
}
