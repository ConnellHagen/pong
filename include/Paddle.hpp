#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"
#include "Math.hpp"

class Paddle : public Entity
{
public:
	Paddle(Vector2f p_pos, Vector2f p_scale, SDL_Texture* p_texture, SDL_Rect p_imgdata);

};