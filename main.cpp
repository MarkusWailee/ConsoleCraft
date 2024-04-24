#include <iostream>
#include "AsciGraphics/AsciGraphics.h"
#include "ChunkManager.hpp"




int main()
{
	//window initialization
	Terminal3D::Init(500,280, '.');

	//DeltaTime::SetTargetFPS(60);

	float x = 40;
	float y = 0;

	Camera3D camera;


	float time = 0;


	ChunkManager n(1);

	n.AddChunk(0, 0, 0);


	const vec3 RIGHT_FACE[] =
	{
		vec3(0.5,-0.5,-0.5),
		vec3(0.5,-0.5,0.5),
		vec3(0.5,0.5,0.5),
		vec3(0.5,0.5,-0.5)
	};
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

