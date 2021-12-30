#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>

#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Paddle.hpp"
#include "Ball.hpp"
#include "Math.hpp"
#include "utils.hpp"

using std::vector;

int main(int argc, char* args[])
{
	if(SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		std::cout << "SDL_Init has FAILED. SDL_ERROR: " << SDL_GetError() << "\n";
	}

	if(!(IMG_Init(IMG_INIT_PNG)))
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << "\n";

	RenderWindow window("Game Title", 1280, 720);

	SDL_Texture* ball_t = window.loadTexture("res/images/ball.png");
	SDL_Rect ball_inf = {0, 0, 16, 16};
	// SDL_Texture* paddle_t = window.loadTexture("res/images/paddle.png");
	// SDL_Rect paddle_inf = {0, 0, 32, 128};
	// SDL_Texture* barrier_t = window.loadTexture("res/images/barrier.png");
	// SDL_Texture* barrier_large_t = window.loadTexture("res/images/barrier.png");

	vector<Entity> entities = 
	{
		Ball(Vector2f(100, 100), Vector2f(10, 10), ball_t, ball_inf), 
		Ball(Vector2f(100, 100), Vector2f(10, 10), ball_t, ball_inf)
	};

	bool gameRunning = true;

	SDL_Event event;

	const float deltaTime = .01;
	float accumulator = 0.0f;
	float currentTime = utils::hireTimeInSeconds(); 

	while(gameRunning)
	{
		int startTicks = SDL_GetTicks();

		float newTime = utils::hireTimeInSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += frameTime;

		while(accumulator >= deltaTime)
		{
			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_QUIT)
					gameRunning = false;
			}

			accumulator -= deltaTime;

		}

		// const float alpha = accumulator / deltaTime;
		

		window.clear();

		for(Entity& temp_entity : entities)
		{
			window.render(temp_entity);
		}

		window.display();

		int frameTicks = SDL_GetTicks() - startTicks;

		if(frameTicks < 1000 / window.getRefreshRate())
			SDL_Delay(1000 / window.getRefreshRate() - frameTicks);

	}

	window.cleanUp();
	SDL_Quit();

	return 0;
}