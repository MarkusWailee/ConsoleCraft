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
	void init_textures()
	{
		//Grass
		Terminal3D::add_texture_ppm(1, "src/Textures/grass_block_side.ppm");
		Terminal3D::add_texture_ppm(2, "src/Textures/grass_block_top.ppm");
		Terminal3D::add_texture_ppm(3, "src/Textures/dirt.ppm");
		//Cobble
		Terminal3D::add_texture_ppm(4, "src/Textures/cobblestone.ppm");
		//oad_plank
		Terminal3D::add_texture_ppm(5, "src/Textures/oak_planks.ppm");
		//oak_log
		Terminal3D::add_texture_ppm(6, "src/Textures/oak_log.ppm");
		Terminal3D::add_texture_ppm(7, "src/Textures/oak_log_top.ppm");
		//crafting table
		Terminal3D::add_texture_ppm(8, "src/Textures/crafting_table_side.ppm");
		Terminal3D::add_texture_ppm(9, "src/Textures/crafting_table_front.ppm");
		Terminal3D::add_texture_ppm(10, "src/Textures/crafting_table_top.ppm");

		//furnace
		Terminal3D::add_texture_ppm(11, "src/Textures/furnace_front.ppm");
		Terminal3D::add_texture_ppm(12, "src/Textures/furnace_side.ppm");
		Terminal3D::add_texture_ppm(13, "src/Textures/furnace_top.ppm");

		//leaves
		Terminal3D::add_texture_ppm(14, "src/Textures/oak_leaves.ppm");
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
