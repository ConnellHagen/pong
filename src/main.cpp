#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "RenderWindow.hpp"
#include "Math.hpp"
#include "Entity.hpp"
	#include "Paddle.hpp"
	#include "Barrier.hpp"
	#include "Ball.hpp"
#include "Tile.hpp"

#include "utils.hpp"
#include "Game.hpp"

using std::vector;

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << "\n";
	}

	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";

	RenderWindow window = RenderWindow("Game Title", utils::display_width(), utils::display_height(), Vector2f(1, 1));

	SDL_Rect canvas_inf = {0, 0, utils::display_width(), utils::display_height()};
	Entity canvas = Entity(Vector2f(0, 0), Vector2f(1, 1), NULL, canvas_inf, canvas_inf, 1);

	//W, S, UP, DOWN
	std::vector<bool> key_pushes = std::vector<bool>(4, false);
	
	Game current_game(1, 5, window);




	bool game_running = true;

	SDL_Event event;

	const float time_step = .01;
	float accumulator = 0.0f;
	float current_time = utils::hire_time_in_seconds();

	while(game_running)
	{
		int start_ticks = SDL_GetTicks();

		float new_time = utils::hire_time_in_seconds();
		float frame_time = new_time - current_time;
		current_time = new_time;
		accumulator += frame_time;

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
				}
			}

			accumulator -= time_step;

		}
		// const float alpha = accumulator / deltaTime;
		// const float delta_time;
		

		window.clear();


		//updating
		for(Ball& temp_ball : current_game.get_ball_list())
		{
			temp_ball.update(canvas, current_game.get_paddle_list(), current_game.get_barrier_list());
		}

		int* key_count = new int(0);
		for(Paddle& temp_paddle : current_game.get_paddle_list())
		{
			std::vector<bool> temp_keys = {key_pushes[*key_count], key_pushes[*key_count + 1]};
			temp_paddle.update(canvas, current_game.get_ball_list(), temp_keys);
			*key_count += 2;
		}
		delete key_count;

		for(Barrier& temp_barrier : current_game.get_barrier_list())
		{
			temp_barrier.update();
		}

		//rendering
		window.render(current_game.get_background());

		for(Entity& temp_entity : current_game.get_entity_list())
		{
			window.render(temp_entity);
		}

		for(Ball& temp_ball : current_game.get_ball_list())
		{
			window.render(static_cast<Entity>(temp_ball));
		}

		for(Paddle& temp_paddle : current_game.get_paddle_list())
		{
			window.render(static_cast<Entity>(temp_paddle));
		}

		for(Barrier& temp_barrier : current_game.get_barrier_list())
		{
			window.render(static_cast<Entity>(temp_barrier));
		}


		//displaying
		window.display();

		int frame_ticks = SDL_GetTicks() - start_ticks;

		if(frame_ticks < 1000 / window.get_refresh_rate())
			SDL_Delay(1000 / window.get_refresh_rate() - frame_ticks);

	}

	window.clean_up();
	SDL_Quit();

	return 0;
}