#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

struct Vector2f
{
	Vector2f()
	:x(0.0f), y(0.0f)
	{}

	Vector2f(const float& p_x, const float& p_y)
	:x(p_x), y(p_y)
	{}

	inline void print(){std::cout << x << ", " << y << "\n";}

	float x, y;

};

struct Vector2i
{
	Vector2i()
	:x(0), y(0)
	{}

	Vector2i(const int& p_x, const int& p_y)
	:x(p_x), y(p_y)
	{}

	inline void print(){std::cout << x << ", " << y << "\n";}

	int x, y;
};

struct Timer
{
	Timer(const float& p_time);

	inline void decrease(const float& delta_time){time -= delta_time;}
	inline void increase(const int& time_add){time += time_add;}
	inline bool is_timer_done(){return time <= 0;}

	float time;
};

struct Triangle
{
	Triangle(const Vector2f& p_point1, const Vector2f& p_point2, const Vector2f& p_point3)
	:point1(p_point1), point2(p_point2), point3(p_point3)
	{}

	inline float get_area(){return abs((point1.x * (point2.y - point3.y) + point2.x * (point3.y - point1.y) + point3.x * (point1.y - point2.y))/2.0);}
	bool is_point_within(const Vector2f& point);

	Vector2f point1, point2, point3;
};

namespace game_math
{
	bool rect_collide(const SDL_Rect& rect1, const SDL_Rect& rect2);
	bool contains_point(const SDL_Rect& rect, const Vector2i& point);
	float clamp(const float& number, const float& clamp1, const float& clamp2);
	int lowest_float(std::vector<float>& numbers);
	float distance(Vector2f point1, Vector2f point2);
}