#include "Math.hpp"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <cmath>

bool Triangle::is_point_within(const Vector2f& point)
{
    Triangle t1(point1, point2, point);
    Triangle t2(point1, point, point3);
    Triangle t3(point, point2, point3);

    return get_area() == t1.get_area() + t2.get_area() + t3.get_area();
}

bool game_math::rect_collide(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	if(rect1.x < rect2.x + rect2.w &&
        rect1.x + rect1.w > rect2.x &&
        rect1.y < rect2.y + rect2.h &&
        rect1.h + rect1.y > rect2.y)
        return true;
    else
    	return false;
}	

float game_math::clamp(const float& number, const float& clamp1, const float& clamp2)
{
    if(clamp1 <= number && number <= clamp2)
        return number;
    if(number < clamp1)
        return clamp1;
    return clamp2;
}

int game_math::lowest_float(std::vector<float>& numbers)
{
    float lowest = numbers[0];
    int lowest_index = 0;
    for(int i = 1; (unsigned)i < numbers.size(); i++)
    {
        if(numbers[i] < lowest)
        {
            lowest = numbers[i];
            lowest_index = i;
        }
    }
    return lowest_index;
}

float game_math::distance(Vector2f point1, Vector2f point2)
{
    float x_comp = pow((point1.x - point2.x), 2);
    float y_comp = pow((point1.y - point2.y), 2);
    float distance = sqrt(x_comp + y_comp);
    return distance;
}

bool game_math::contains_point(const SDL_Rect& rect, const Vector2i& point)
{
    return point.x >= rect.x && point.x < rect.x + rect.w && point.y >= rect.y && point.y < rect.y + rect.h;
}

Timer::Timer(const float& p_time)
{
    time = p_time;
}