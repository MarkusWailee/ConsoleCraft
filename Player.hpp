#pragma once
#include "AsciGraphics/Camera3D.hpp"
#include "ChunkManager.hpp"
#include "CubeMesh.h"

const vec3 vertices[]
{
	vec3(-1,-1,0),
	vec3(1,-1,0),
	vec3(1,1,0),
	vec3(-1,1,0)
};

struct Player : Camera3D
{

	void CastRay(ChunkManager& world);
	void Controls() {};
};

/*

inline void Player::CastRay(ChunkManager& world)
{
	vec3 p = position + vec3(0.3,0.3,0.3);
	vec3 v = Direction;
	for(int i = 0; i<100; i ++)
	{
		if (world.GetBlock(p.x, p.y, p.z) != 0)
		{
			p -= Direction*1.5;
			//Draw3D::Plain(vec3(int(p.x), int(p.y), int(p.z)), Cube::TOP_FACE, ']', Camera3D(position, view, Direction));
			for (int i = 0; i < 6; i++)
			{
				vec3 vertices[] =
				{
					Cube::data[i * 4 + 0],
					Cube::data[i * 4 + 1],
					Cube::data[i * 4 + 2],
					Cube::data[i * 4 + 3]
				};
				Draw3D::Plain_uv(p, vertices, 1, Camera3D(position, view, Direction));
			}
		}
		float dx = v.x >= 0 ? (int(p.x) + 1 - p.x) / v.x : (int(p.x) - 1 - p.x) / v.x;
		float dy = v.y >= 0 ? (int(p.y) + 1 - p.y) / v.y : (int(p.y) - 1 - p.y) / v.y;
		float dz = v.z >= 0 ? (int(p.z) + 1 - p.z) / v.z : (int(p.z) - 1 - p.z) / v.z;
		//p = dx < dy ? p + v * dx : p + v * dy;
		if (dx < dy && dx < dz) p = p + v * dx;
		else if (dy < dx && dy < dz) p = p + v * dy;
		else p = p + v * dz;
	}

}

*/
inline void Player::CastRay(ChunkManager& world)
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
			Cube::data[i * 4 + 0]*0.5 - vec3(0.5, 0.5, 0.5),
			Cube::data[i * 4 + 1]*0.5 - vec3(0.5, 0.5, 0.5),
			Cube::data[i * 4 + 2]*0.5 - vec3(0.5, 0.5, 0.5),
			Cube::data[i * 4 + 3]*0.5 - vec3(0.5, 0.5, 0.5)
		};
		Draw3D::Plain(p, vertices, ']', Camera3D(position, view, Direction));
	}
}

