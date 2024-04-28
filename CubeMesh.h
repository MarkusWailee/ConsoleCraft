#pragma once
#pragma once
#include "AsciGraphics/Math/mat.h"

//This is the order I render the voxels
namespace Cube
{

	std::vector<vec3> tree_blocks = {};
	std::vector<unsigned char> tree_assemble = {};
	inline void init_tree()
	{
		vec3 base = vec3(0,0,0);

		for (int y = 4; y <= 8; y++)
			for (int z = -2; z <= 2; z++)
				for (int x = -2; x <= 2; x++)
				{
					if (x * x + z * z + (y-5) * (y-5)< 6 || y<6)
					{
						tree_blocks.push_back(vec3(x, y, z));
						tree_assemble.push_back(7);

					}
				}
		for (int i = 0; i <= 3; i++)
		{
			tree_blocks.push_back(base + vec3(0,i,0));
			tree_assemble.push_back(4);
		}
	}

		



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
		11,12,12,12,13,13,

		//oak leaves
		14,14,14,14,14,14

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
