#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "Entity.hpp"
#include "Math.hpp"

class Ball;


class Paddle : public Entity
{
public:
	Paddle(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode);

	inline void set_direction(const int& p_direction){direction = p_direction;}
	inline int get_direction(){return direction;}

	inline float get_velocity(){return velocity;}
	inline void set_velocity(const int& new_vel){velocity = new_vel;}

	Vector2f next_pos();

	void update(Entity& canvas, std::vector<Ball>& ball_list, std::vector<bool>& key_pushes);

private:
	//0 for still, 1 for up, 2 for down
	int direction;
	float velocity;

};