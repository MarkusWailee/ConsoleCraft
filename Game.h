#pragma once
#include "thread"
#include "ChunkManager.hpp"
#include "Player.hpp"
#include <queue>

int myrand(int seed) {
	seed = seed * 1103515245 + 12345;
	return((unsigned)(seed / 65536) % 32768);
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
		player.position = vec3(32, get_height_map(32, 32) + 1, 32);
	}

	void generate_chunks()
	{
		vec3i player_chunk_position = vec3i(
			get_chunk_position(float_to_int(player.position.x)),
			get_chunk_position(float_to_int(player.position.y)),
			get_chunk_position(float_to_int(player.position.z)));
		int gen_offset = render_distance / 2;
		std::vector<vec3i> chunks_added;
		int count = 0;
		if (generation_position != player_chunk_position)
		{
			generation_position = player_chunk_position;
			for (int chunk_y = generation_position.y - gen_offset; chunk_y < generation_position.y + gen_offset; chunk_y++)
				for (int chunk_z = generation_position.z - gen_offset; chunk_z < generation_position.z + gen_offset; chunk_z++)
					for (int chunk_x = generation_position.x - gen_offset; chunk_x < generation_position.x + gen_offset; chunk_x++)
					{
						if (chunk_manager.add_block(chunk_x, chunk_y, chunk_z))
						{
							chunks_added.push_back(vec3i(chunk_x, chunk_y, chunk_z));
						}

					}
			for (int i = 0; i < chunks_added.size(); i++)
			{
				vec3i pos = chunks_added[i];
				chunk_manager.mesh_chunk(pos.x, pos.y, pos.z);
				int block_x = pos.x * CHUNK_LENGTH;
				int block_z = pos.y * CHUNK_LENGTH; 
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
			game_time.HandleTime();
			global_brightness = (sun_position.y + 50) / 100 + 0.1;
			sun_position = mat::GetRotY(0.5) * mat::GetRotX(world_time) * vec3(10, 0, -50);
			//sun_position = mat::GetRotY(0.5) * mat::GetRotX(game_time.GetTime() * 0.2) * vec3(10, 0, -50);
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
				chunk_manager.render(player, clampf(global_brightness, 0.4, 0.60), sun_position);
				Draw::circle(vec3(0, 0, 0), 1, ' ');
				Draw3D::sun(sun_position, 7, 1, player);

				Terminal3D::render();
				Terminal3D::clear_buffer();
			}
	}
private:
	bool is_game_running = 1;
	float world_time = 1;
	int render_distance = 0;
	vec3 sun_position = vec3(1,1,1);
	float global_brightness = 0;
	ChunkManager chunk_manager;
	Player player;

	vec3i generation_position;
};