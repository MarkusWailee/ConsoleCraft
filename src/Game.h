#pragma once
#include "thread"
#include "ChunkManager.hpp"
#include "Player.hpp"
#include <queue>

int myrand(int seed) {
	const int a = 1664525;
	const int c = 1013904223;
	seed = (a * seed + c) & 0x7fffffff; 
	return seed;
}

struct vec3i 
{
	int x = 0, y = 0, z = 0;
	vec3i(){}
	vec3i(int x, int y, int z) : x(x), y(y), z(z){}
	vec3i(const vec3& v) : x(v.x), y(v.y), z(v.z){}
	bool operator==(const vec3i& vi){
		return vi.x == x && vi.y == y && vi.z == z;
	}
	bool operator!=(const vec3i& vi) {
		return !(vi.x == x && vi.y == y && vi.z == z);
	}
};

class Game
{
public:
	Game(int render_distance) :render_distance(render_distance), chunk_manager(render_distance)
	{
		player.position = vec3(32, get_height_map(32, 32) + 2, 32);
	}

	void generate_chunks()
	{
		//THIS WAS TEMPORARY DONT JUDGE
		vec3i player_chunk_position = vec3i(
			get_chunk_position(float_to_int(player.position.x)),
			get_chunk_position(float_to_int(player.position.y)),
			get_chunk_position(float_to_int(player.position.z)));
		int gen_offset = render_distance / 2;
		if (generation_position != player_chunk_position)
		{
			generation_position = player_chunk_position;
			for (int chunk_y = generation_position.y - gen_offset; chunk_y < generation_position.y + gen_offset; chunk_y++)
				for (int chunk_z = generation_position.z - gen_offset; chunk_z < generation_position.z + gen_offset; chunk_z++)
					for (int chunk_x = generation_position.x - gen_offset; chunk_x < generation_position.x + gen_offset; chunk_x++)
						chunk_manager.add_block(chunk_x, chunk_y, chunk_z);
			for (int chunk_y = generation_position.y - gen_offset; chunk_y < generation_position.y + gen_offset; chunk_y++)
				for (int chunk_z = generation_position.z - gen_offset; chunk_z < generation_position.z + gen_offset; chunk_z++)
					for (int chunk_x = generation_position.x - gen_offset; chunk_x < generation_position.x + gen_offset; chunk_x++)
						chunk_manager.mesh_chunk(chunk_x, chunk_y, chunk_z);
			for (int chunk_z = generation_position.z - gen_offset; chunk_z < generation_position.z + gen_offset; chunk_z++)
				for (int chunk_x = generation_position.x - gen_offset; chunk_x < generation_position.x + gen_offset; chunk_x++)
				{
					int block_x = chunk_x * CHUNK_LENGTH + myrand(myrand(chunk_x) + chunk_z * render_distance) % CHUNK_LENGTH;
					int block_z = chunk_z * CHUNK_LENGTH + myrand(myrand(chunk_z) + chunk_x * render_distance) % CHUNK_LENGTH;
					if(!(myrand(myrand(block_x) + myrand(block_z)) % 2))
					chunk_manager.place_tree(block_x, get_height_map(block_x, block_z), block_z);
				}

		}
	}

	void run_physics()
	{
		DeltaTime game_time;
		game_time.HandleTime();
		while (is_game_running)
		{
			if (day_time > 209.5) day_time = 0;
			day_time += game_time.GetFrameTime();
			if (GetAsyncKeyState('G') & 0x8000)
				day_time += 20 * game_time.GetFrameTime();
			game_time.HandleTime();
			global_brightness = (sun_position.y + 50) / 100 + 0.1;
			//sun_position = mat::GetRotY(0.5) * mat::GetRotX(world_time) * vec3(10, 0, -50);
			sun_position = mat::GetRotY(0.5) * mat::GetRotX(day_time * 0.03) * vec3(10, 0, -50);
			Terminal3D::change_background(get_asci_gradient(global_brightness * 0.8));
			player.controls(game_time.GetFrameTime());
			player.cast_ray(chunk_manager, game_time.GetFrameTime());
			player.world_collision(chunk_manager);
		}
	}

	void render_game()
	{
			//Rendering Logic
			DeltaTime Render_Time;
			while (is_game_running)
			{
				Render_Time.HandleTime();
				Render_Time.ShowFPS();
				Draw::circle(vec3(0, 0, 0), 1, ' ');
				Draw3D::sun(sun_position, 14, 1, player);


				//Render clouds
				for (int z = - render_distance; z < + render_distance; z++)
					for (int x = - render_distance; x < + render_distance; x++)
					{
						int block_x = generation_position.x - x;
						int block_z = generation_position.z - z;
						float blk_x = block_x * CHUNK_LENGTH + (myrand(myrand(block_x) + myrand(block_z)) % CHUNK_LENGTH);
						float blk_z = block_z * CHUNK_LENGTH + (myrand(myrand(block_z) + myrand(block_x)) % CHUNK_LENGTH);
						if(!(myrand(myrand(block_x) + myrand(block_z * 100)) % 6))
							Draw3D::cube(vec3(blk_x , 60 + myrand(myrand(block_x) + myrand(block_z + block_z) ) % 30, blk_z), 5, clampf(0.1 + global_brightness, 0, 0.99), player);
					}
				chunk_manager.render(player, clampf(global_brightness, 0.4, 0.60), sun_position);

				Terminal3D::render();
				Terminal3D::clear_buffer();
			}
	}
private:
	float day_time = 0;
	bool is_game_running = 1;
	float world_time = 1;
	int render_distance = 0;
	vec3 sun_position = vec3(1,1,1);
	float global_brightness = 0;
	ChunkManager chunk_manager;
	Player player;

	vec3i generation_position;
};