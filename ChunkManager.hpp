#pragma once
#include "AsciGraphics/AsciGraphics.h"
#include "GenerateFunctions.h"
#include "CubeMesh.h"
/* chunk_x  (World space Chunk position) World Chunk Position(WSCP)
*  block_x  (World space block position) World Block Position(WSBP)
*  x		(Chunk space block position) Chunk Block Position(CSBP)
*/ 



const int CHUNK_LENGTH = 8;
const int CHUNK_SIZE = CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_LENGTH;



struct Block
{
	//specifies which face to render
	Block(){}
	Block(unsigned char Block_Type) : block_type(Block_Type) {};
	bool face[6] = {};
	unsigned char block_type = 0;
};

struct Chunk
{
	Block* data = NULL;
	int chunk_x = INT_MAX, chunk_y = INT_MAX, chunk_z = INT_MAX;
	//r for reference
	Block& get_block_r(int x, int y, int z)
	{
		if (x < 0 || x >= CHUNK_LENGTH || y < 0 || y >= CHUNK_LENGTH || z < 0 || z >= CHUNK_LENGTH || data == NULL)
		{
			std::cout << "Incorrect Chunk Indexing in reference function\n";
			throw;
		}
		return data[x + z * CHUNK_LENGTH + y * CHUNK_LENGTH * CHUNK_LENGTH];
	}
	unsigned char get_block(int x, int y, int z)
	{
		if (x < 0 || x >= CHUNK_LENGTH || y < 0 || y >= CHUNK_LENGTH || z < 0 || z >= CHUNK_LENGTH || data == NULL)
		{
			std::cout << "Incorrect Chunk Indexing in GetBlock\n";
			throw;
		}
		return data[x + z * CHUNK_LENGTH + y * CHUNK_LENGTH * CHUNK_LENGTH].block_type;
	}
	~Chunk()
	{
		delete[] data;
	}
};

class ChunkManager
{
	int map_size = 0;
	int map_length = 0;
	Chunk* chunks = NULL;
public:
	ChunkManager(int distance) : map_length(distance), map_size(distance * distance * distance) 
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
		Cube::init_tree();

		chunks = new Chunk[map_size];
	}
	~ChunkManager() { delete[] chunks; }
	unsigned int hash_function(int chunk_x, int chunk_y, int chunk_z);
	unsigned char get_block(int x, int y, int z);
	//r for reference
	Block& get_block_r(int x, int y, int z);
	bool does_block_exist(int block_x, int block_y, int block_z);
	void add_block(int chunk_x, int chunk_y, int chunk_z);
	void mesh_chunk(int chunk_x, int chunk_y, int chunk_z);
	void mesh_block(int block_x, int block_y, int block_z);
	void mesh_adjacent_blocks(int block_x, int block_y, int block_z);
	void place_tree(int x, int y, int z);
	//I use this for raycasting 
	void render(Camera3D camera, float Brightness, vec3 sun_position);
};

inline bool ChunkManager::does_block_exist(int block_x, int block_y, int block_z)
{
	int chunk_x = (block_x / CHUNK_LENGTH) - (block_x % CHUNK_LENGTH != 0 && block_x < 0);
	int chunk_y = (block_y / CHUNK_LENGTH) - (block_y % CHUNK_LENGTH != 0 && block_y < 0);
	int chunk_z = (block_z / CHUNK_LENGTH) - (block_z % CHUNK_LENGTH != 0 && block_z < 0);
	Chunk& chunk = chunks[hash_function(chunk_x, chunk_y, chunk_z)];
	if (!(chunk.chunk_x == chunk_x && chunk.chunk_y == chunk_y && chunk.chunk_z == chunk_z))
	{
		//return air block. Helps for meshing when the chunk DNE.
		return 0;
	}
	return 1;
}

//unique identifier for each chunk
inline unsigned int ChunkManager::hash_function(int chunk_x, int chunk_y, int chunk_z)
{
	return ((chunk_x + chunk_z * map_length + chunk_y * map_length * map_length) + 536870911) % map_size;
}
inline unsigned char ChunkManager::get_block(int block_x, int block_y, int block_z)
{
	//world space block position(WSBP) to world space chunk position(WSCP).			 This code was to support negative block positions
	int chunk_x = (block_x / CHUNK_LENGTH) - (block_x % CHUNK_LENGTH != 0 && block_x < 0);
	int chunk_y = (block_y / CHUNK_LENGTH) - (block_y % CHUNK_LENGTH != 0 && block_y < 0);
	int chunk_z = (block_z / CHUNK_LENGTH) - (block_z % CHUNK_LENGTH != 0 && block_z < 0);

	Chunk& chunk = chunks[hash_function(chunk_x, chunk_y, chunk_z)];
	if (!(chunk.chunk_x == chunk_x && chunk.chunk_y == chunk_y && chunk.chunk_z == chunk_z))
	{
		//return air block. Helps for meshing when the chunk DNE.
		return 0;
	}
	//world space block position(WSBP) to Chunk space block position(CSBP)
	int x = block_x % CHUNK_LENGTH;
	int y = block_y % CHUNK_LENGTH;
	int z = block_z % CHUNK_LENGTH;
	x = x < 0 ? CHUNK_LENGTH + x : x;
	y = y < 0 ? CHUNK_LENGTH + y : y;
	z = z < 0 ? CHUNK_LENGTH + z : z;
	return chunk.get_block(x, y, z);
}
inline Block& ChunkManager::get_block_r(int block_x, int block_y, int block_z)
{
	//world space block position(WSBP) to world space chunk position(WSCP).			 This code was to support negative block positions
	int chunk_x = (block_x / CHUNK_LENGTH) - (block_x % CHUNK_LENGTH != 0 && block_x < 0);
	int chunk_y = (block_y / CHUNK_LENGTH) - (block_y % CHUNK_LENGTH != 0 && block_y < 0);
	int chunk_z = (block_z / CHUNK_LENGTH) - (block_z % CHUNK_LENGTH != 0 && block_z < 0);

	Chunk& chunk = chunks[hash_function(chunk_x, chunk_y, chunk_z)];
	if (!(chunk.chunk_x == chunk_x && chunk.chunk_y == chunk_y && chunk.chunk_z == chunk_z))
	{
		std::cout << "Incorrect Indexing in GetBlock_r function\n";
		throw;
	}
	//world space block position(WSBP) to Chunk space block position(CSBP)
	int x = block_x % CHUNK_LENGTH;
	int y = block_y % CHUNK_LENGTH;
	int z = block_z % CHUNK_LENGTH;
	x = x < 0 ? CHUNK_LENGTH + x : x;
	y = y < 0 ? CHUNK_LENGTH + y : y;
	z = z < 0 ? CHUNK_LENGTH + z : z;
	return chunk.get_block_r(x, y, z);
}
inline void ChunkManager::add_block(int chunk_x, int chunk_y, int chunk_z)
{
	//std::cout<<"Created Chunk\n";
	Chunk& chunk = chunks[hash_function(chunk_x, chunk_y, chunk_z)];
	if (chunk.chunk_x == chunk_x && chunk.chunk_y == chunk_y && chunk.chunk_z == chunk_z)
	{
		std::cout << "Chunk already added\n";
		return;
	}
	if (chunk.data == NULL) 
		chunk.data = new Block[CHUNK_SIZE];
	
	chunk.chunk_x = chunk_x;
	chunk.chunk_y = chunk_y;
	chunk.chunk_z = chunk_z;

	int block_x = chunk_x * CHUNK_LENGTH;
	int block_y = chunk_y * CHUNK_LENGTH;
	int block_z = chunk_z * CHUNK_LENGTH;

	for (int y = 0; y < CHUNK_LENGTH; y++)
		for (int z = 0; z < CHUNK_LENGTH; z++)
			for (int x = 0; x < CHUNK_LENGTH; x++)
				chunk.get_block_r(x, y, z).block_type = sinwave_world(block_x + x, block_y + y, block_z + z);
}
inline void ChunkManager::mesh_chunk(int chunk_x, int chunk_y, int chunk_z)
{
	Chunk& chunk = chunks[hash_function(chunk_x, chunk_y, chunk_z)];
	if (!(chunk.chunk_x == chunk_x && chunk.chunk_y == chunk_y && chunk.chunk_z == chunk_z))
	{
		std::cout << "Incorrect chunk indexing in MeshChunk\n";
		throw;
	}
	int block_x = chunk_x * CHUNK_LENGTH;
	int block_y = chunk_y * CHUNK_LENGTH;
	int block_z = chunk_z * CHUNK_LENGTH;
	for (int y = 0; y < CHUNK_LENGTH; y++)
		for (int z = 0; z < CHUNK_LENGTH; z++)
			for (int x = 0; x < CHUNK_LENGTH; x++)
				mesh_block(block_x + x, block_y + y, block_z + z);
}
inline void ChunkManager::mesh_block(int block_x, int block_y, int block_z)
{
	Block& current_block = get_block_r(block_x, block_y, block_z);
	current_block.face[0] = 0;
	//front
	if (get_block(block_x, block_y, block_z - 1) == 0)
		current_block.face[0] = 1;
	//Right
	current_block.face[1] = 0;
	if (get_block(block_x + 1, block_y, block_z) == 0)
		current_block.face[1] = 1;
	//Back
	current_block.face[2] = 0;
	if (get_block(block_x, block_y, block_z + 1) == 0)
		current_block.face[2] = 1;
	//Left
	current_block.face[3] = 0;
	if (get_block(block_x - 1, block_y, block_z) == 0)
		current_block.face[3] = 1;
	//Bottom
	current_block.face[4] = 0;
	if (get_block(block_x, block_y - 1, block_z) == 0)
		current_block.face[4] = 1;
	//Top
	current_block.face[5] = 0;
	if (get_block(block_x, block_y + 1, block_z) == 0)
		current_block.face[5] = 1;
}

inline void ChunkManager::mesh_adjacent_blocks(int block_x, int block_y, int block_z)
{
	for(int y = -1; y <= 1; y++)
		for (int z = -1; z <= 1; z++)
			for (int x = -1; x <= 1; x++)
				if(does_block_exist(block_x + x, block_y + y, block_z+z))
					mesh_block(block_x+x, block_y+y, block_z+z);
}
inline void ChunkManager::place_tree(int x, int y, int z)
{
	for (int i = 0; i < Cube::tree_assemble.size(); i++)
	{
		vec3 p = Cube::tree_blocks[i] + vec3(x, y, z);
		if (does_block_exist(p.x, p.y, p.z))
		{
			get_block_r(p.x, p.y, p.z) = Cube::tree_assemble[i];
			mesh_adjacent_blocks(p.x, p.y, p.z);
		}
	}
}

inline void ChunkManager::render(Camera3D camera, float Brightness, vec3 sun_position)
{
	vec3 sun_dir = sun_position.Normalize();
	float Face_Brightness[] =
	{
		Brightness * ((2 + sun_dir.dot(vec3(0, 0,-1))) / 2),
		Brightness * ((2 + sun_dir.dot(vec3(1, 0, 0))) / 2),
		Brightness * ((2 + sun_dir.dot(vec3(0, 0, 1))) / 2),
		Brightness * ((2 + sun_dir.dot(vec3(-1, 0,0))) / 2),
		Brightness * ((2 + sun_dir.dot(vec3(0,-1, 0))) / 2),
		(Brightness + 0.2) * ((2 + sun_dir.dot(vec3(0, 1, 0))) / 2)
	};

	for (int i = 0; i < map_size; i++)
	{
		Chunk& chunk = chunks[i];
		if (chunk.data == NULL) continue;

		int block_x = chunk.chunk_x * CHUNK_LENGTH;
		int block_y = chunk.chunk_y * CHUNK_LENGTH;
		int block_z = chunk.chunk_z * CHUNK_LENGTH;


		for (int y = 0; y < CHUNK_LENGTH; y++)
			for (int z = 0; z < CHUNK_LENGTH; z++)
				for (int x = 0; x < CHUNK_LENGTH; x++)
				{
					Block& block = chunk.get_block_r(x, y, z);
					if (block.block_type == 0) continue;
					for (int i = 0; i < 6; i++)
					{
						if (block.face[i] == 0) continue;
						vec3 position = vec3(x + block_x, y + block_y, z + block_z);
						vec3 vertices[] =
						{
							Cube::vertice_data[i * 4 + 0],
							Cube::vertice_data[i * 4 + 1],
							Cube::vertice_data[i * 4 + 2],
							Cube::vertice_data[i * 4 + 3]
						};
						Draw3D::plain_uv(position, vertices, Cube::block_type[6 * block.block_type + i], Face_Brightness[i], camera);
					}
				}
	

	}
}
