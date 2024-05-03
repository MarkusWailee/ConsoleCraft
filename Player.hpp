#pragma once
#include "ChunkManager.hpp"
#include "CubeMesh.h"
#include <Windows.h>
#include "AABB.hpp"

int float_to_int(float n)
{
	if (n < 0) return int(n - 1);
	return n;
}

struct Player : Camera3D
{

	void cast_ray(ChunkManager& world, float FrameTime);
	void world_collision(ChunkManager& world);
	void controls(float FrameTime);
private:
	bool is_flying = 0;
	bool on_ground = 0;
	vec3 velocity;
	vec2 view_velocity;
	unsigned char block_selected = 1;
};


inline void Player::cast_ray(ChunkManager& world, float FrameTime)
{
	vec3 p = position + vec3(0.5, 0.5, 0.5);
	vec3 v = Direction;
	for (int step = 0; step < 400; step++)
	{
		if (world.get_block(float_to_int(p.x), float_to_int(p.y), float_to_int(p.z))) break;
		float dx = v.x >= 0 ? (ceilf(p.x) - p.x) / v.x : (floorf(p.x) - p.x) / v.x;
		float dy = v.y >= 0 ? (ceilf(p.y) - p.y) / v.y : (floorf(p.y) - p.y) / v.y;
		float dz = v.z >= 0 ? (ceilf(p.z) - p.z) / v.z : (floorf(p.z) - p.z) / v.z;
		if (dx < dy && dx < dz)p = p + v * (dx + 0.01);
		if (dy < dx && dy < dz)p = p + v * (dy + 0.01);
		if (dz < dx && dz < dy)p = p + v * (dz + 0.01);
	}
	p = p - v * 0.01;
	static float place_time = 0;
	place_time += FrameTime;
	//Place Blocks
	if (GetAsyncKeyState(13) & 0x8000 && place_time > 0.2)
	{
		p -= Direction * 0.02;
		int p_x = float_to_int(p.x);
		int p_y = float_to_int(p.y);
		int p_z = float_to_int(p.z);
		if (world.does_block_exist(p_x, p_y, p_z))
		{
			world.get_block_r(int(p_x), int(p_y), int(p_z)).block_type = block_selected;
			world.mesh_adjacent_blocks(p_x, p_y, p_z);
		}
		place_time = 0;
	}

	//Break Blocks
	if (GetAsyncKeyState(8) & 0x8000 && place_time > 0.2)
	{
		p += Direction * 0.02;
		int p_x = float_to_int(p.x);
		int p_y = float_to_int(p.y);
		int p_z = float_to_int(p.z);
		if (world.does_block_exist(p_x, p_y, p_z))
		{
			world.get_block_r(p_x, p_y, p_z).block_type = 0;
			world.mesh_adjacent_blocks(p_x, p_y, p_z);
		}
		place_time = 0;
	}
}

inline void Player::world_collision(ChunkManager& world)
{
	AABB player_AABB(position - vec3(0, 1, 0), 0.8f, 1.8f);
	int p_x = float_to_int(position.x);
	int p_y = float_to_int(position.y);
	int p_z = float_to_int(position.z);
	for (int y = p_y - 1; y <= p_y + 1; y++)
		for (int z = p_z - 1; z <= p_z + 1; z++)
			for (int x = p_x - 1; x <= p_x + 1; x++)
			{
				Block block = world.get_block(x, y, z);
				if (block.block_type == 0) continue;
				AABB block_AABB(vec3(x, y, z), 1);
				if (player_AABB.Collision_test(block_AABB))
				{
					int face_hit = -1;
					player_AABB.p = player_AABB.Collide(block_AABB, face_hit);
					switch (face_hit)
					{
					case 0: velocity.z = 0; break;
					case 1: velocity.x = 0; break;
					case 2: velocity.z = 0; break;
					case 3: velocity.x = 0; break;
					case 5: 
						velocity.y = 0; 
						on_ground = 1;
						break;
					}
				}
			}
	position = player_AABB.p + vec3(0,1,0);
}


inline void Player::controls(float FrameTime)
{
	vec2 movement_direction = vec2(0, 0);
	float movement_y = 0;

	float speed =6;
	float acceleration = 50;
	float sensitivity = 2;
	float friction = 8;
	mat3 RotY =
	{
		cosf(-view.x), 0, sinf(-view.x),
		0,1,0,
		-sinf(-view.x), 0 , cosf(-view.x),
	};
	mat3 RotX =
	{
		1,0,0,
		 0,cosf(-view.y), -sinf(-view.y),
		 0,sinf(-view.y), cosf(-view.y),
	};
	Direction = RotY * RotX * vec3(0, 0, 1);
	//Block Selection
	if (GetAsyncKeyState(49) & 0x8000)
		block_selected = 1;
	if (GetAsyncKeyState(50) & 0x8000)
		block_selected = 2;
	if (GetAsyncKeyState(51) & 0x8000)
		block_selected = 3;
	if (GetAsyncKeyState(52) & 0x8000)
		block_selected = 4;
	if (GetAsyncKeyState(53) & 0x8000)
		block_selected = 5;
	if (GetAsyncKeyState(54) & 0x8000)
		block_selected = 6;
	if (GetAsyncKeyState(55) & 0x8000)
		block_selected = 7;


	//Camera movement windows users only
	if (GetAsyncKeyState('W') & 0x8000)
		movement_direction += vec2(-sin(view.x), cos(view.x));
	if (GetAsyncKeyState('A') & 0x8000)
		movement_direction -= vec2(cos(view.x), sin(view.x));
	if (GetAsyncKeyState('D') & 0x8000)
		movement_direction += vec2(cos(view.x), sin(view.x));
	if (GetAsyncKeyState('S') & 0x8000)
		movement_direction -= vec2(-sin(view.x), cos(view.x));
	if (movement_direction.mag() > 1)
		movement_direction = movement_direction.Normalize();
	if (GetAsyncKeyState('C') & 0x8000)
		velocity.y = -5;
	if (GetAsyncKeyState('F') & 0x8000)
		is_flying = 1;
	if (GetAsyncKeyState('X') & 0x8000)
		is_flying = 0;
	if (GetAsyncKeyState(' ') & 0x8000 && (on_ground || is_flying))
	{
			velocity.y = 6;
			on_ground = 0;
	}
	if (GetAsyncKeyState(37) & 0x8000)
		view_velocity.x += sensitivity * FrameTime;
	if (GetAsyncKeyState(39) & 0x8000)
		view_velocity.x -= sensitivity * FrameTime;
	if (GetAsyncKeyState(40) & 0x8000)
		view_velocity.y -= sensitivity * FrameTime;
	if (GetAsyncKeyState(38) & 0x8000)
		view_velocity.y += sensitivity * FrameTime;
	

	if (is_flying)
	{
		if (velocity.x * velocity.x + velocity.z * velocity.z < speed * speed)
		{
			velocity.x += acceleration * 0.2 * movement_direction.x * FrameTime;
			velocity.z += acceleration * 0.2 * movement_direction.y * FrameTime;
		}
		velocity.y -= friction *velocity.y * FrameTime;
		velocity.x -= friction * 0.2 * velocity.x * FrameTime;
		velocity.z -= friction * 0.2 * velocity.z * FrameTime;
		position += velocity * FrameTime;

		view += view_velocity * FrameTime;
		view_velocity -= view_velocity * FrameTime;
	}
	else
	{
		if (velocity.x * velocity.x + velocity.z * velocity.z < speed * speed)
		{
			velocity.x += acceleration * movement_direction.x * FrameTime;
			velocity.z += acceleration * movement_direction.y * FrameTime;
		}
		velocity.y -= 15 * FrameTime;
		velocity.x -= friction * velocity.x * FrameTime;
		velocity.z -= friction * velocity.z * FrameTime;
		position += velocity * FrameTime;

		view += 25 * view_velocity * FrameTime;
		view_velocity -= 25 * view_velocity * FrameTime;
	}



}