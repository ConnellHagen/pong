#include "Math.hpp"
#include "Entity.hpp"
#include "Paddle.hpp"

Paddle::Paddle(Vector2f p_pos, Vector2f p_scale, SDL_Texture* p_texture, SDL_Rect p_imgdata)
	:Entity(p_pos, p_scale, p_texture)
{
	Entity::setCurrentFrame(p_imgdata);
}