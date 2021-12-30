#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"
#include "Ball.hpp"

Ball::Ball(Vector2f p_pos, Vector2f p_scale, SDL_Texture* p_texture, SDL_Rect p_imgdata)
	:Entity(p_pos, p_scale, p_texture)
{
	Entity::setCurrentFrame(p_imgdata);
}