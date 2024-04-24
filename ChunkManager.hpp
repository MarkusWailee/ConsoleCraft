#pragma once
#include "AsciGraphics/AsciGraphics.h"
#include "GenerateFunctions.h"
#include "CubeMesh.h"
/* chunk_x  (World space Chunk position) World Chunk Position(WSCP)
*  block_x  (World space block position) World Block Position(WSBP)
*  x		(Chunk space block position) Chunk Block Position(CSBP)
*/ 



char _ = '.';
char O = ']';
char G = '-';
char tex_H[] = //16x16
{
	G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,_,_,_,_,_,_,_,_,_,_,_,_,_,_,G,
	G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,G,
};

const int CHUNK_LENGTH = 16;
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
	Block& GetBlock_r(int x, int y, int z)
	{
		if (x < 0 || x >= CHUNK_LENGTH || y < 0 || y >= CHUNK_LENGTH || z < 0 || z >= CHUNK_LENGTH || data == NULL)
		{
			std::cout << "Incorrect Chunk Indexing in reference function\n";
			throw;
		}
		return data[x + z * CHUNK_LENGTH + y * CHUNK_LENGTH * CHUNK_LENGTH];
	}
	unsigned char GetBlock(int x, int y, int z)
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
		Terminal3D::Add_Texture(1, tex_H, 16, 16);
		chunks = new Chunk[map_size];
		//for (int i = 0; i < 16 * 16 - 3; i += rand() % 3 + 1)
		//{
		//	int x = i % 16;
		//	int y = i / 16;
		//	if(x>0 && x< 16 && y>0 && y < 15)
		//		tex_H[i] = ',';
		//}
	}
	~ChunkManager() { delete[] chunks; }
	unsigned int HashFunction(int chunk_x, int chunk_y, int chunk_z);
	unsigned char GetBlock(int x, int y, int z);
	//r for reference
	Block& GetBlock_r(int x, int y, int z);
	void AddChunk(int chunk_x, int chunk_y, int chunk_z);
	void MeshChunk(int chunk_x, int chunk_y, int chunk_z);
	void Render(Camera3D camera);
};

//unique identifier for each chunk
inline unsigned int ChunkManager::HashFunction(int chunk_x, int chunk_y, int chunk_z)
{
	return ((chunk_x + chunk_z * map_length + chunk_y * map_length * map_length) + 536870911) % map_size;
}
inline unsigned char ChunkManager::GetBlock(int block_x, int block_y, int block_z)
{
	//world space block position(WSBP) to world space chunk position(WSCP).			 This code was to support negative block positions
	int chunk_x = (block_x / CHUNK_LENGTH) - (block_x % CHUNK_LENGTH != 0 && block_x < 0);
	int chunk_y = (block_y / CHUNK_LENGTH) - (block_y % CHUNK_LENGTH != 0 && block_y < 0);
	int chunk_z = (block_z / CHUNK_LENGTH) - (block_z % CHUNK_LENGTH != 0 && block_z < 0);

	Chunk& chunk = chunks[HashFunction(chunk_x, chunk_y, chunk_z)];
	if (!(chunk.chunk_x == chunk_x && chunk.chunk_y == chunk_y && chunk.chunk_z == chunk_z))
	{
		//return air block. Helps for meshing when the chunk DNE.
		return 0;
	}
	//world space block position(WSBP) to Chunk space block position(CSBP)
	int x = block_x % CHUNK_LENGTH;
	int y = block_y % CHUNK_LENGTH;
	int z = block_z % CHUNK_LENGTH;
	return chunk.GetBlock(x, y, z);
}
inline Block& ChunkManager::GetBlock_r(int block_x, int block_y, int block_z)
{
	//world space block position(WSBP) to world space chunk position(WSCP).			 This code was to support negative block positions
	int chunk_x = (block_x / CHUNK_LENGTH) - (block_x % CHUNK_LENGTH != 0 && block_x < 0);
	int chunk_y = (block_y / CHUNK_LENGTH) - (block_y % CHUNK_LENGTH != 0 && block_y < 0);
	int chunk_z = (block_z / CHUNK_LENGTH) - (block_z % CHUNK_LENGTH != 0 && block_z < 0);

	Chunk& chunk = chunks[HashFunction(chunk_x, chunk_y, chunk_z)];
	if (chunk.chunk_x == chunk_x && chunk.chunk_y == chunk_y && chunk.chunk_z == chunk_z)
	{
		std::cout << "Incorrect Indexing in GetBlock_r function\n";
		throw;
	}
	//world space block position(WSBP) to Chunk space block position(CSBP)
	int x = block_x % CHUNK_LENGTH;
	int y = block_y % CHUNK_LENGTH;
	int z = block_z % CHUNK_LENGTH;
	return chunk.GetBlock_r(x, y, z);
}
inline void ChunkManager::AddChunk(int chunk_x, int chunk_y, int chunk_z)
{
	//std::cout<<"Created Chunk\n";
	Chunk& chunk = chunks[HashFunction(chunk_x, chunk_y, chunk_z)];
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
				chunk.GetBlock_r(x, y, z).block_type = sinwave_world(block_x + x, block_y + y, block_z + z);
}
inline void ChunkManager::MeshChunk(int chunk_x, int chunk_y, int chunk_z)
{
	Chunk& chunk = chunks[HashFunction(chunk_x, chunk_y, chunk_z)];
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
			{
				Block& current_block = chunk.GetBlock_r(x, y, z);

				//Front
				current_block.face[0] = 0;
				if (GetBlock(block_x + x, block_y + y, block_z + z - 1) == 0)
					current_block.face[0] = 1;
				//Right
				current_block.face[1] = 0;
				if (GetBlock(block_x + x + 1, block_y + y, block_z + z) == 0)
					current_block.face[1] = 1;
				//Back
				current_block.face[2] = 0;
				if (GetBlock(block_x + x, block_y + y, block_z + z + 1) == 0)
					current_block.face[2] = 1;
				//Left
				current_block.face[3] = 0;
				if (GetBlock(block_x + x - 1, block_y + y, block_z + z) == 0)
					current_block.face[3] = 1;
				//Bottom
				current_block.face[4] = 0;
				if (GetBlock(block_x + x, block_y + y - 1, block_z + z) == 0)
					current_block.face[4] = 1;
				//Top
				current_block.face[5] = 0;
				if (GetBlock(block_x + x, block_y + y + 1, block_z + z) == 0)
					current_block.face[5] = 1;
			}
}

void ChunkManager::Render(Camera3D camera)
{
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
					for (int i = 0; i < 6; i++)
					{
						Block& block = chunk.GetBlock_r(x, y, z);
						if (block.face[i] == 0 || block.block_type == 0) continue;
						vec3 position = vec3(x + block_x, y + block_y, z + block_z);
						vec3 vertices[] =
						{
							Cube::data[i * 4 + 0],
							Cube::data[i * 4 + 1],
							Cube::data[i * 4 + 2],
							Cube::data[i * 4 + 3]
						};
						Draw3D::Plain_uv(position, vertices, 1, camera);
					}
	

	}
}
