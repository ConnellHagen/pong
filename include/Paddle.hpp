#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"

class Paddle : public Entity
{
public:
	Paddle(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode);

	inline void set_direction(const int& p_direction){direction = p_direction;}
	inline int get_direction(){return direction;}

	Vector2f next_pos();

	void update();

private:
	//0 for still, 1 for up, 2 for down
	int direction;

};