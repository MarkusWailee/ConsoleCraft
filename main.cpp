#include <iostream>
#include "AsciGraphics/AsciGraphics.h"
#include "ChunkManager.hpp"
#include "Player.hpp"
#include <thread>


int main()
{
	//window initialization
	Terminal3D::Init(430,320, ' ');


	//DeltaTime::SetTargetFPS(60);
	Player camera;




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





	//Game Logic
	std::thread([&]
	{
		DeltaTime Game_Time;
		while (true)
		{
			Game_Time.HandleTime();
			camera.FreeCam(Game_Time.GetFrameTime());
			camera.CastRay(n, Game_Time.GetFrameTime());
		}
	}).detach();

	//Rendering Logic
	DeltaTime Render_Time;
	while (true)
	{
		Render_Time.HandleTime();
		Render_Time.ShowFPS();

		n.Render(camera);
		Draw::Circle(vec2(Terminal3D::GetScreenWidth()/2, Terminal3D::GetScreenHeight()/2), 1, '#');

		Terminal3D::Render();// <- couts the string
		Terminal3D::ClearBuffer();
	}
	Terminal3D::Terminate();
}

