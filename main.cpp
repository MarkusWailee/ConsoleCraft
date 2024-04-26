#include <iostream>
#include "AsciGraphics/AsciGraphics.h"
#include "ChunkManager.hpp"
#include "Player.hpp"
#include <thread>


int main()
{
	//window initialization
	Terminal3D::Init(430,300,float(4)/2, 'Q');


	//DeltaTime::SetTargetFPS(60);
	Player camera;

	camera.position = vec3(8, 16, 8);


	//Temporary World Generation
	int map_length = 10;
	ChunkManager n(map_length);
	for (int chunk_y = 0; chunk_y < map_length; chunk_y++)
		for (int chunk_z = 0; chunk_z < map_length; chunk_z++)
			for (int chunk_x = 0; chunk_x < map_length; chunk_x++)
				n.AddChunk(chunk_x, chunk_y, chunk_z);

	for (int chunk_y = 0; chunk_y < map_length; chunk_y++)
		for (int chunk_z = 0; chunk_z < map_length; chunk_z++)
			for (int chunk_x = 0; chunk_x < map_length; chunk_x++)
				n.MeshChunk(chunk_x, chunk_y, chunk_z);

	float Global_Brightness = 0;
	//Game Logic
	std::thread([&]
	{
		DeltaTime Game_Time;
		while (true)
		{
			//std::cout << Timer << '\n';
			Game_Time.HandleTime();
			Global_Brightness = (sinf(Game_Time.GetTime() * 0.5) + 1) / 2.0f + 0.5;
			Terminal3D::ChangeBackbuffer(GetGradient(Global_Brightness));
			//Terminal3D::ChangeBackbuffer(' ');
			camera.FreeCam(Game_Time.GetFrameTime());
			camera.CastRay(n, Game_Time.GetFrameTime());
		}
	}).detach();

	//std::cin.get();
	//Rendering Logic
	DeltaTime Render_Time;
	while (true)
	{
		Render_Time.HandleTime();
		Render_Time.ShowFPS();
		n.Render(camera, Global_Brightness);
		Draw::Circle(vec2(Terminal3D::GetScreenWidth()/2, Terminal3D::GetScreenHeight()/2), 1, '#');

		Terminal3D::Render();// <- couts the string
		Terminal3D::ClearBuffer();
	}
	Terminal3D::Terminate();
}

