#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <ctime>
#include <cmath>
#include <vector>


namespace utils
{
	inline float hire_time_in_seconds()
	{
		float t = SDL_GetTicks();
		t *= 0.001f;

		return t;
	}

	inline int random(const int& lower, const int& upper)
	{
		srand((unsigned int) time (NULL));
    	return rand() % (upper - lower + 1) + lower;
	}

	constexpr int ORIG_DISPLAY_WIDTH{1280};
	constexpr int ORIG_DISPLAY_HEIGHT{720};

	class display
	{
	public:
		static int DISPLAY_WIDTH;
		static int DISPLAY_HEIGHT;
	};

}