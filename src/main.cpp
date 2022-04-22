#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Math.hpp"

#include "FullDisplay.hpp"

// global static variables
int utils::display::ORIG_DISPLAY_X;
int utils::display::ORIG_DISPLAY_Y;
int utils::display::DISPLAY_WIDTH;
int utils::display::DISPLAY_HEIGHT;
int utils::display::DISPLAY_X;
int utils::display::DISPLAY_Y;

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
		std::cout << "SDL_Init has failed. Error " << SDL_GetError() << "\n";
	
	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";

	if(TTF_Init() != 0)
		std::cout << "TTF_Init has failed. Error: " << SDL_GetError() << "\n";


	RenderWindow window("Pong", utils::ORIG_DISPLAY_WIDTH, utils::ORIG_DISPLAY_HEIGHT, Vector2f(1, 1));

	SDL_GetWindowPosition(window.get_window(), &utils::display::ORIG_DISPLAY_X, &utils::display::ORIG_DISPLAY_Y);

	utils::display::DISPLAY_WIDTH = utils::ORIG_DISPLAY_WIDTH;
	utils::display::DISPLAY_HEIGHT = utils::ORIG_DISPLAY_HEIGHT;
	utils::display::DISPLAY_X = utils::display::ORIG_DISPLAY_X;
	utils::display::DISPLAY_Y = utils::display::ORIG_DISPLAY_Y;
	
	FullDisplay display(window);


	bool game_running = true;

	SDL_Event event;

	const float time_step = .016666;
	float accumulator = 0.0f;
	float current_time = utils::hire_time_in_seconds();

	// bool updated = false;

	int last_frame_ticks = SDL_GetTicks();

	// main loop
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
				case SDL_WINDOWEVENT:
					if(event.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						SDL_GetWindowSize(window.get_window(), &utils::display::DISPLAY_WIDTH, &utils::display::DISPLAY_HEIGHT);

						utils::display::DISPLAY_WIDTH = static_cast<int>(utils::display::DISPLAY_HEIGHT * (static_cast<double>(utils::ORIG_DISPLAY_WIDTH) / utils::ORIG_DISPLAY_HEIGHT));

						SDL_SetWindowSize(window.get_window(), utils::display::DISPLAY_WIDTH, utils::display::DISPLAY_HEIGHT);

						SDL_GetWindowPosition(window.get_window(), &utils::display::DISPLAY_X, &utils::display::DISPLAY_Y);

						// std::cout << "ORIG X: " << utils::display::ORIG_DISPLAY_X << "\n";
						// std::cout << "ORIG Y: " << utils::display::ORIG_DISPLAY_Y << "\n";
						// std::cout << "X: " << utils::display::DISPLAY_X << "\n";
						// std::cout << "Y: " << utils::display::DISPLAY_Y << "\n";
						// std::cout << "WIDTH: " << utils::display::DISPLAY_WIDTH << "\n";
						// std::cout << "HEIGHT: " << utils::display::DISPLAY_HEIGHT << "\n";

						display.resize_display();
						//display.resize_display(&event); then use SDL_GetWindowPosition
						//
					}
					break;

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
			// updated = true;
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

		// updated = false;
	}

	window.clean_up();
	SDL_Quit();

	return 0;
}