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

	enum DIRECTION{NONE, UP, DOWN};
	inline void set_direction(const DIRECTION& p_direction){direction = p_direction;}
	inline int get_direction(){return direction;}

	inline float get_velocity(){return velocity;}
	inline void set_velocity(const int& new_vel){velocity = new_vel;}

	Vector2f next_pos(const float& delta_time);

	void update(Entity* canvas, std::vector<Ball> ball_list, std::vector<bool>& key_pushes, const float& delta_time);

private:
	DIRECTION direction;
	float velocity;

};