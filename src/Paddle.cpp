#include "Paddle.hpp"

Paddle::Paddle(const Vector2f& p_pos, const Vector2f& p_scale, SDL_Texture* p_texture, const SDL_Rect& p_sheet, const SDL_Rect& p_current, const int& p_render_mode)
	:Entity(p_pos, p_scale, p_texture, p_sheet, p_current, p_render_mode), direction(0)
{
}

Vector2f Paddle::next_pos()
{
	Vector2f new_pos = get_pos();
	switch(direction)
	{
		case 0:
			break;
		case 1:
			new_pos.y -= 6;
			break;
		case 2:
			new_pos.y += 6;
			break;
	}
	return new_pos;
}

void Paddle::update()
{
	Entity::set_pos(next_pos());
}