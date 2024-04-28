#include <iostream>
#include "AsciGraphics/AsciGraphics.h"
#include "ChunkManager.hpp"
#include "Player.hpp"
#include <thread>


int main()
{
	//window initialization
	Terminal3D::Init(460,320,float(4)/2.1, 'Q');
	std::cout << "\033[1;37m";//white
	//Terminal3D::Init(900, 480, float(4) / 2.1, 'Q');

	Player camera;

	camera.position = vec3(8, 32, 8);


	//Temporary World Generation
	int map_length = 10;
	int offset = 0;
	ChunkManager n(map_length);
	for (int chunk_y = - offset; chunk_y < map_length - offset; chunk_y++)
		for (int chunk_z = -offset; chunk_z < map_length - offset; chunk_z++)
			for (int chunk_x = -offset; chunk_x < map_length - offset; chunk_x++)
				n.AddChunk(chunk_x, chunk_y, chunk_z);

	for (int chunk_y = - offset; chunk_y < map_length - offset; chunk_y++)
		for (int chunk_z = -offset; chunk_z < map_length - offset; chunk_z++)
			for (int chunk_x = -offset; chunk_x < map_length - offset; chunk_x++)
				n.MeshChunk(chunk_x, chunk_y, chunk_z);

	n.PlaceTree(15, get_height_map(15, 40), 40);

	float Global_Brightness = 0;
	vec3 sun_pos = vec3(1,1,1);


	//Game Logic


	std::thread([&]
	{
		DeltaTime Game_Time;
		Game_Time.HandleTime();
		while (true)
		{
			Game_Time.HandleTime();
			Global_Brightness = (sun_pos.y + 50 )/100 + 0.1;
			sun_pos = mat::GetRotY(0.5) * mat::GetRotX(1) * vec3(10, 0, -50);
			Terminal3D::ChangeBackbuffer(GetGradient(Global_Brightness*0.8));

			//Terminal3D::ChangeBackbuffer(' ');
			camera.controls(Game_Time.GetFrameTime());
			camera.cast_ray(n, Game_Time.GetFrameTime());
			camera.world_collision(n);
		}
	}).detach();

	//Rendering Logic
	DeltaTime Render_Time;
	while (true)
	{
		Render_Time.HandleTime();
		//Render_Time.ShowFPS();
		n.Render(camera, Clampf(Global_Brightness, 0.4, 0.60), sun_pos);
		Draw::Circle(vec3(0,0,0), 1, ' ');

		
		Draw3D::Sun(sun_pos, 7, 1, camera);

		Terminal3D::Render();
		Terminal3D::ClearBuffer();
	}
	Terminal3D::Terminate();
}

