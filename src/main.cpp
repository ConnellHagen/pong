#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Math.hpp"

#include "Entity.hpp"
	#include "Paddle.hpp"
	#include "Barrier.hpp"
	#include "Ball.hpp"
#include "Tile.hpp"
#include "Game.hpp"

#include "GUI.hpp"
#include "Scene.hpp"

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_Init has failed. Error " << SDL_GetError() << "\n";
	
	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";

	if(TTF_Init() != 0)
		std::cout << "TTF_Init has failed. Error: " << SDL_GetError() << "\n";


	RenderWindow window("Pong", utils::display_width(), utils::display_height(), Vector2f(1, 1));

	SDL_Rect canvas_inf = {0, 0, utils::display_width(), utils::display_height()};
	Entity canvas(Vector2f(0, 0), Vector2f(1, 1), NULL, canvas_inf, canvas_inf, 1);

	//W, S, UP, DOWN, Left Click, Right Click
	std::vector<bool> key_pushes(6, false);
	Vector2i mouse_coords;

	int current_scene = 0;


	//scene 0
	Scene main_menu(window, GUI(window, 1));
	//scene 1
	Game current_game(1, 5, window);
	//scene 2
	Scene end_screen(window, GUI(window, 4));


	bool game_running = true;

	SDL_Event event;

	const float time_step = .016666;
	float accumulator = 0.0f;
	float current_time = utils::hire_time_in_seconds();

	bool updated = false;

	int last_frame_ticks = SDL_GetTicks();

	while(game_running)
	{
		float new_time = utils::hire_time_in_seconds();
		float loop_time = new_time - current_time;
		current_time = new_time;
		accumulator += loop_time;

		while(accumulator >= time_step)
		{
			while(SDL_PollEvent(&event))
			{
				switch(event.type)
				{
					case SDL_QUIT:
						game_running = false;
						break;

					case SDL_KEYDOWN:
						switch (event.key.keysym.sym)
						{
						    case SDLK_w:
						    	key_pushes[0] = true;
						    	break;

						    case SDLK_s:
						    	key_pushes[1] = true;
						    	break;

						    case SDLK_UP:
							    key_pushes[2] = true;
							    break;

						    case SDLK_DOWN:
							    key_pushes[3] = true;
							    break;
						}
						break;

					case SDL_KEYUP:
						switch (event.key.keysym.sym)
						{
							case SDLK_w:
						    	key_pushes[0] = false;
						    	break;

						    case SDLK_s:
						    	key_pushes[1] = false;
						    	break;

						    case SDLK_UP:
							    key_pushes[2] = false;
							    break;
							    
						    case SDLK_DOWN:
							    key_pushes[3] = false;
							    break;
						}
						break;

					case SDL_MOUSEBUTTONDOWN:
					    switch (event.button.button)
					    {
					        case SDL_BUTTON_LEFT:
					        	key_pushes[4] = true;
					            break;

					        case SDL_BUTTON_RIGHT:
					        	key_pushes[5] = true;
					        	break;
					    }
					    break;

					case SDL_MOUSEBUTTONUP:
					    switch (event.button.button)
					    {
					        case SDL_BUTTON_LEFT:
					        	key_pushes[4] = false;
					            break;
					        case SDL_BUTTON_RIGHT:
					        	key_pushes[5] = false;
					        	break;
					    }
					    break;
					case SDL_MOUSEMOTION:
						mouse_coords.x = event.motion.x;
    					mouse_coords.y = event.motion.y;
						break;
				}
			}

			accumulator -= time_step;

			float current_frame_ticks = SDL_GetTicks();
			const float delta_time = (current_frame_ticks - last_frame_ticks) / 1000;

			//updating
			switch(current_scene)
			{
				case 0:
					main_menu.update(key_pushes, mouse_coords, &current_scene);
					break;
				case 1:
					current_game.update(canvas, key_pushes, delta_time);
					break;
			}

			last_frame_ticks = SDL_GetTicks();
			updated = true;
		}

		if(!updated)
		{
			//interpolation
			const float alpha_time = accumulator / time_step;
		}

		window.clear();

		// rendering
		switch(current_scene)
		{
			case 0:
				main_menu.render(window);
				break;
			case 1:
				current_game.render(window);
				break;
		}
		
		// displaying
		window.display();

		updated = false;
	}

	window.clean_up();
	SDL_Quit();

	return 0;
}