#include <iostream>
#include "AsciGraphics/AsciGraphics.h"
#include "ChunkManager.hpp"




int main()
{
	//window initialization
	Terminal3D::Init(500,280, ' ');

	//DeltaTime::SetTargetFPS(60);

	float x = 40;
	float y = 0;

	Camera3D camera;


	float time = 0;

	int map_length = 3;

	ChunkManager n(map_length);
	for (int chunk_y = 0; chunk_y < map_length; chunk_y++)
		for (int chunk_z = 0; chunk_z < map_length; chunk_z++)
			for (int chunk_x = 0; chunk_x < map_length; chunk_x++)
				n.AddChunk(chunk_x, chunk_y, chunk_z);

	for (int chunk_y = 0; chunk_y < map_length; chunk_y++)
		for (int chunk_z = 0; chunk_z < map_length; chunk_z++)
			for (int chunk_x = 0; chunk_x < map_length; chunk_x++)
				n.MeshChunk(chunk_x, chunk_y, chunk_z);

	while (true)
	{
		DeltaTime::HandleTime();
		DeltaTime::ShowFPS();

		time += 0.5*DeltaTime::GetFrameTime();
		camera.Update();



		//Demo
		n.Render(camera);
		
		//rendering to the string



		Terminal3D::Render();// <- couts the string
		Terminal3D::ClearBuffer();
	}
	Terminal3D::Terminate();
}

