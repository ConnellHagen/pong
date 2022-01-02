#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"

class Paddle : public Entity
{
public:
	Paddle(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect const &p_imgdata, const int& p_render_mode);

	inline void set_direction(int const &p_direction){direction = p_direction;}

	Vector2f next_pos();

	void update();

private:
	//0 for still, 1 for up, 2 for down
	int direction;

};