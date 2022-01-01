#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"

class Ball : public Entity
{
public:
	Ball(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect const &p_imgdata);

	inline Vector2f get_velocity(){return velocity;}
	inline void set_velocity(Vector2f const &p_velocity){velocity = p_velocity;}
	void scale_velocity(Vector2f const &p_scale);

	inline float get_rotation_velocity(){return rotation_velocity;}
	void random_rotation_velocity();
	void set_rotation_direction(int const &direction);
	int get_rotation_direction();
	inline void reverse_rotation_direction(){rotation_velocity *= -1;}


	Vector2f next_pos();
	
	void update();

private:
	Vector2f velocity;
	float rotation_velocity;

};