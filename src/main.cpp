#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"
#include "Tile.hpp"
#include "Barrier.hpp"

#include "Math.hpp"
#include "utils.hpp"

using std::vector;

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL_Init has failed. SDL_ERROR: " << SDL_GetError() << "\n";
	}

	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";

	RenderWindow window("Game Title", utils::display_width(), utils::display_height(), Vector2f(1, 1));


	SDL_Texture* background_t = window.load_texture("res/images/background.png");
	SDL_Rect background_inf = {0, 0, 32, 32};

	Background background(Vector2f(4, 4), background_t, background_inf);

	// SDL_Texture* ball_t = window.load_texture("res/images/ball.png");
	// SDL_Rect ball_inf = {0, 0, 16, 16};
	// SDL_Texture* paddle_t = window.load_texture("res/images/paddle.png");
	// SDL_Rect paddle_inf = {0, 0, 32, 128};
	SDL_Texture* barrier_t = window.load_texture("res/images/barrier.png");
	SDL_Rect barrier_inf = {0, 0, 64, 64};
	// SDL_Texture* barrier_large_t = window.load_texture("res/images/barrier.png");

	vector<Entity> entities = 
	{
		Barrier(Vector2f(50, 50), Vector2f(1, 1), barrier_t, barrier_inf),
		Barrier(Vector2f(114, 10), Vector2f(1, 1), barrier_t, barrier_inf)
	};

	bool game_running = true;

	SDL_Event event;

	const float delta_time = .01;
	float accumulator = 0.0f;
	float current_time = utils::hire_time_in_seconds(); 

	while(game_running)
	{
		int start_ticks = SDL_GetTicks();

		float new_time = utils::hire_time_in_seconds();
		float frame_time = new_time - current_time;
		current_time = new_time;
		accumulator += frame_time;

		while(accumulator >= delta_time)
		{
			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_QUIT)
					game_running = false;
			}

			accumulator -= delta_time;

		}
		// const float alpha = accumulator / deltaTime;
		

		window.clear();

		window.render(background);

		for(Entity& temp_entity : entities)
		{
			window.render(temp_entity);
		}

		window.display();

		int frame_ticks = SDL_GetTicks() - start_ticks;

		if(frame_ticks < 1000 / window.get_refresh_rate())
			SDL_Delay(1000 / window.get_refresh_rate() - frame_ticks);

	}

	window.clean_up();
	SDL_Quit();

	return 0;
}