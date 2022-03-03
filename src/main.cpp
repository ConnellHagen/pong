#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Math.hpp"

#include "FullDisplay.hpp"

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_Init has failed. Error " << SDL_GetError() << "\n";
	
	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";

	if(TTF_Init() != 0)
		std::cout << "TTF_Init has failed. Error: " << SDL_GetError() << "\n";


	RenderWindow window("Pong", utils::display_width(), utils::display_height(), Vector2f(1, 1));

	FullDisplay display(window);


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
				display.update_keys(&event);

				switch(event.type)
				{
				case SDL_QUIT:
					game_running = false;
					break;
				}
			}

			accumulator -= time_step;

			float current_frame_ticks = SDL_GetTicks();
			const float delta_time = (current_frame_ticks - last_frame_ticks) / 1000;

			// updating
			display.update(delta_time);

			last_frame_ticks = SDL_GetTicks();
			updated = true;
		}

		// if(!updated)
		// {
		// 	// interpolation
		// 	const float alpha_time = accumulator / time_step;
		// }

		window.clear();

		// rendering
		display.render();
		
		// displaying
		window.display();

		updated = false;
	}

	window.clean_up();
	SDL_Quit();

	return 0;
}