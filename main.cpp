#include <iostream>
#include "Game.h"
#include <thread>


int main()
{
	//window initialization
	Terminal3D::init(460,320,float(4)/2.1, 'Q');
	std::cout << "\033[1;37m";//white
	//Terminal3D::init(200, 160, float(4) / 2.1, 'Q');

	Game game(15);
	//game.generate_chunks();
	std::thread([&]
		{
			while(true)
				game.generate_chunks();
		}).detach();
	

	std::thread([&]
		{
			game.run_physics();
		}).detach();


	game.render_game();

	Terminal3D::terminate();
}

