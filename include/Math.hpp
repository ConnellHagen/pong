#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct Vector2f
{
	Vector2f()
	:x(0.0f), y(0.0f)
	{}

	Vector2f(float const &p_x, float const &p_y)
	:x(p_x), y(p_y)
	{}

	void print()
	{
		std::cout << x << ", " << y << "\n";
	}

	float x, y;

};

namespace game_math
{
	bool rect_collide(SDL_Rect rect1, SDL_Rect rect2);	
}