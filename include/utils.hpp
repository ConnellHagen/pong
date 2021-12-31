#pragma once

#include <SDL2/SDL.h>

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
}