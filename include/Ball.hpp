#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"

class Ball : public Entity
{
public:
	Ball(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode);

	inline Vector2f get_velocity(){return velocity;}
	inline void set_velocity(const Vector2f& p_velocity){velocity = p_velocity;}
	void bounce_y_velocity(const float& percent_off);
	void scale_velocity(const Vector2f& p_scale);

	inline float get_rotation_velocity(){return rotation_velocity;}
	void random_rotation_velocity();
	void set_rotation_direction(const int& direction);
	int get_rotation_direction();
	inline void reverse_rotation_direction(){rotation_velocity *= -1;}

	Vector2f next_pos();
	
	void update();

private:
	Vector2f velocity;
	float rotation_velocity;
	int net_velocity;

	void update_x_velocity();
};