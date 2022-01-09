#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Math.hpp"
#include "utils.hpp"

#include "Paddle.hpp"
#include "Barrier.hpp"
#include "Ball.hpp"
#include "Tile.hpp"

using std::vector;

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << "\n";
	}

	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";


	SDL_Rect canvas_inf = {0, 0, utils::display_width(), utils::display_height()};
 	Entity canvas(Vector2f(0, 0), Vector2f(1, 1), NULL, canvas_inf, canvas_inf, 1);

	RenderWindow window("Game Title", utils::display_width(), utils::display_height(), Vector2f(1, 1));


	SDL_Texture* background_t = window.load_texture("res/images/background.png");
	SDL_Rect background_inf = {0, 0, 32, 32};

	Background background(Vector2f(4, 4), background_t, background_inf);


	SDL_Texture* ball_t = window.load_texture("res/images/ball.png");
	SDL_Rect ball_inf = {0, 0, 16, 16};
	SDL_Texture* paddle_t = window.load_texture("res/images/paddle.png");
	SDL_Rect paddle_inf = {0, 0, 32, 128};
	SDL_Texture* barrier_t = window.load_texture("res/images/barrier.png");
	SDL_Rect barrier_inf = {0, 0, 64, 64};
	SDL_Rect barrier_infsheet = {0, 0, 256, 128};
	SDL_Texture* barrier_large_t = window.load_texture("res/images/barrier_large.png");
	SDL_Rect barrier_large_inf = {0, 0, 128, 128};
	SDL_Rect barrier_large_infsheet = {0, 0, 512, 256};

	vector<Entity> entities = {};

	vector<Ball> balls =
	{
		Ball(Vector2f(utils::display_width()/2, utils::display_height()/2), Vector2f(2, 2), ball_t, ball_inf, ball_inf, 5)
	};

	vector<Paddle> paddles =
	{
		Paddle(Vector2f(50, utils::display_height()/2), Vector2f(1, 1), paddle_t, paddle_inf, paddle_inf, 4),
		Paddle(Vector2f(utils::display_width() - 50, utils::display_height()/2), Vector2f(1, 1), paddle_t, paddle_inf, paddle_inf, 6)
	};

	vector<Barrier> barriers = {
		Barrier(Vector2f(utils::display_width()/2, 0), Vector2f(1, 1), barrier_large_t, barrier_large_infsheet, barrier_large_inf, 2),
		Barrier(Vector2f(utils::display_width()/2, utils::display_height()), Vector2f(1, 1), barrier_large_t, barrier_large_infsheet, barrier_large_inf, 8),
		Barrier(Vector2f(utils::display_width()/3, utils::display_height()/3), Vector2f(1, 1), barrier_t, barrier_infsheet, barrier_inf, 5),
		Barrier(Vector2f(utils::display_width()*2/3, utils::display_height()*2/3), Vector2f(1, 1), barrier_t, barrier_infsheet, barrier_inf, 5)
	};

	//W, S, UP, DOWN
	vector<bool> key_pushes(4, false);


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
		for(Ball& temp_ball : balls)
		{
			temp_ball.update(canvas, paddles, barriers);
		}

		int* key_count = new int(0);
		for(Paddle& temp_paddle : paddles)
		{
			std::vector<bool> temp_keys = {key_pushes[*key_count], key_pushes[*key_count + 1]};
			temp_paddle.update(canvas, balls, temp_keys);
			*key_count += 2;
		}
		delete key_count;

		for(Barrier& temp_barrier : barriers)
		{
			temp_barrier.update();
		}

		//rendering
		window.render(background);

		for(Entity& temp_entity : entities)
		{
			window.render(temp_entity);
		}

		for(Ball& temp_ball : balls)
		{
			window.render(static_cast<Entity>(temp_ball));
		}

		for(Paddle& temp_paddle : paddles)
		{
			window.render(static_cast<Entity>(temp_paddle));
		}

		for(Barrier& temp_barrier : barriers)
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