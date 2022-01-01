#include "Barrier.hpp"

Barrier::Barrier(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect const &p_imgdata)
	:Entity(p_pos, p_scale, p_texture, p_imgdata)
{
	Entity::set_original_image(p_imgdata);
}

void Barrier::update()
{}