#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctime>
#include <cmath>


namespace utils
{
	inline float hire_time_in_seconds()
	{
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}

	inline int display_width()
	{
		return 1280;
	}

	inline int display_height()
	{
		return 720;
	}

	inline int random(const int& lower, const int& upper)
	{
		srand((unsigned int) time (NULL));
    	return rand() % (upper - lower + 1) + lower;
	}

}