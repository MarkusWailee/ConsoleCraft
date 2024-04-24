#pragma once
#include "AsciGraphics/AsciGraphics.h"
#include "GenerateFunctions.h"
#include "CubeMesh.h"
/* chunk_x  (World space Chunk position)
*  block_x  (World space block position)
*  x		(Chunk space block position)
*/ 



const int CHUNK_LENGTH = 16;
const int CHUNK_SIZE = CHUNK_LENGTH * CHUNK_LENGTH * CHUNK_LENGTH;

struct Block
{
	//specifies which face to render
	bool face[6] = {}; 
	unsigned char block_type = 0;
};

struct Chunk
{
	Block* data = NULL;
	int chunk_x = INT_MAX, chunk_y = INT_MAX, chunk_z = INT_MAX;
	Block& GetBlock(int x, int y, int z)
	{
		if (x < 0 || x >= CHUNK_LENGTH || y < 0 || y >= CHUNK_LENGTH || z < 0 || z >= CHUNK_LENGTH || data == NULL)
		{
			std::cout << "Incorrect Chunk Indexing\n";
			throw;
		}
		return data[x + z * CHUNK_LENGTH + y * CHUNK_LENGTH * CHUNK_LENGTH];
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
	ChunkManager(int distance) : map_length(distance), map_size(distance* distance* distance) , chunks(new Chunk[map_size]) {}
	~ChunkManager() { delete[] chunks; }
	unsigned int HashFunction(int chunk_x, int chunk_y, int chunk_z);
	void AddChunk(int chunk_x, int chunk_y, int chunk_z);
	void Render(Camera3D camera);
};

//unique identifier for each chunk
inline unsigned int ChunkManager::HashFunction(int chunk_x, int chunk_y, int chunk_z)
{
	return ((chunk_x + 715827882) + (chunk_z + 357913941) * map_length + (chunk_y + 178956970) * map_length * map_length) % map_size;
}
inline void ChunkManager::AddChunk(int chunk_x, int chunk_y, int chunk_z)
{
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
				chunk.GetBlock(x, y, z).block_type = sinwave_world(block_x + x, block_y + y, block_z + z);
}

void ChunkManager::Render(Camera3D camera)
{
	for (int i = 0; i < map_size; i++)
	{
		Chunk& chunk = chunks[i];
		if (chunk.data == NULL) continue;

		int x = chunk.chunk_x * CHUNK_LENGTH;
		int y = chunk.chunk_y * CHUNK_LENGTH;
		int z = chunk.chunk_z * CHUNK_LENGTH;

		for (int i = 0; i < 6; i++)
		{
			vec3 vertices[] =
			{
				Cube::data[i * 4 + 0],
				Cube::data[i * 4 + 1],
				Cube::data[i * 4 + 2],
				Cube::data[i * 4 + 3]
			};
			Draw3D::Plain(vec3(x, y, z), vertices, 'p', camera);
		}

	}
}
