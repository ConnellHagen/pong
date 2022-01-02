#include "Paddle.hpp"

Paddle::Paddle(Vector2f const &p_pos, Vector2f const &p_scale, SDL_Texture* const &p_texture, SDL_Rect const &p_imgdata, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_imgdata, p_render_mode), direction(0)
{
	Entity::set_original_image(p_imgdata);
}

Vector2f Paddle::next_pos()
{
	Vector2f new_pos = get_pos();
	switch(direction)
	{
		case 0:
			break;
		case 1:
			new_pos.y -= 4;
			break;
		case 2:
			new_pos.y += 4;
			break;
	}
	return new_pos;
}

void Paddle::update()
{
	Entity::set_pos(next_pos());
}