#include <iostream>
#include "AsciGraphics/AsciGraphics.h"
#include "ChunkManager.hpp"
#include "Player.hpp"
#include <thread>


int main()
{
	//window initialization
	Terminal3D::Init(800,480, ' ');


	//DeltaTime::SetTargetFPS(60);
	Player camera;



	float time = 0;

	int map_length = 5;

	ChunkManager n(map_length);
	for (int chunk_y = 0; chunk_y < map_length; chunk_y++)
		for (int chunk_z = 0; chunk_z < map_length; chunk_z++)
			for (int chunk_x = 0; chunk_x < map_length; chunk_x++)
				n.AddChunk(chunk_x, chunk_y, chunk_z);

	for (int chunk_y = 0; chunk_y < map_length; chunk_y++)
		for (int chunk_z = 0; chunk_z < map_length; chunk_z++)
			for (int chunk_x = 0; chunk_x < map_length; chunk_x++)
				n.MeshChunk(chunk_x, chunk_y, chunk_z);

	//physics
	//std::thread([&]
	//	{
	//		while (true)
	//		{
	//			camera.FreeCam();
	//			DeltaTime::HandleTime();
	//		}
	//	}).detach();

	//Rendering
	while (true)
	{
		DeltaTime::HandleTime();
		DeltaTime::ShowFPS();
		camera.FreeCam();


		n.Render(camera);
		Draw::Circle(vec2(Terminal3D::GetScreenWidth()/2, Terminal3D::GetScreenHeight()/2), 1, '#');
		camera.CastRay(n);

		Terminal3D::Render();// <- couts the string
		Terminal3D::ClearBuffer();
	}
	Terminal3D::Terminate();
}

