#pragma once
#include "AsciGraphics/Camera3D.hpp"
#include "ChunkManager.hpp"
#include "CubeMesh.h"
#include <Windows.h>

const vec3 vertices[]
{
	vec3(-1,-1,0),
	vec3(1,-1,0),
	vec3(1,1,0),
	vec3(-1,1,0)
};

struct Player : Camera3D
{

	void CastRay(ChunkManager& world, float FrameTime);
	void Controls() {};
};
inline void Player::CastRay(ChunkManager& world, float FrameTime)
{
	vec3 p = position + vec3(0.5, 0.5, 0.5);
	vec3 v = Direction;
	for (int step = 0; step < 500; step++)
	{
		if (world.GetBlock(p.x, p.y, p.z)) break;
		float dx = v.x >= 0 ? (ceilf(p.x) - p.x) / v.x : (floorf(p.x) - p.x) / v.x;
		float dy = v.y >= 0 ? (ceilf(p.y) - p.y) / v.y : (floorf(p.y) - p.y) / v.y;
		float dz = v.z >= 0 ? (ceilf(p.z) - p.z) / v.z : (floorf(p.z) - p.z) / v.z;
		if (dx < dy && dx < dz)p = p + v * (dx + 0.01);
		if (dy < dx && dy < dz)p = p + v * (dy + 0.01);
		if (dz < dx && dz < dy)p = p + v * (dz + 0.01);
	}
	p = p - v * 0.01;



	for (int i = 0; i < 6; i++)
	{
		vec3 vertices[] =
		{
			Cube::data[i * 4 + 0],
			Cube::data[i * 4 + 1],
			Cube::data[i * 4 + 2],
			Cube::data[i * 4 + 3]
		};
		vec3 pos = p - Direction * 0.02;
		//Draw3D::Plain(vec3(int(pos.x), int(pos.y), int(pos.z)), vertices, ']', Camera3D(position, view, Direction));
	}

	static float place_time = 0;
	place_time += FrameTime;


	//Place Blocks
	if (GetAsyncKeyState(13) & 0x8000 && place_time > 0.2)
	{
		p -= Direction * 0.02;
		if (world.does_chunk_exist(p.x, p.y, p.z))
		{
			world.GetBlock_r(int(p.x), int(p.y), int(p.z)).block_type = 1;
			world.MeshChunk(p.x / CHUNK_LENGTH, p.y / CHUNK_LENGTH, p.z / CHUNK_LENGTH);
		}
		place_time = 0;
	}

	//Break Blocks
	if (GetAsyncKeyState(8) & 0x8000 && place_time > 0.2)
	{
		p += Direction * 0.02;
		if (world.does_chunk_exist(p.x, p.y, p.z))
		{
			world.GetBlock_r(int(p.x), int(p.y), int(p.z)).block_type = 0;
			world.MeshChunk(p.x / CHUNK_LENGTH, p.y / CHUNK_LENGTH, p.z / CHUNK_LENGTH);
		}
		place_time = 0;
	}

}

//vec3 vertices[] =
		//{
		//	Cube::data[i * 4 + 0]*0.5 - vec3(0.5, 0.5, 0.5),
		//	Cube::data[i * 4 + 1]*0.5 - vec3(0.5, 0.5, 0.5),
		//	Cube::data[i * 4 + 2]*0.5 - vec3(0.5, 0.5, 0.5),
		//	Cube::data[i * 4 + 3]*0.5 - vec3(0.5, 0.5, 0.5)
		//};