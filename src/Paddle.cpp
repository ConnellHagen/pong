#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math.hpp"
#include "Entity.hpp"
#include "Paddle.hpp"

Paddle::Paddle(Vector2f p_pos, Vector2f p_scale, SDL_Texture* p_texture, SDL_Rect p_imgdata)
	:Entity(p_pos, p_scale, p_texture)
{
	Entity::setOriginalImage(p_imgdata);
	SDL_Rect current = {static_cast<int>(p_pos.x), static_cast<int>(p_pos.y), static_cast<int>(p_scale.x), static_cast<int>(p_scale.y)};
	Entity::setCurrentFrame(current);
}